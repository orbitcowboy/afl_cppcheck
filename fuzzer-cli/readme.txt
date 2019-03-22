This is a cppcheck client for fuzzers.

the binary input from the fuzzers is translated into source code.

Command line:
cppcheck-fuzzing-client [--translate-input] [--type1] <file>

--translate-input    only show translated code
--type1              generate code "type 1"


$ cat testcases/1.txt
AA
$ ./cppcheck-fuzzing-client --translate-input testcases/1.txt
 & | { switch -5}( )



