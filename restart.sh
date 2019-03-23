#!/bin/bash
set -x
set -e
# Determine the current working directory
WORKINGDIR=$(dirname $(readlink -f "$0"))

"$WORKINGDIR/afl/afl-fuzz" -i - -f "$WORKINGDIR/input/test.c" -o afl-output -x "$WORKINGDIR/dictionary" -- "$WORKINGDIR/fuzzer-cli/cppcheck-fuzzing-client" --type1 "$WORKINGDIR/input/test.c" 
