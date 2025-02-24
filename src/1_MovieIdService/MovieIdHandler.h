//
// Created by tank on 2/18/23.
//

#ifndef LUMINE_EXPERIMENTS_MOVIEIDHANDLER_H
#define LUMINE_EXPERIMENTS_MOVIEIDHANDLER_H

#include "MediaServiceTypes.h"
#include "utils_mongodb.h"
#include "utils_memcached.h"
#include "utils.h"
#include "ClientPool.h"
#include "MCClient.h"
#include "utils_for_test.h"
#include "base/logging.h"

namespace media_service {
    using json = nlohmann::json;

    int init_movie_id(mongoc_client_pool_t *&mongoc_client_pool, ClientPool<MCClient> *&mc_client_pool) {
        json config_json;

        // step 01: init logger
        init_logger();

        // step 02: loca config
        if (load_config(&config_json) != 0)
            return -1;

        // step 03: set memcache & mongodb client pool
        mc_client_pool = init_memcached_client_pool(config_json, "movie-id", MONGODB_POOL_MAX_SIZE);
        mongoc_client_pool = init_mongodb_client_pool(config_json, "movie-id", MONGODB_POOL_MAX_SIZE);
        if (mongoc_client_pool == nullptr ||
            mc_client_pool == nullptr) {
            return -1;
        }

        return 0;
    }

    class MovieIdHandler {
    public:
        explicit MovieIdHandler(FaasWorker *faas_worker, ClientPool<MCClient> *mc_client_pool,
                                mongoc_client_pool_t *mongodb_client_pool)
                : faas_worker_(faas_worker),
                  mc_client_pool_(mc_client_pool),
                  mongodb_client_pool_(mongodb_client_pool) {}

        int UploadMovieId(Title &val);

        int RegisterMovieId(TitleForRegister &val);

    private:
        FaasWorker *faas_worker_;
        ClientPool<MCClient> *mc_client_pool_;
        mongoc_client_pool_t *mongodb_client_pool_;
    };

    int MovieIdHandler::UploadMovieId(Title &val) {
        const char *output;
        size_t output_length;
        int ret1, ret2;

        auto mc_client = mc_client_pool_->Pop();
        if (!mc_client) {
            fprintf(stderr, "E UploadMovieId: no available mc-client\n");
            return -1;
        }

        size_t movie_id_size;
        uint32_t memcached_flags;

        // Look for the movie id from memcached
        bool found;
        std::string movie_id_mmc;
        bool success = mc_client->Get(base64_encode(val.title), &found, &movie_id_mmc, &memcached_flags);
        if (!success) {
            mc_client_pool_->Push(mc_client);
            fprintf(stderr, "E UploadMovieId: Cannot get components of request[%ld]\n", val.req_id);
            return -1;
        }
        mc_client_pool_->Push(mc_client);

        std::string movie_id_str;
        if (found) {
            // If cached in memcached
            fprintf(stderr, "I Get movie_id %s, cache hit from memcached\n", movie_id_mmc.c_str());
            movie_id_str = std::string(movie_id_mmc);
        } else {
            // If not cached in memcached
            // get a mongoc client
            auto mongodb_client = mongoc_client_pool_pop(mongodb_client_pool_);
            if (mongodb_client == nullptr) {
                fprintf(stderr, "E no available mongodb client in pool\n");
                return -1;
            }

            // get collection
            auto collection = mongoc_client_get_collection(mongodb_client, "movie-id", "movie-id");
            if (collection == nullptr) {
                fprintf(stderr, "E review collection does not exist\n");
                mongoc_client_pool_push(mongodb_client_pool_, mongodb_client);
                return -1;
            }

            bson_t *query = bson_new();
            BSON_APPEND_UTF8(query, "title", val.title);
            mongoc_cursor_t *cursor = mongoc_collection_find_with_opts(
                    collection, query, nullptr, nullptr);
            const bson_t *doc;
            found = mongoc_cursor_next(cursor, &doc);

            bool not_found_and_others = false;
            if (found) {
                bson_iter_t iter;
                if (bson_iter_init_find(&iter, doc, "movie_id")) {
                    movie_id_str = std::string(bson_iter_value(&iter)->value.v_utf8.str);
                    fprintf(stderr, "I Find movie %s cache miss\n", movie_id_str.c_str());
                } else {
                    not_found_and_others = true;
                    fprintf(stderr, "E Attribute movie_id is not find in MongoDB\n");
                }
            } else {
                not_found_and_others = true;
                fprintf(stderr, "E Movie %s not find in MongoDB\n", val.title);
            }
            bson_destroy(query);
            mongoc_cursor_destroy(cursor);
            mongoc_collection_destroy(collection);
            mongoc_client_pool_push(mongodb_client_pool_, mongodb_client);
            if (not_found_and_others) {
                return -1;
            }
        }

        mc_client = mc_client_pool_->Pop();
        if (mc_client == nullptr) {
            fprintf(stderr, "E UploadMovieId: Failed to pop a client from memcached pool\n");
            return -1;
        }
        success = mc_client->Set(base64_encode(val.title), movie_id_str, 0, 0);
        if (!success) {
            fprintf(stderr, "E Movie %s failed to set movie_id to Memcached\n", val.title);
        }
        mc_client_pool_->Push(mc_client);


        MovieId movie_id{};
        movie_id.req_id = val.req_id;
        strcpy(movie_id.movie_id, movie_id_str.c_str());

//        ret1 = utils::invoke_via_socket("exp02UploadMovieId3", reinterpret_cast<char *>(&movie_id), sizeof(MovieId));

        ret1 = faas_worker_->invoke_func_fn_(faas_worker_->caller_context_,
                                             "exp02UploadMovieId3",
                                             reinterpret_cast<char *>(&movie_id),
                                             sizeof(MovieId),
                                             &output,
                                             &output_length,
                                             ChooseMessagePassingMethod());

        Rating rating = {
                .req_id = val.req_id,
                .rating = val.rating
        };
        ret2 = faas_worker_->invoke_func_fn_(faas_worker_->caller_context_,
                                             "exp02UploadRating3",
                                             reinterpret_cast<char *>(&rating),
                                             sizeof(Rating),
                                             &output,
                                             &output_length,
                                             ChooseMessagePassingMethod());

        if (ret1 != 0 || ret2 != 0) {
            return -1;
        }
        return 0;
    }

