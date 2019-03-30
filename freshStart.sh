#!/bin/bash
set -x
set -e
# Determine the current working directory
WORKINGDIR=$(dirname $(readlink -f "$0"))
INPUT_DIR="$WORKINGDIR/input"
INPUT_FILE="$INPUT_DIR/test.c"
AFL_OUTPUT_DIR="$WORKINGDIR/afl-output"
# Remove the input directory (if exists)
rm -fR "$INPUT_DIR"
mkdir "$INPUT_DIR"
# Generate a random input file of bytes length
head -c 4 /dev/urandom > "$INPUT_FILE"
# Remove the afl-output directory (if exists)
rm -fR "$AFL_OUTPUT_DIR"
# Start fuzzing
"$WORKINGDIR/afl/afl-fuzz" -i "$INPUT_DIR" -f "$INPUT_FILE" -o "$AFL_OUTPUT_DIR" -x "$WORKINGDIR/dictionary" -- "$WORKINGDIR/fuzzer-cli/cppcheck-fuzzing-client" --type2 "$INPUT_FILE" 
