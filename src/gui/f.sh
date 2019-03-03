#!/bin/bash

rm -r build
cmake -S./ -B build -G "Ninja"

cmake --build build
cp ./build/compile_commands.json ./compile_commands.json 
./build/gui
