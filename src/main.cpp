#include "gen.h"

int main() { 
  std::string code = 
  "def main(): \n"
  "  s = 0\n"
  "  f1 = 1\n"
  "  s = s + f1\n"
  "  f2 = 1\n"
  "  s = s + f2\n"
  "  f3 = f1 + f2\n"
  "  s = s + f3\n"
  "  f4 = f3 + f2\n"
  "  s = s + f4\n"
  "  f5 = f4 + f3\n"
  "  s = s + f5\n"
  "  return s\n";
  // code = 
  // "def main(): \n"
  // "  s = 0\n"
  // "  f1 = 1\n"
  // "  s = s + f1\n"
  // "  return s\n";
  auto res = ast::Parser(code).parse();
  gen::emit(std::move(res));
}