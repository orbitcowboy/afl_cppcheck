#!/bin/bash
# A script to reduce the size of collected crashes
# Requirements: sudo apt-get install fupdes
# cleanup from previous run
rm -fR crashes
mkdir crashes
# copy crashes from afl_output into crashes folder
cp -f alf_output/crashes/* crashes/.
cd crashes
# cleanup potential duplicates
fdupes -N -d -q 
# rename files for scanning with cppcheck
j=0; for i in id* ; do let j+=1 ; mv $i crash$j.c ; done
# reduce with afl-tmin
j=0; for i in *.c ; do let j+=1 ; ../afl/afl-tmin -i $i -o r$i -- ../cppcheck/cppcheck --enable=all --inconclusive @@ ; done
# cleanup
rm crash*.c
j=0; for i in rcrash*.c ; do let j+=1 ; mv $i crash$j.c ; done
# cleanup potential duplicates
fdupes -N -d -q 
