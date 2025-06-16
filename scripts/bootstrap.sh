#!/bin/bash

set -e

rm -f compile_commands.json
mkdir -p build
cd build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
cd ..
ln -sf build/compile_commands.json compile_commands.json
