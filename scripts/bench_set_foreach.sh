#!/bin/bash
set -e

BUILD_DIR=`dirname "$BASH_SOURCE"`/../build
cd $BUILD_DIR

./unbench/flat.hpp.unbench --benchmark_filter=_set_foreach --benchmark_format=csv > benchmark_set_foreach.csv
../scripts/bench_drawer.py -f benchmark_set_foreach.csv
