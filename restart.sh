#!/bin/bash
set -x
set -e
# Determine the current working directory
WORKINGDIR=$(dirname $(readlink -f $0))

${WORKINGDIR}/afl/afl-fuzz -i - -f ${WORKINGDIR}/input/test.c -o afl-output -x dictionary -- ${WORKINGDIR}/fuzzer-cli/cppcheck-fuzzing-client 1 input/test.c 
