#!/bin/bash
set -e 
# ----------------------------------------------------------------------
#  A script to start fuzzing of cppcheck with american fuzzy lop.
#
#  Parameter:
#
#  	$1 --> A path pointing to C/C++ test cases for afl to get started.
#
# Author: Dr. Martin Ettl
# Date  : 2015-11-06
# 
# Dependencies:
#
# $ sudo apt-get install clang++ clang
# 
# Example:
#
# $ check.sh some_folder
# ----------------------------------------------------------------------

# Validate the number of input parameters
if [ $# -le 0 ]
  then
    /bin/echo "Please call this script with exactly parameter: [path to input folder]."
    /bin/echo "e.g.: check.sh input_folder"
    exit
fi

# Get afl sources
git clone https://github.com/mirrorer/afl.git
cd afl
make clean
# Ensure llvm-config is available. On Ubuntu systems, a symbolic link has to be created:
# cd /usr/bin/ && sudo ln -s llvm-config-3.6 llvm-config
cd llvm_mode
make 
cd ..
make
cd ..

# Get cppcheck sources
git clone http://github.com/danmar/cppcheck.git
cd cppcheck
make clean
# Compile cppcheck with afl
make CXX=../afl/afl-clang-fast++ CC=../afl/afl-clang-fast CXXFLAGS+='-O3 -std=c++0x' HAVE_RULES=yes AFL_HARDEN=1 -j3
cd ..
afl/afl-fuzz -i $1 -o afl-output -x dictionary -- ./cppcheck/cppcheck --enable=all --inconclusive --force --std=posix @@
cd ..

# Optional cleanup
#rm -fR afl
#rm -fr cppcheck
