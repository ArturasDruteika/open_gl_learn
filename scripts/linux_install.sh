#!/usr/bin/env bash
set -euo pipefail


cmake -S . -B build/release \
    -G Ninja \
    -DCMAKE_C_COMPILER=clang \
    -DCMAKE_CXX_COMPILER=clang++ \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX="$PWD/build/install"

cmake --build build/release --parallel
cmake --install build/release