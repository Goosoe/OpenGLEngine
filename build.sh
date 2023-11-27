#!/bin/bash

# builds the program
cd ./build
if [ "$1" = "debug" ]; then
    echo "===Debug build==="
    cmake -DCMAKE_BUILD_TYPE=Debug ..
else
    echo "===Normal build==="
    cmake ..
fi
make
cp compile_commands.json ../compile_commands
