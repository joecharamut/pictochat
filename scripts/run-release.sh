#!/bin/bash

cd ../cmake-build-release-web/web
source ~/emsdk/emsdk_env.sh

#python2 -m SimpleHTTPServer 8080 &
emrun --serve_after_close index.html
PID=$!

#firefox -new-tab http://localhost:8080/

handle () {
    trap SIGINT
    kill ${PID}
    wait
    exit
}
trap "handle" INT

wait
