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
 ",", ";", "#", "##",
 "1", "0.1", "0xff", "-5", "\"abc\"", "'x'"
};

#define NUM(data)  (sizeof(data)/sizeof(*data))

static void writeCode(std::ostream &ostr, int type, unsigned int *value, unsigned int *ones, const unsigned int min) {
  static char par[20] = {' ',0};
  static char parindex;
  const unsigned int num = (type == 0) ? NUM(data0) : NUM(data1);
  const char * const * const data = (type == 0) ? data0 : data1;
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
  const char *filename = nullptr;
  int type = 0;
  bool execCppcheck=true;

  for (int i = 1; i < argc; ++i) {
	if (strcmp(argv[i],"1")==0)
	  type = 1;
	else if (strcmp(argv[i],"x")==0)
	  execCppcheck = false;
	else
	  filename = argv[i];  
  }

  if (!filename) {
    std::cout << "Invalid args\n";
    return 1;
  }

  std::ostringstream ostr;
  std::ifstream f(filename);
  if (!f.is_open()) {
    std::cout << "fopen\n";
    return 1;
  }
  unsigned int value = 0;
  unsigned int ones = 0;
  while (f.good()) {
    unsigned char c = f.get();
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
  std::cout << code << std::endl;
  
  if(execCppcheck) {
    CppcheckExecutor cppcheckExecutor;
    cppcheckExecutor.run(code.c_str());
  }
  return 0;
}
