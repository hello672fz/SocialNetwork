//
// Created by tank on 2/20/23.
//

#include "func_worker_interface.h"
#include "MediaServiceTypes.h"
#include <fmt/format.h>
#include <string>
#include <cstdio>
#include <nlohmann/json.hpp>
#include "utils.h"
#include "utils_for_test.h"
#include "absl/functional/bind_front.h"
#include "ThreadPool.h"

#include "base/thread.h"

using json = nlohmann::json;
using namespace media_service;

struct worker_context {
    void *caller_context;
    faas_invoke_func_fn_t invoke_func_fn;
    faas_append_output_fn_t append_output_fn;
    ThreadPool *thread_pool;
};

struct Job {
    struct worker_context *context;
    uint64_t elapsed_time;
    const char *func_name;
    const char *input;
    size_t input_length;
    bool succeed;

    Job(struct worker_context *context,
        uint64_t elapsed_time,
        const char *func_name,
        const char *input,
        size_t input_length,
        const bool succeed)
            : context(context),
              elapsed_time(0),
              func_name(func_name),
              input(input),
              input_length(input_length),
              succeed(false) {}
};

int faas_init() {
    return 0;
}

int faas_create_func_worker(void *caller_context,
                            faas_invoke_func_fn_t invoke_func_fn,
                            faas_append_output_fn_t append_output_fn,
                            void **worker_handle) {
    auto context = new worker_context;

    context->caller_context = caller_context;
    context->invoke_func_fn = invoke_func_fn;
    context->append_output_fn = append_output_fn;
    context->thread_pool = new ThreadPool(4);

    *worker_handle = context;
    return 0;
}

int invoke(void *arg) {
    auto job = reinterpret_cast<struct Job *>(arg);
    auto context = reinterpret_cast<struct worker_context *>(job->context);

    const char *fn_e_output;
    size_t fn_e_output_length;
    // invoke fn_b via fast internal function call
    auto start_time = utils::get_timestamp_us();


    int ret = context->invoke_func_fn(context->caller_context,
                                      job->func_name,
                                      job->input,
                                      job->input_length,
                                      &fn_e_output,
                                      &fn_e_output_length,
                                      ChooseMessagePassingMethod());
    auto end_time = utils::get_timestamp_us();

    job->elapsed_time = end_time - start_time;
    job->succeed = (ret != -1);

    return 0;
}

int faas_func_call(void *worker_handle,
                   const char *input,
                   size_t input_length) {
#ifdef __EXP02_ENABLE_BREAKDOWN
    auto start_time = utils::get_timestamp_us();
#endif

    auto context = reinterpret_cast<struct worker_context *>(worker_handle);
    auto fn_invoke = absl::bind_front(invoke);
    std::vector<std::shared_ptr<faas::base::Thread>> threads;
    std::vector<std::shared_ptr<Job>> jobs;

    json input_json = json::parse(input);
    if (!input_json.contains("req_id") ||
        !input_json.contains("title") ||
        !input_json.contains("rating") ||
        !input_json.contains("text")) {
        fprintf(stderr, "RegisterMovieId: invalid input");
        return -1;
    }

    auto req_id = input_json.at("req_id").get<long long>();

    std::vector<std::string> downstream_funcs = {"exp02UploadMovieId1", "exp02UploadText1", "exp02UploadUniqueId1"};
//    for (auto &downstream_fun: downstream_funcs) {
//        auto thread = std::make_shared<faas::base::Thread>(fmt::format("Thread-{}", downstream_fun), fn_invoke);
//        auto job = std::make_shared<Job>((worker_context *) worker_handle, 0, downstream_fun.c_str(), input,
//                                         input_length, false);
//
//        thread->Start((void *) job.get());
//
//        jobs.push_back(job);
//        threads.push_back(thread);
//    }
//
//    for (auto &thread: threads) {
//        thread->Join();
//    }
    std::vector<std::future<int>> results;
    for (auto &downstream_fun: downstream_funcs) {
        auto job = std::make_shared<Job>((worker_context *) worker_handle, 0, downstream_fun.c_str(), input,
                                         input_length, false);
        jobs.push_back(job);

        results.emplace_back(context->thread_pool->enqueue(invoke, (void *)job.get()));
    }

    for(auto &&result : results){
        result.get();
    }

    for (auto &job: jobs) {
        if (!job->succeed) {
            return -1;
        }
    }

#ifdef __EXP02_ENABLE_BREAKDOWN
    LOG(INFO) << fmt::format("req_id = {}, t0 {}", req_id, start_time);

    auto end_time = utils::get_timestamp_us();
    LOG(INFO) << fmt::format("req_id = {}, t5 {}", req_id, end_time);
#endif

    return 0;
}

int faas_destroy_func_worker(void *worker_handle) {
    auto context = reinterpret_cast<struct worker_context *>(worker_handle);
    delete context;
    return 0;
}