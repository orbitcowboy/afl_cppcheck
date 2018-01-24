
# a small client for fuzzers. binary input from fuzzers (afl) is converted to source code.
TARGET_NAME=cppcheck-fuzzing-client
# Set afl-g++ as default compiler
AFL_GPP=../afl/afl-g++
CXX=${AFL_GPP}
CXX_OPT=-std=c++11 -g -static -O1
CPPCHECK_DIR=../cppcheck
INCLUDE_DIR=-I ${CPPCHECK_DIR}/lib -I ${CPPCHECK_DIR}/externals/simplecpp -I ${CPPCHECK_DIR}/externals/tinyxml
SRC_FILES=main.cpp ${CPPCHECK_DIR}/externals/simplecpp/simplecpp.cpp ${CPPCHECK_DIR}/externals/tinyxml/tinyxml2.cpp ${CPPCHECK_DIR}/lib/*.cpp
all: ${TARGET_NAME}

${TARGET_NAME}:
	$(CXX) ${CXX_OPT} -o ${TARGET_NAME} ${INCLUDE_DIR} ${SRC_FILES}

clean:
	rm -f ${TARGET_NAME}
