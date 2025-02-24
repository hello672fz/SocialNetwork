//
// Created by tank on 2/12/23.
//
#include "TextHandler.h"
#include "base/logging.h"
#include "utils_for_test.h"

using namespace media_service;

int faas_init() {
    return 0;
}

int faas_create_func_worker(void *caller_context,
                            faas_invoke_func_fn_t invoke_func_fn,
                            faas_append_output_fn_t append_output_fn,
                            void **worker_handle) {
    auto faas_worker = new FaasWorker();

    faas_worker->caller_context_ = caller_context;
    faas_worker->invoke_func_fn_ = invoke_func_fn;
    faas_worker->append_output_fn_ = append_output_fn;
    faas_worker->handler = reinterpret_cast<void *>(new
            TextHandler(faas_worker));

    *worker_handle = faas_worker;
    return 0;
}


int faas_func_call(void *worker_handle,
                   const char *input,
                   size_t input_length) {
#ifdef __EXP02_ENABLE_BREAKDOWN
    auto start_time = utils::get_timestamp_us();
#endif

    auto faas_worker = reinterpret_cast<FaasWorker *>(worker_handle);
    auto handler = reinterpret_cast<TextHandler *>(faas_worker->handler);

    std::string json_context(input, input_length);
    // parse input
    json input_json = json::parse(json_context);
    if (!input_json.contains("req_id") ||
        !input_json.contains("text")) {
        fprintf(stderr, "UploadText1: invalid input");
        return -1;
    }

    auto req_id = input_json.at("req_id").get<int>();
    auto text = input_json.at("text").get<std::string>();

    Text val{};
    val.req_id = req_id;
    strcpy(val.text, text.c_str());

    int ret = handler->UploadText(val);

#ifdef __EXP02_ENABLE_BREAKDOWN
    LOG(INFO) << fmt::format("req_id = {}, t1 {}", val.req_id, start_time);
#endif

    return ret;
}

int faas_destroy_func_worker(void *worker_handle) {
    auto faas_worker = reinterpret_cast<FaasWorker *>(worker_handle);
    auto handler = reinterpret_cast<TextHandler *>(faas_worker->handler);
    delete handler;
    delete faas_worker;
    return 0;
}