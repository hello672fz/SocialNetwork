#!/bin/bash

source_dir=/tmp/dzl/Lumine-benchmarks
build_dir=${source_dir}/cmake-build-126
output_dir=/tmp/media_service

#config for launcher
root_path_for_ipc=/dev/shm/ipc2
device_name=mlx5_0
device_port=1
device_gidx=3

# set config file path
export CONFIG_PATH=${source_dir}/Experiments/config.json

# set message path
#export MessagePathingMethod=IPC
export MessagePathingMethod=DRC_OVER_IPC
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
${build_dir}/Lumine/launcher/launcher \
  --func_id=35 \
  --fprocess=${build_dir}${media_service_dir}/exp02_compose_review_0 \
  --fprocess_output_dir=${output_dir}\
  --root_path_for_ipc=${root_path_for_ipc} \
  --device_name=${device_name} \
  --device_port=${device_port} \
  --device_gidx=${device_gidx} \
  1>${output_dir}/exp02_compose_review_0.log 2>&1 &

# func_id: 20
${build_dir}/Lumine/launcher/launcher \
  --func_id=20 \
  --fprocess=${build_dir}${media_service_dir}/exp02_upload_movie_id_1 \
  --fprocess_output_dir=${output_dir}\
  --root_path_for_ipc=${root_path_for_ipc} \
  --device_name=${device_name} \
  --device_port=${device_port} \
  --device_gidx=${device_gidx} \
  1>${output_dir}/exp02_upload_movie_id_1.log 2>&1 &
# func_id: 21
${build_dir}/Lumine/launcher/launcher \
  --func_id=21 \
  --fprocess=${build_dir}${media_service_dir}/exp02_upload_text_1 \
  --fprocess_output_dir=${output_dir}\
  --root_path_for_ipc=${root_path_for_ipc} \
  --device_name=${device_name} \
  --device_port=${device_port} \
  --device_gidx=${device_gidx} \
  1>${output_dir}/exp02_upload_text_1.log 2>&1 &

# func_id: 22
${build_dir}/Lumine/launcher/launcher \
  --func_id=22 \
  --fprocess=${build_dir}${media_service_dir}/exp02_upload_unique_id_1 \
  --fprocess_output_dir=${output_dir}\
  --root_path_for_ipc=${root_path_for_ipc} \
  --device_name=${device_name} \
  --device_port=${device_port} \
  --device_gidx=${device_gidx} \
  1>${output_dir}/exp02_upload_unique_id_1.log 2>&1 &
# func_id: 24
${build_dir}/Lumine/launcher/launcher \
  --func_id=24 \
  --fprocess=${build_dir}${media_service_dir}/exp02_upload_rating_2 \
  --fprocess_output_dir=${output_dir}\
  --root_path_for_ipc=${root_path_for_ipc} \
  --device_name=${device_name} \
  --device_port=${device_port} \
  --device_gidx=${device_gidx} \
  1>${output_dir}/exp02_upload_rating_2.log 2>&1 &

# func_id: 25
${build_dir}/Lumine/launcher/launcher \
  --func_id=25 \
  --fprocess=${build_dir}${media_service_dir}/exp02_upload_movie_ld_3 \
  --fprocess_output_dir=${output_dir}\
  --root_path_for_ipc=${root_path_for_ipc} \
  --device_name=${device_name} \
  --device_port=${device_port} \
  --device_gidx=${device_gidx} \
  1>${output_dir}/exp02_upload_movie_ld_3.log 2>&1 &

# func_id: 26
${build_dir}/Lumine/launcher/launcher \
  --func_id=26 \
  --fprocess=${build_dir}${media_service_dir}/exp02_upload_rating_3 \
  --fprocess_output_dir=${output_dir}\
  --root_path_for_ipc=${root_path_for_ipc} \
  --device_name=${device_name} \
  --device_port=${device_port} \
  --device_gidx=${device_gidx} \
  1>${output_dir}/exp02_upload_rating_3.log 2>&1 &

# func_id: 27
${build_dir}/Lumine/launcher/launcher \
  --func_id=27 \
  --fprocess=${build_dir}${media_service_dir}/exp02_upload_text_3 \
  --fprocess_output_dir=${output_dir}\
  --root_path_for_ipc=${root_path_for_ipc} \
  --device_name=${device_name} \
  --device_port=${device_port} \
  --device_gidx=${device_gidx} \
  1>${output_dir}/exp02_upload_text_3.log 2>&1 &

# func_id: 28
${build_dir}/Lumine/launcher/launcher \
  --func_id=28 \
  --fprocess=${build_dir}${media_service_dir}/exp02_upload_unique_id_3 \
  --fprocess_output_dir=${output_dir}\
  --root_path_for_ipc=${root_path_for_ipc} \
  --device_name=${device_name} \
  --device_port=${device_port} \
  --device_gidx=${device_gidx} \
  1>${output_dir}/exp02_upload_unique_id_3.log 2>&1 &

# func_id: 32
${build_dir}/Lumine/launcher/launcher \
  --func_id=32 \
  --fprocess=${build_dir}${media_service_dir}/exp02_store_review_4 \
  --fprocess_output_dir=${output_dir}\
  --root_path_for_ipc=${root_path_for_ipc} \
  --device_name=${device_name} \
  --device_port=${device_port} \
  --device_gidx=${device_gidx} \
  1>${output_dir}/exp02_store_review_4.log 2>&1 &

# func_id: 34
${build_dir}/Lumine/launcher/launcher \
  --func_id=34 \
  --fprocess=${build_dir}${media_service_dir}/exp02_register_movie_id \
  --fprocess_output_dir=${output_dir}\
  --root_path_for_ipc=${root_path_for_ipc} \
  --device_name=${device_name} \
  --device_port=${device_port} \
  --device_gidx=${device_gidx} \
  1>${output_dir}/exp02_register_movie_id.log 2>&1 &
