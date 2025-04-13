#include "prs.h"
#include "gen.h"
#include <cassert>
#include <fstream>
#include <iostream>

void handle(int argc, char* argv[]) {
  ifstream file(argv[1]);
  string code;
  if (file.is_open()) {
    code = string(istreambuf_iterator<char>(file), { });
  } else {
    print("Unable to open {}\n", argv[1]);
  }
  auto ast = ast::Parser(code).parse();
  auto res = gen::emit(mv(ast));
  print("{}\n", res);
}

int main(int argc, char* argv[]) {
  if (argc > 1) {
    handle(argc, argv);
    return;
  }
}