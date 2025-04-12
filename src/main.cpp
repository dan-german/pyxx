#include "prs.h"

int main() {
  std::string x = "a+a";
  auto ast = ast::Parser(x).expr();
  ast->print();
}