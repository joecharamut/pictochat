#!/bin/bash

cd cmake-build-web

python2 -m SimpleHTTPServer 8080 &
PID=$!

firefox -new-tab http://localhost:8080/project.html

handle () {
    trap SIGINT
    kill ${PID}
    wait
    exit
}
trap "handle" INT

wait