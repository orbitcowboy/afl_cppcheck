# afl_cppcheck
A script to start fuzzing of cppcheck with american fuzzy lop

![A screenshot of afl fuzzing of cppcheck](screenshot/afl_cppcheck.png?raw=true "A screenshot")

# Usage

- Open a Linux console
- git clone https://github.com/orbitcowboy/afl_cppcheck.git
- cd afl_cppcheck
- ./check.sh
- Get a coffee 

# Installation Requirements

Ubuntu

Install libpcre before executing the fuzzing script with
```
 $ sudo apt-get install libpcre3-dev
```


# Utility scripts

```pullAndBuild.sh```:

In case you have already checked out afl-fuzz and cppcheck, but the executables
are old. Use ```pullAndBuild.sh``` to refresh your tool environment. This
script pulls the latest sources and builds all executables required for fuzzing cppcheck.

```freshStart.sh```:

Execute this script in case you intend to start fuzzing cppcheck from new. This script throws 
away previously generated results, stored in afl-output.

```restart.sh```:

This script restarts a previously stopped fuzzing session. The required input data is 
taken from afl-output-folder. 
