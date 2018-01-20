#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>

#include "cppcheck.h"


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

    void run(const char code[]) {
        cppcheck.check("test.c", code);
    }

    void reportOut(const std::string &outmsg) { }
    void reportErr(const ErrorLogger::ErrorMessage &msg) {}
    void reportProgress(const std::string& filename,
                        const char stage[],
                        const unsigned int value) {}
};


static const char * const data[] = {
 "(", ")", "{", "}", "[", "]",
 "<", "<=", "==", "!=", ">=", ">",
 "+", "-", "*", "/", "%", "&", "|", "^", "&&", "||", "++", "--", "="
 "name1", "name2", "name3", "name4", "name5", "name6",
 "const", "void", "char", "int", "enum", "if", "else", "while", "for", "switch", "case", "default", "return", "continue", "break", "struct", "typedef",
 ";", ",", ":",
 "1", "0.1", "0xff", "-5", "\"abc\"", "'x'",
};



static void writeCode(std::ostream &ostr, unsigned int *value, unsigned int *ones, const unsigned int min) {
  static char par[20] = {' ',0};
  static char parindex;
  const unsigned int num = sizeof(data) / sizeof(*data);
  while (*ones > min) {
      unsigned char i = *value % num;
      *value = *value / num;
      *ones = *ones / num;

      if (parindex < sizeof(par) && std::strchr("([{", *data[i]))
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

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cout << "Invalid args\n";
    return 1;
  }
  std::ostringstream ostr;
  char inp[64] = {0};
  std::ifstream f(argv[1]);
  if (!f.is_open()) {
    std::cout << "fopen\n";
    return 1;
  }
  unsigned int value = 0;
  unsigned int ones = 0;
  while (f.good()) {
    unsigned char c = f.get();
    value = (value << 8) + c;
    ones = (ones << 8) + 0xff;
    writeCode(ostr, &value, &ones, 0x100);
  }
  writeCode(ostr, &value, &ones, 0);

  const std::string code = ostr.str();
  std::cout << code << std::endl;

  CppcheckExecutor cppcheckExecutor;
  cppcheckExecutor.run(code.c_str());

  return 0;
}
