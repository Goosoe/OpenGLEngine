#!/bin/bash

# Can send "debug" keyword as a parameter to build the project in debug mode
# builds the project and runs the program
./build.sh $1

./build/opengl/opengl
