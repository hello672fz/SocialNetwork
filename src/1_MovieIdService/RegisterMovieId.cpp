//
// Created by tank on 2/12/23.
//
#include "MovieIdHandler.h"

using namespace media_service;

static mongoc_client_pool_t *g_mongodb_client_pool;
static ClientPool<MCClient> *g_mc_client_pool;

int faas_init() {
    return init_movie_id(g_mongodb_client_pool, g_mc_client_pool);
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
            MovieIdHandler(faas_worker, g_mc_client_pool, g_mongodb_client_pool));

    *worker_handle = faas_worker;
    return 0;
}


int faas_func_call(void *worker_handle,
                   const char *input,
                   size_t input_length) {
    auto faas_worker = reinterpret_cast<FaasWorker *>(worker_handle);
    auto handler = reinterpret_cast<MovieIdHandler *>(faas_worker->handler);

    // parse input
    json input_json = json::parse(input);
    if (!input_json.contains("req_id") ||
        !input_json.contains("title") ||
        !input_json.contains("movie_id")) {
        fprintf(stderr, "RegisterMovieId: invalid input");
        return -1;
    }

    auto req_id = input_json.at("req_id").get<int>();
    auto title = input_json.at("title").get<std::string>();
    auto movie_id = input_json.at("movie_id").get<std::string>();

    TitleForRegister val{};
    val.req_id = req_id;
    strcpy(val.title, title.c_str());
    strcpy(val.movie_id, movie_id.c_str());

    int ret = handler->RegisterMovieId(val);

    return ret;
}

int faas_destroy_func_worker(void *worker_handle) {
    auto faas_worker = reinterpret_cast<FaasWorker *>(worker_handle);
    auto handler = reinterpret_cast<MovieIdHandler *>(faas_worker->handler);
    delete handler;
    delete faas_worker;
    return 0;
}