#include "prs.h"
#include "gen.h"
#include <fstream>

bool DBG = true;

void handle(int argc, char* argv[]) {
  ifstream filePath(argv[1]);
  string code;
  if (filePath.is_open()) {
    code = string(istreambuf_iterator<char>(filePath), { });
  } else {
    print("Unable to open {}\n", argv[1]);
  }
  auto ast = ast::Parser(code).parse();
  auto res = gen::emit(mv(ast));
  print("{}\n", res);
}

int test() {
  string code = 
  "def b(x,y):\n"
  "  a = x == y\n"
  "  return 1 if a else 2";
  auto ast = ast::Parser(code).parse();
  auto res = gen::emit(mv(ast));
  print("{}\n", res);
}

int main(int argc, char* argv[]) {
  if (argc > 1) {
    DBG = false;
    handle(argc, argv);
    return 0;
  }
  test();
  return 0;
}