#!/usr/bin/env bash
if [ ! -d "build" ]; then
    mkdir build
fi
if [ ! -d "bin" ]; then
    mkdir bin
fi
cd build
cmake ..
cd ..
cmake --build ./build
cd ..