    int MovieIdHandler::RegisterMovieId(TitleForRegister &val) {
        // get a mongoc client
        auto mongodb_client = mongoc_client_pool_pop(mongodb_client_pool_);
        if (mongodb_client == nullptr) {
            fprintf(stderr, "E no available mongodb client in pool\n");
            return -1;
        }

        // get collection
        auto collection = mongoc_client_get_collection(mongodb_client, "movie-id", "movie-id");
        if (collection == nullptr) {
            fprintf(stderr, "E review collection does not exist\n");
            mongoc_client_pool_push(mongodb_client_pool_, mongodb_client);
            return -1;
        }

        // Check if the username has existed in the database
        bson_t *query = bson_new();
        BSON_APPEND_UTF8(query, "title", val.title);

        mongoc_cursor_t *cursor = mongoc_collection_find_with_opts(
                collection, query, nullptr, nullptr);
        const bson_t *doc;
        bool found = mongoc_cursor_next(cursor, &doc);

        bool op_error = false;
        if (found) {
            fprintf(stderr, "E Movie %s already existed in MongoDB\n", val.title);
            op_error = true;
        } else {
            bson_t *new_doc = bson_new();
            BSON_APPEND_UTF8(new_doc, "title", val.title);
            BSON_APPEND_UTF8(new_doc, "movie_id", val.movie_id);
            bson_error_t error;

            bool plotinsert = mongoc_collection_insert_one(
                    collection, new_doc, nullptr, nullptr, &error);
            if (!plotinsert) {
                fprintf(stderr, "E Movie %s failed to insert to MongoDB\n", val.title);
                op_error = true;
            }
            bson_destroy(new_doc);
        }
        mongoc_cursor_destroy(cursor);
        mongoc_collection_destroy(collection);
        mongoc_client_pool_push(mongodb_client_pool_, mongodb_client);

        if (op_error)
            return -1;

        return 0;
    }
}

#endif //LUMINE_EXPERIMENTS_MOVIEIDHANDLER_H
