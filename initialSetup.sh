#!/bin/bash
set -x
set -e

# Determine the current working directory
WORKINGDIR=$(dirname $(readlink -f "$0"))

rm -fR "$WORKINGDIR/afl"
rm -fR "$WORKINGDIR/cppcheck"

# Get afl sources
git clone https://github.com/mirrorer/afl.git "$WORKINGDIR/afl"

# Get cppcheck sources
git clone https://github.com/danmar/cppcheck.git "$WORKINGDIR/cppcheck"
cp -fR "$WORKINGDIR/cppcheck" "$WORKINGDIR/cppcheck_asan"
cp -fR "$WORKINGDIR/cppcheck" "$WORKINGDIR/cppcheck_usan"

cd "$WORKINGDIR"
