#!/bin/bash
set -x
set -e
# This small script pulls the latest version of afl-fuzz and cppcheck
# and builds the executables from:
#  - afl-fuzz
#  - cppcheck   (compiled with g++)
#  - fuzzer-cli (instrumented by afl-g++)

# Determine the current working directory
WORKINGDIR=$(dirname $(readlink -f $0))
# Update afl-sources and build

cd ${WORKINGDIR}/afl
git stash
git pull
make clean all -j4

# Update cppcheck-sources and build 
cd ${WORKINGDIR}/cppcheck
git stash
git pull
make clean all -j4

# Build fuzzer-cli executable
cd ${WORKINGDIR}/fuzzer-cli/
make clean all

# Return back to previous directory
cd ${WORKINGDIR}



