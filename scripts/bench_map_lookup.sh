#!/bin/bash
set -e
DIR="$( cd "$( dirname "${BASH_SOURCE}" )" && pwd )"

ROOT_DIR="${DIR}/.."
BUILD_DIR="${ROOT_DIR}/build/unbench"

mkdir -p "${BUILD_DIR}"
(cd "${BUILD_DIR}" && cmake "${ROOT_DIR}" -DCMAKE_BUILD_TYPE=Release\
    -DBUILD_WITH_UNBENCH=ON\
    -DBUILD_WITH_UNTESTS=OFF)
(cd "${BUILD_DIR}" && cmake --build .)

(cd "${BUILD_DIR}" && ./unbench/flat.hpp.unbench\
    --benchmark_filter=_map_lookup\
    --benchmark_format=csv > benchmark_map_lookup.csv)

(cd "${BUILD_DIR}" && "${ROOT_DIR}/scripts/bench_drawer.py" -f benchmark_map_lookup.csv)
