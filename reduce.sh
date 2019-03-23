#!/bin/bash
set -x
set -e
# A script to reduce the size of collected crashes
# Requirements: sudo apt-get install fdupes
# cleanup from previous run
# Determine the current working directory
WORKINGDIR=$(dirname $(readlink -f "$0"))
CURRENT_DATE=$(date +'%m_%d_%Y-%H-%M-%S')
rm -fR "$WORKINGDIR/crashes"
mkdir "$WORKINGDIR/crashes"
# copy crashes from afl-output into crashes folder
cp -f "$WORKINGDIR"/afl-output/crashes/id* "$WORKINGDIR"/crashes/.
cd "$WORKINGDIR/crashes"
# cleanup potential duplicates
fdupes -N -d -q .
# Execute cppcheck-fuzzing-client to generate files 
j=0; for i in id* ; do (( j+=1 )) ; "$WORKINGDIR"/fuzzer-cli/cppcheck-fuzzing-client --type1 --translate-input "$i" > crash"$j".c; done
# Cleanup files from afl-output/crashes
rm -f id*
# reduce with afl-tmin
j=0; for i in *.c ; do (( j+=1 )) ; "$WORKINGDIR/afl/afl-tmin" -t 5000 -i "$i" -o r"$i" -- "$WORKINGDIR"/cppcheck/cppcheck --enable=all --inconclusive @@ ; done
# cleanup original crashes and keep reduced
rm crash*.c
j=0; for i in rcrash*.c ; do (( j+=1 )) ; mv "$i" crash_"$CURRENT_DATE"_"$j".c ; done
# cleanup potential duplicates
fdupes -N -d -q .
# go back to working directory
cd "$WORKINGDIR"
