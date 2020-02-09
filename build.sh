#!/bin/bash

source /home/joseph/emsdk/emsdk_env.sh

mkdir -p cmake-build-web
#rm -rf cmake-build-web/*
cd cmake-build-web

emconfigure cmake \
    -DCMAKE_TOOLCHAIN_FILE=~/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake \
    -G "Unix Makefiles" \
    -DCMAKE_BUILD_TYPE=Debug \
    ..

emmake make

