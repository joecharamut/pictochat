#!/bin/bash

source /home/joseph/emsdk/emsdk_env.sh

mkdir -p cmake-build-release-web
#rm -rf cmake-build-web/*
cd cmake-build-release-web

emconfigure cmake \
    -DCMAKE_TOOLCHAIN_FILE=~/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake \
    -G "Unix Makefiles" \
    -DCMAKE_BUILD_TYPE=Release \
    ..

emmake make
