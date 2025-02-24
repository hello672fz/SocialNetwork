//
// Created by tank on 2/18/23.
//

#ifndef LUMINE_EXPERIMENTS_TEXTHANDLER_H
#define LUMINE_EXPERIMENTS_TEXTHANDLER_H

#include <nlohmann/json.hpp>
#include "MediaServiceTypes.h"
#include "FaasWorker.h"
#include "string"
#include "utils.h"

namespace media_service {
    using json = nlohmann::json;
    using namespace media_service;

    class TextHandler {
    public:
        explicit TextHandler(FaasWorker *faas_worker)
                : faas_worker_(faas_worker) {}

        int UploadText(Text &val);

    private:
        FaasWorker *faas_worker_;
    };

    int TextHandler::UploadText(Text &val) {
        const char *output;
        size_t output_length;
        int ret = faas_worker_->invoke_func_fn_(faas_worker_->caller_context_,
                                                "exp02UploadText3",
                                                reinterpret_cast<char *>(&val),
                                                sizeof(Text),
                                                &output,
                                                &output_length,
                                                ChooseMessagePassingMethod());
        if (ret != 0) {
            return -1;
        }
        return 0;
    }
}

#endif //LUMINE_EXPERIMENTS_TEXTHANDLER_H
