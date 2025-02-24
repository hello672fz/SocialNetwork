#!/bin/bash
source_dir=/tmp/dzl/Lumine-benchmarks
build_dir=${source_dir}/cmake-build-126
#build_dir=/tmp/lumine-benchmarks
kill `ps x | grep ${build_dir} | grep -v grep | awk '{print $1}'`