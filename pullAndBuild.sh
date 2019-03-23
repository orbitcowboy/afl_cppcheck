#!/bin/bash
set -x
set -e
# This small script pulls the latest version of afl-fuzz and cppcheck
# and builds the executables from:
#  - afl-fuzz
#  - cppcheck   (instrumented by afl-clang++)
#  - fuzzer-cli (instrumented by afl-clang++)

# Determine the current working directory
WORKINGDIR=$(dirname $(readlink -f "$0"))
AFL_GPP=../afl/afl-clang++
# Update afl-sources and build

cd "$WORKINGDIR"/afl 
git stash
git pull
make clean
make all -j12 CXX=clang++ CC=clang

# Update cppcheck-sources and build 
cd "$WORKINGDIR"/cppcheck
git stash
git pull
make clean
make CXX=${AFL_GPP} -j12

# Build fuzzer-cli executable
cd "$WORKINGDIR"/fuzzer-cli/
make CXX=${AFL_GPP} CXXFLAGS="-static -O1" clean all

# Return back to previous directory
cd "$WORKINGDIR"



