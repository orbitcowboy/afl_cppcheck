This is a cppcheck client for fuzzers.

the binary input from the fuzzers is translated into source code.

To see the translation from binary code to source code just run the client with a textfile as parameter:

$ cat testcases/1.txt
AA
$ ./cppcheck-fuzzing-client testcases/1.txt
 & | { switch -5}( )

After the translation, the code is analyzed.
