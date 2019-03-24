#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>

#include "cppcheck.h"

#ifdef NOMAIN
constexpr int type = 1;
constexpr bool execCppcheck = true;
#else
int type = 1;
bool execCppcheck = true;
#endif

class CppcheckExecutor : public ErrorLogger {
private:
    CppCheck cppcheck;

public:
    CppcheckExecutor()
        : ErrorLogger()
        , cppcheck(*this, false) {
        cppcheck.settings().addEnabled("all");
        cppcheck.settings().inconclusive = true;
    }

    void run(const std::string &code) {
        cppcheck.check("test.cpp", code);
    }

    void reportOut(const std::string &outmsg) { }
    void reportErr(const ErrorLogger::ErrorMessage &msg) {}
    void reportProgress(const std::string& filename,
                        const char stage[],
                        const unsigned int value) {}
};

static const char * const data0[] = {
 "(", ")", "{", "}", "[", "]",
 "<", "<=", "==", "!=", ">=", ">",
 "+", "-", "*", "/", "%", "~", "&", "|", "^", "&&", "||", "++", "--", "=", "?", ":"
 "name1", "name2", "name3", "name4", "name5", "name6",
 "const", "void", "char", "int", "enum", "if", "else", "while", "for", "switch", "case", "default", "return", "continue", "break", "struct", "typedef",
 ";", ",",
 "1", "0.1", "0xff", "-5", "\"abc\"", "'x'",
};

static const char * const data1[] = {
 "(", ")", "[", "]", "{", "}",
 "<", "<=", "==", "!=", ">=", ">",
 "+", "-", "*", "/", "%", "~", "&", "|", "^", "&&", "||", "++", "--", "=", "?", ":",
 "name1", "name2", "name3", "name4", "name5", "name6",
 "const", "void", "char", "int", "enum", "if", "else", "while", "for", "switch", "case", "default", "return", "continue", "break", "struct", "typedef",
 ",", ";", "#", "##", ".", "->", "...",
 "1", "0.1", "0xff", "-5", "\"abc\"", "'x'"
};

#define NUM(data)  (sizeof(data)/sizeof(*data))

static void writeCode(std::ostream &ostr, int type, unsigned int *value, unsigned int *ones, const unsigned int min) {
  static char par[20] = {' ',0};
  static char parindex = 0;
  const unsigned int num = (type == 0) ? NUM(data0) : NUM(data1);
  const char * const * const data = (type == 0) ? data0 : data1;
  while (*ones > min) {
      unsigned char i = *value % num;
      *value = *value / num;
      *ones = *ones / num;

      if (parindex < (sizeof(par)-1) && std::strchr("([{", *data[i]))
        par[++parindex] = *data[i+1];
      else if (std::strchr(")]}", *data[i])) {
        while (parindex > 0 && par[parindex] != *data[i])
            ostr << par[parindex--];
        if (parindex > 0)
          parindex--;
        else
          ostr << *data[i-1];
      }
      ostr << ' ' << data[i];
    }
    
    if (min == 0) {
      while (parindex > 0)
        ostr << par[parindex--];
    }
}

std::string generateCode(const uint8_t *data, size_t dataSize) {
  std::ostringstream ostr;
  unsigned int value = 0;
  unsigned int ones = 0;
  for (size_t i = 0; i < dataSize; ++i) {
    uint8_t c = data[i];
    value = (value << 8) | c;
    ones = (ones << 8) | 0xff;
    writeCode(ostr, type, &value, &ones, 0x100);
  }
  writeCode(ostr, type, &value, &ones, 0);

  std::string code;
  switch (type) {
  case 0:
    code = ostr.str();
    break;
  case 1:
    code = "void f() {\n  x=" + ostr.str() + ";\n}";
    break;
  }
  return code;
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t dataSize) {
  
  CppcheckExecutor cppcheckExecutor;
  cppcheckExecutor.run(generateCode(data, dataSize));

  return 0;
}

#ifndef NO_MAIN

int main(int argc, char **argv) {
  const char *filename = nullptr;
  bool execCppcheck = true;

  std::cout << "Command line:";
  for (int i = 0; i < argc; ++i)
    std::cout << " " << argv[i];
  std::cout << "\n";

  for (int i = 1; i < argc; ++i) {
    if (strcmp(argv[i],"--type1")==0)
      type = 1;
    else if (strcmp(argv[i],"--translate-input")==0)
       execCppcheck = false;
    else if (*argv[i] == '-') {
      std::cout << "Invalid option: " << argv[i] << std::endl;
    } else
      filename = argv[i];  
  }

  if (!filename) {
    std::cout << "Invalid args, no filename\n";
    return 1;
  }

  std::ifstream f(filename);
  if (!f.is_open()) {
    std::cout << "failed to open file:" << filename << "\n";
    return 1;
  }

  std::string str((std::istreambuf_iterator<char>(f)),
                   std::istreambuf_iterator<char>());

  if (execCppcheck)
    LLVMFuzzerTestOneInput((const uint8_t *)str.data(), str.size());
  else
    std::cout << generateCode((const uint8_t *)str.data(), str.size()) << std::endl;

  return 0;
}
#endif
