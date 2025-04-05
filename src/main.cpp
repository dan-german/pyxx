#include "prs.h"
#include "gen.h"

using namespace std;
using namespace ast;

int main() { 
  string code = 
  "def main():\n"
  "  a = 2\n"
  "  b = 3\n"
  "  return a * b\n";
  // code = 
  // "def main():\n"
  // "  a = 2*3\n"
  // "  return a\n";
  auto ast = Parser(code).parse();
  // for (auto& stmt : ast) {
  //   stmt = fold(std::move(stmt));
  //   stmt->print();
  // }
  // ast[0]->print();
  gen::emit(std::move(ast));
}