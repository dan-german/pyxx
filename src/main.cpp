#include <print>
#include "prs.h"
#include "gen.h"

using namespace std;
using namespace ast;

int main() {
  string code = 
  "def f():\n"
  "  a = 1\n"
  "  b = a*a-3\n"
  "  return b\n"
  "def a():\n"
  "  return f()";

  auto ast = Parser(code).parse();
  gen::emit(std::move(ast), "shabobr");
}