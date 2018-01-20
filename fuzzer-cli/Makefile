
# a small client for fuzzers. binary input from fuzzers (afl) is converted to source code.

cppcheck-fuzzing-client:
	$(CXX) -g -static -O1 -o cppcheck-fuzzing-client -I ~/cppcheck/lib -I ~/cppcheck/externals/simplecpp -I ~/cppcheck/externals/tinyxml main.cpp ~/cppcheck/externals/simplecpp/simplecpp.cpp ~/cppcheck/externals/tinyxml/tinyxml2.cpp ~/cppcheck/lib/*.cpp


