#include "prs.h"
#include <print>
#include <string>

int main() { 
  std::string code = 
  "if x== 1 and y ==2:\n" 
  "  a = 2\n" 
  "  return a\n" 
  "else:\n" 
  "  b = 3\n";
  "  return b\n";
  auto res = ast::Parser(code).parse();
  std::print("{}\n", std::string(*res[0]));
}

/* 
If(
  test=BOp(
    BOp(Name(x),==,Int(1)),
    and,
    BOp(Name(y),==,Int(2))),
  then=[Var(a,op==,value=Int(2)),Ret(Name(a))],
  else=[Var(b,op==,value=Int(3))]
)
*/