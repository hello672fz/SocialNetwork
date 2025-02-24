include_directories(${EXP02_BASE_DIR}/common)

# mongoc-client
find_package(libmongoc-1.0 1.13 REQUIRED)
find_package(Boost 1.54.0 REQUIRED COMPONENTS log log_setup)

# precondition : build Lumine
set(FUNC_WORKER_CPP_LIBRARIES_NAME worker_cpp mc)

# one: set name
set(EXP02_COMPOSE_REVIEW_0                      exp02_compose_review_0)
set(EXP02_UPLOAD_MOVIE_ID_1                     exp02_upload_movie_id_1)
set(EXP02_UPLOAD_TEXT_1                         exp02_upload_text_1)
set(EXP02_UPLOAD_UNIQUE_ID_1                    exp02_upload_unique_id_1)
set(EXP02_UPLOAD_USER_ID_1                      exp02_upload_user_id_1)
set(EXP02_UPLOAD_RATING_2                       exp02_upload_rating_2)
set(EXP02_UPLOAD_MOVIE_LD_3                     exp02_upload_movie_ld_3)
set(EXP02_UPLOAD_RATING_3                       exp02_upload_rating_3)
set(EXP02_UPLOAD_TEXT_3                         exp02_upload_text_3)
set(EXP02_UPLOAD_UNIQUE_ID_3                    exp02_upload_unique_id_3)
set(EXP02_UPLOAD_USER_ID_3                      exp02_upload_user_id_3)
set(EXP02_UPLOAD_MOVIE_REVIEW_4                 exp02_upload_movie_review_4)
set(EXP02_READ_REVIEW_4                         exp02_read_review_4)
set(EXP02_STORE_REVIEW_4                        exp02_store_review_4)
set(EXP02_UPLOAD_USER_REVIEW_4                  exp02_upload_user_review_4)
set(EXP02_REGISTER_MOVIE_ID                     exp02_register_movie_id)

# two: include
include_directories(${MONGOC_INCLUDE_DIRS})
include_directories(${THRIFT_INCLUDE_DIRS})

# three: add executable files
add_executable(${EXP02_COMPOSE_REVIEW_0}        ${EXP02_BASE_DIR}/src/0_ComposeReviewService/ComposeReview.cpp)
add_executable(${EXP02_UPLOAD_MOVIE_ID_1}       ${EXP02_BASE_DIR}/src/1_MovieIdService/UploadMovieId1.cpp)
add_executable(${EXP02_UPLOAD_TEXT_1}           ${EXP02_BASE_DIR}/src/1_TextService/UploadText1.cpp)
add_executable(${EXP02_UPLOAD_UNIQUE_ID_1}      ${EXP02_BASE_DIR}/src/1_UniqueIdService/UploadUniqueId1.cpp)
add_executable(${EXP02_UPLOAD_USER_ID_1}        ${EXP02_BASE_DIR}/src/1_UserService/UploadUserId1.cpp)
add_executable(${EXP02_UPLOAD_RATING_2}         ${EXP02_BASE_DIR}/src/2_RatingService/UploadRating2.cpp)
add_executable(${EXP02_UPLOAD_MOVIE_LD_3}       ${EXP02_BASE_DIR}/src/3_ComposeReviewService/UploadMovieId3.cpp)
add_executable(${EXP02_UPLOAD_RATING_3}         ${EXP02_BASE_DIR}/src/3_ComposeReviewService/UploadRating3.cpp)
add_executable(${EXP02_UPLOAD_TEXT_3}           ${EXP02_BASE_DIR}/src/3_ComposeReviewService/UploadText3.cpp)
add_executable(${EXP02_UPLOAD_UNIQUE_ID_3}      ${EXP02_BASE_DIR}/src/3_ComposeReviewService/UploadUniqueId3.cpp)
add_executable(${EXP02_UPLOAD_USER_ID_3}        ${EXP02_BASE_DIR}/src/3_ComposeReviewService/UploadUserId3.cpp)
add_executable(${EXP02_UPLOAD_MOVIE_REVIEW_4}   ${EXP02_BASE_DIR}/src/4_MovieReviewService/UploadMovieReview4.cpp)
add_executable(${EXP02_READ_REVIEW_4}           ${EXP02_BASE_DIR}/src/4_ReviewStorageService/ReadReview4.cpp)
add_executable(${EXP02_STORE_REVIEW_4}          ${EXP02_BASE_DIR}/src/4_ReviewStorageService/StoreReview4.cpp)
add_executable(${EXP02_UPLOAD_USER_REVIEW_4}    ${EXP02_BASE_DIR}/src/4_UserReviewService/UploadUserReview4.cpp)
add_executable(${EXP02_REGISTER_MOVIE_ID}       ${EXP02_BASE_DIR}/src/1_MovieIdService/RegisterMovieId.cpp)

# four: link
set(EXP02_COMMON_LIB
        ${FUNC_WORKER_CPP_LIBRARIES_NAME}
        ${MONGOC_LIBRARIES}
        ${Boost_LIBRARIES}
        nlohmann_json mc hiredis)

target_link_libraries(${EXP02_COMPOSE_REVIEW_0}        ${EXP02_COMMON_LIB})
target_link_libraries(${EXP02_UPLOAD_MOVIE_ID_1}       ${EXP02_COMMON_LIB})
target_link_libraries(${EXP02_UPLOAD_TEXT_1}           ${EXP02_COMMON_LIB})
target_link_libraries(${EXP02_UPLOAD_UNIQUE_ID_1}      ${EXP02_COMMON_LIB})
target_link_libraries(${EXP02_UPLOAD_USER_ID_1}        ${EXP02_COMMON_LIB})
target_link_libraries(${EXP02_UPLOAD_RATING_2}         ${EXP02_COMMON_LIB})
target_link_libraries(${EXP02_UPLOAD_MOVIE_LD_3}       ${EXP02_COMMON_LIB})
target_link_libraries(${EXP02_UPLOAD_RATING_3}         ${EXP02_COMMON_LIB})
target_link_libraries(${EXP02_UPLOAD_TEXT_3}           ${EXP02_COMMON_LIB})
target_link_libraries(${EXP02_UPLOAD_UNIQUE_ID_3}      ${EXP02_COMMON_LIB})
target_link_libraries(${EXP02_UPLOAD_USER_ID_3}        ${EXP02_COMMON_LIB})
target_link_libraries(${EXP02_UPLOAD_MOVIE_REVIEW_4}   ${EXP02_COMMON_LIB})
target_link_libraries(${EXP02_READ_REVIEW_4}           ${EXP02_COMMON_LIB})
target_link_libraries(${EXP02_STORE_REVIEW_4}          ${EXP02_COMMON_LIB})
target_link_libraries(${EXP02_UPLOAD_USER_REVIEW_4}    ${EXP02_COMMON_LIB})
target_link_libraries(${EXP02_REGISTER_MOVIE_ID}       ${EXP02_COMMON_LIB})