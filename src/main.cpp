#include "prs.h"
#include "gen.h"
#include <cassert>

using namespace std;
using namespace ast;

int main() {
  string code = 
  "a+b if a==b else a-b\n";
  auto ast = Parser(code).expr();
  ast->print();
}