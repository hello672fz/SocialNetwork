//
// Created by tank on 2/18/23.
//

#ifndef LUMINE_EXPERIMENTS_UNIQUEIDHANDLER_H
#define LUMINE_EXPERIMENTS_UNIQUEIDHANDLER_H

#include <nlohmann/json.hpp>
#include "MediaServiceTypes.h"
#include "FaasWorker.h"
#include "string"
#include "utils.h"

namespace media_service {
    using json = nlohmann::json;
    using namespace media_service;

    class UniqueIdHandler {
    public:
        explicit UniqueIdHandler(FaasWorker *faas_worker)
                : faas_worker_(faas_worker) {}

        int UploadUniqueId(int64_t req_id);

    private:
        FaasWorker *faas_worker_;
    };

    inline int64_t make_unique_id() {
        // timestamp
        struct timespec time_stamp{};
        clock_gettime(CLOCK_REALTIME, &time_stamp);
        uint64_t timestamp = time_stamp.tv_sec * 1000000 + time_stamp.tv_nsec / 1000;

        std::stringstream sstream;
        sstream << std::hex << timestamp;
        std::string timestamp_hex(sstream.str());

        if (timestamp_hex.size() > 10) {
            timestamp_hex.erase(0, timestamp_hex.size() - 10);
        } else if (timestamp_hex.size() < 10) {
            timestamp_hex = std::string(10 - timestamp_hex.size(), '0') + timestamp_hex;
        }

        // Empty the sstream buffer.
        sstream.clear();
        sstream.str(std::string());

        sstream << std::hex << rand();
        std::string counter_hex(sstream.str());

        if (counter_hex.size() > 3) {
            counter_hex.erase(0, counter_hex.size() - 3);
        } else if (counter_hex.size() < 3) {
            counter_hex = std::string(3 - counter_hex.size(), '0') + counter_hex;
        }
        std::string review_id_str = timestamp_hex + counter_hex;
        int64_t review_id = stoul(review_id_str, nullptr, 16) & 0x7FFFFFFFFFFFFFFF;

        return review_id;
    }

    int UniqueIdHandler::UploadUniqueId(int64_t req_id) {
        const char *output;
        size_t output_length;

        UniqueId val = {
                .req_id = req_id,
                .review_id = make_unique_id()
        };
        int ret = faas_worker_->invoke_func_fn_(faas_worker_->caller_context_,
                                                "exp02UploadUniqueId3",
                                                reinterpret_cast<char *>(&val),
                                                sizeof(UniqueId),
                                                &output,
                                                &output_length,
                                                ChooseMessagePassingMethod());
        if (ret != 0) {
            return -1;
        }
        return 0;
    }
}
#endif //LUMINE_EXPERIMENTS_UNIQUEIDHANDLER_H
