//
// Created by tank on 2/12/23.
//

#ifndef LUMINE_EXPERIMENTS_RATINGHANDLER_H
#define LUMINE_EXPERIMENTS_RATINGHANDLER_H

#include <nlohmann/json.hpp>
#include "RedisClient.h"
#include "MediaServiceTypes.h"
#include "FaasWorker.h"
#include "ClientPool.h"
#include "utils.h"
#include "utils_redis.h"

namespace media_service {
    using json = nlohmann::json;

    int init_rating(ClientPool<RedisClient> *&redis_client_pool) {
        json config_json;

        // step 01: init logger
        init_logger();

        // step 02: loca config
        if (load_config(&config_json) != 0)
            return -1;

        redis_client_pool = init_redis_client_pool(config_json, "rating-redis", REDIS_POOL_MAX_SIZE);
        if (redis_client_pool == nullptr)
            return -1;

        return 0;
    }

    class RatingHandler {
    public:
        RatingHandler(FaasWorker *faas_worker, ClientPool<RedisClient> *redis_client_pool)
                : faas_worker_(faas_worker), redis_client_pool_(redis_client_pool) {}

        int UploadRating(const MovieId &val);

    private:
        FaasWorker *faas_worker_;
        ClientPool<RedisClient> *redis_client_pool_;
    };

    int RatingHandler::UploadRating(const MovieId &val) {
        const char *output;
        size_t output_length;

        Rating rating = {
                .req_id = val.req_id,
                .rating = val.rating
        };

        int ret = faas_worker_->invoke_func_fn_(faas_worker_->caller_context_,
                                                "exp02UploadRating3",
                                                reinterpret_cast<char *>(&rating),
                                                sizeof(Rating),
                                                &output,
                                                &output_length,
                                                ChooseMessagePassingMethod());
        if (ret != 0) {
            return -1;
        }

        auto redis_client_wrapper = redis_client_pool_->Pop();
        if (!redis_client_wrapper) {
            fprintf(stderr, "Cannot connected to Redis server");
            return -1;
        }
        auto redis_client = redis_client_wrapper->GetClient();
        redis_client.AppendCommand("INCRBY %s:uncommit_sum %d", val.movie_id, rating);
        redis_client.AppendCommand("INCR %s:uncommit_num", val.movie_id);
        auto incrby_reply = redis_client.GetReply();
        auto incr_reply = redis_client.GetReply();
        incrby_reply->check_ok();
        incr_reply->check_ok();
        redis_client_pool_->Push(redis_client_wrapper);
        return 0;
    }
}

#endif //LUMINE_EXPERIMENTS_RATINGHANDLER_H
