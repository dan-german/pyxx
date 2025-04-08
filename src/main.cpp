#include "prs.h"
#include "gen.h"

using namespace std;
using namespace ast;

int main() { 
  string code = 
  "def main():\n"
  "  a = 2\n"
  "  b = 3\n"
  "  dbg()\n"
  "  return a*b\n";
  auto ast = Parser(code).parse();
  ast[0]->print();
  gen::emit(mv(ast));
}