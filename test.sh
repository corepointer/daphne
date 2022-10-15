#!/bin/bash

# Copyright 2021 The DAPHNE Consortium
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# This script builds and runs the test cases for the DAPHNE prototype.
#
# Arguments to this script are forwarded to the test-case executable, i.e., all
# arguments to the catch2 main-function can be used. Invoke `./test.sh -h` or
# see https://github.com/catchorg/Catch2/blob/v2.x/docs/command-line.md for a
# reference.
#
# Invoke this script in the root directory "prototype".

# Stop immediately if any command fails.
set -e

catch2_options=""
BUILD_CUDA="-DUSE_CUDA=OFF"
BUILD_ARROW="-DUSE_ARROW=OFF"
BUILD_FPGAOPENCL="-DUSE_FPGAOPENCL=OFF"
BUILD_DEBUG="-DCMAKE_BUILD_TYPE=Release"

while [[ $# -gt 0 ]]; do
    key=$1
    shift
    case $key in
        --cuda)
            echo using CUDA
            export BUILD_CUDA="-DUSE_CUDA=ON"
            ;;
        --arrow)
            echo using ARROW
            BUILD_ARROW="-DUSE_ARROW=ON"
            ;;
        --fpgaopencl)
            echo using FPGAOPENCL
            export BUILD_FPGAOPENCL="-DUSE_FPGAOPENCL=ON"
            ;;
        --debug)
            echo building DEBUG version
            export BUILD_DEBUG="-DCMAKE_BUILD_TYPE=Debug"
            ;;
        *)
            catch2_options="${catch2_options} ${key}"
            ;;
    esac
done

# Build tests.
./build.sh --target run_tests

# Preparations for running DaphneLib (Python API) tests.
export PYTHONPATH=$PYTHONPATH:$(pwd)/src/
mkdir --parents src/api/python/tmp

# Run tests.
build/test/run_tests $catch2_options

set +e