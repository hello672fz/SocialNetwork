//
// Created by tank on 2/12/23.
//

#include "ComposeReviewHandler.h"

using namespace media_service;

static ClientPool<MCClient> *g_mc_client_pool;

int faas_init() {
    return init_compose_review(g_mc_client_pool);
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
            ComposeReviewHandler(g_mc_client_pool, faas_worker));

    *worker_handle = faas_worker;
    return 0;
}


int faas_func_call(void *worker_handle,
                   const char *input,
                   size_t input_length) {
    auto faas_worker = reinterpret_cast<FaasWorker *>(worker_handle);
    auto handler = reinterpret_cast<ComposeReviewHandler *>(faas_worker->handler);
    if (input_length != sizeof(UniqueId)) {
        fprintf(stderr, "UploadUniqueId3: illegal input!\n");
        return -1;
    }
    auto unique_id = reinterpret_cast<const UniqueId *>(input);
    int ret = handler->UploadUniqueId(*unique_id);
    return ret;
}

int faas_destroy_func_worker(void *worker_handle) {
    auto faas_worker = reinterpret_cast<FaasWorker *>(worker_handle);
    auto handler = reinterpret_cast<ComposeReviewHandler *>(faas_worker->handler);
    delete handler;
    delete faas_worker;
    return 0;
}