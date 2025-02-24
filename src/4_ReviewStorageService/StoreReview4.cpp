//
// Created by tank on 2/12/23.
//

#include "ReviewStorageHandler.h"
#include "utils_for_test.h"
#include "base/logging.h"

using namespace media_service;

static mongoc_client_pool_t *g_mongodb_client_pool;
static ClientPool<MCClient> *g_mc_client_pool;

int faas_init() {
    return init_review_storage(g_mongodb_client_pool, g_mc_client_pool);
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
            ReviewStorageHandler(g_mc_client_pool, g_mongodb_client_pool));

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
    auto handler = reinterpret_cast<ReviewStorageHandler *>(faas_worker->handler);
    if (input_length != sizeof(Review)) {
        fprintf(stderr, "StoreReview4: illegal input!\n");
        return -1;
    }
    auto review = reinterpret_cast<const Review *>(input);
    int ret = handler->StoreReview(*review);

#ifdef __EXP02_ENABLE_BREAKDOWN
    auto end_time = utils::get_timestamp_us();

    LOG(INFO) << fmt::format("req_id = {}, t3 {}", review->req_id, start_time);
    LOG(INFO) << fmt::format("req_id = {}, t4 {}", review->req_id, end_time);
#endif

    return ret;
}

int faas_destroy_func_worker(void *worker_handle) {
    auto faas_worker = reinterpret_cast<FaasWorker *>(worker_handle);
    auto handler = reinterpret_cast<ReviewStorageHandler *>(faas_worker->handler);
    delete handler;
    delete faas_worker;
    return 0;
}
