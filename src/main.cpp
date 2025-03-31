#include <prs.h>
#include <string>
using namespace AST;
int main() { 
    std::string code = 
    "def f():\n"
    "  a = 1\n"
    "  return a";

    auto res = Parser(code).parse();
    int x;
}