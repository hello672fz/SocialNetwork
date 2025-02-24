#!/bin/bash

source_dir=/tmp/dzl/Lumine-benchmarks
build_dir=${source_dir}/cmake-build-126
output_dir=/tmp/media_service

# set config file path
export CONFIG_PATH=${source_dir}/Experiments/config.json

# set message path
export MessagePathingMethod=IPC
#export MessagePathingMethod=DRC_OVER_IPC
#export MessagePathingMethod=DRC_OVER_Fabric
#export MessagePathingMethod=Fabric

# start gateway
${build_dir}/Lumine/gateway/gateway \
  --config_file=${source_dir}/Experiments/config.json \
  1>${output_dir}/gateway.log 2>&1 &

sleep 2

# start engine
${build_dir}/Lumine/engine/engine \
  --config_file=${source_dir}/Experiments/config.json \
  --guid=101 1>${output_dir}/engine.log 2>&1 &

sleep 2

# start functions
media_service_dir=/experiments/exp02/exp02_media_service

# func_id: 35
${build_dir}${media_service_dir}/exp02_compose_review_0 \
  --debug_file_path=${source_dir}/Experiments/02/debug/compose_review.json \
  1>${output_dir}/exp02_compose_review_0.log 2>&1 &

# func_id: 20
${build_dir}${media_service_dir}/exp02_upload_movie_id_1 \
  --debug_file_path=${source_dir}/Experiments/02/debug/upload_movieid1.json \
  1>${output_dir}/exp02_upload_movie_id_1.log 2>&1 &

# func_id: 21
${build_dir}${media_service_dir}/exp02_upload_text_1 \
  --debug_file_path=${source_dir}/Experiments/02/debug/upload_text1.json \
  1>${output_dir}/exp02_upload_text_1.log 2>&1 &

# func_id: 22
${build_dir}${media_service_dir}/exp02_upload_unique_id_1 \
  --debug_file_path=${source_dir}/Experiments/02/debug/upload_uniqueid1.json \
  1>${output_dir}/exp02_upload_unique_id_1.log 2>&1 &

# func_id: 24
${build_dir}${media_service_dir}/exp02_upload_rating_2 \
  --debug_file_path=${source_dir}/Experiments/02/debug/upload_rating2.json \
  1>${output_dir}/exp02_upload_rating_2.log 2>&1 &

# func_id: 25
${build_dir}${media_service_dir}/exp02_upload_movie_ld_3 \
  --debug_file_path=${source_dir}/Experiments/02/debug/upload_movieid3.json \
  1>${output_dir}/exp02_upload_movie_ld_3.log 2>&1 &

# func_id: 26
${build_dir}${media_service_dir}/exp02_upload_rating_3 \
  --debug_file_path=${source_dir}/Experiments/02/debug/upload_rating3.json \
  1>${output_dir}/exp02_upload_rating_3.log 2>&1 &

# func_id: 27
${build_dir}${media_service_dir}/exp02_upload_text_3 \
  --debug_file_path=${source_dir}/Experiments/02/debug/upload_text3.json \
  1>${output_dir}/exp02_upload_text_3.log 2>&1 &

# func_id: 28
${build_dir}${media_service_dir}/exp02_upload_unique_id_3 \
  --debug_file_path=${source_dir}/Experiments/02/debug/upload_uniqueid3.json \
  1>${output_dir}/exp02_upload_unique_id_3.log 2>&1 &

# func_id: 32
${build_dir}${media_service_dir}/exp02_store_review_4 \
  --debug_file_path=${source_dir}/Experiments/02/debug/store_review4.json \
  1>${output_dir}/exp02_store_review_4.log 2>&1 &

# func_id: 34
${build_dir}${media_service_dir}/exp02_register_movie_id \
  --debug_file_path=${source_dir}/Experiments/02/debug/register_movieid.json \
  1>${output_dir}/exp02_register_movie_id.log 2>&1 &