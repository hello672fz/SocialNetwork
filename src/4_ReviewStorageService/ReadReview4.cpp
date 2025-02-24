//
// Created by tank on 2/20/23.
//

#include "func_worker_interface.h"

int faas_init() {
    return 0;
}

int faas_create_func_worker(void *caller_context,
                            faas_invoke_func_fn_t invoke_func_fn,
                            faas_append_output_fn_t append_output_fn,
                            void **worker_handle) {
    return 0;
}

int faas_func_call(void *worker_handle,
                   const char *input,
                   size_t input_length) {
    return 0;
}

int faas_destroy_func_worker(void *worker_handle) {
    return 0;
}