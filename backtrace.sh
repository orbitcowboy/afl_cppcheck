#!/bin/bash
set -x
set -e
# Create gdb-backtraces 
# Determine the current working directory
WORKINGDIR=$(dirname $(readlink -f "$0"))
cd "$WORKINGDIR/crashes"
# create gdb backtrace files
j=0; for i in crash*.c*; do (( j+=1 )) ; echo "${i}" && gdb -batch-silent -ex "run" -ex "set logging overwrite on" -ex "set logging file gdb.txt" -ex "set logging on" -ex "set pagination off" -ex "handle SIG33 pass nostop noprint" -ex "echo backtrace:\\n" -ex "backtrace full" -ex "echo \\n\\nregisters:\\n\\n" -ex "info registers" -ex "echo \\n\\ncurrent instructions:\\n" -ex "x/16i \$pc" -ex "echo \\n\\nthreads backtrace:\\n" -ex "thread apply all backtrace" -ex "set logging off" -ex "quit" --args "$WORKINGDIR/cppcheck/cppcheck" --enable=all --inconclusive "${i}" && mv gdb.txt "${i}"_bt.txt; done
# go back to working directory
cd "$WORKINGDIR"
