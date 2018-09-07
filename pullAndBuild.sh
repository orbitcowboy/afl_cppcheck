#!/bin/bash
set -x
set -e
# This small script pulls the latest version of afl-fuzz and cppcheck
# and builds the executables from:
#  - afl-fuzz
#  - cppcheck   (instrumented by afl-g++)
#  - fuzzer-cli (instrumented by afl-g++)

# Determine the current working directory
WORKINGDIR=$(dirname $(readlink -f $0))
AFL_GPP=../afl/afl-g++
# Update afl-sources and build

cd ${WORKINGDIR}/afl 
git stash
git pull
make all -j4 CXX=clang++ CC=clang

# Update cppcheck-sources and build 
cd ${WORKINGDIR}/cppcheck
git stash
git pull
make CXX=${AFL_GPP} all -j4

# Build fuzzer-cli executable
cd ${WORKINGDIR}/fuzzer-cli/
make clean all

# Return back to previous directory
cd ${WORKINGDIR}



