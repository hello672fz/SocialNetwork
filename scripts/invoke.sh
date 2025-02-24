#!/bin/bash

WRK_BIN=/usr/bin/wrk
WRK_SCRIPT=../wrk2_scripts/compose-review.lua
URL=http://192.168.1.126:8082/function/exp02ComposeReview

# runs a benchmark for 3 seconds, using 5 threads, keeping 20 HTTP connections open
# and a constant throughput of 20 requests per second (total, across all connections combined).
$WRK_BIN -d60s -c10 -t1 -R1000 --u_latency -s $WRK_SCRIPT $URL