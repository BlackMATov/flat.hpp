#!/bin/bash
set -e

BUILD_DIR=`dirname "$BASH_SOURCE"`/../build
cd $BUILD_DIR

./unbench/flat.hpp.unbench --benchmark_filter=_map_lookup --benchmark_format=csv > benchmark_map_lookup.csv
../scripts/bench_drawer.py -f benchmark_map_lookup.csv