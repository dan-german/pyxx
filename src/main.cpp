#include "prs.h"

int main() { 
    // AST::Parser p("a=1+2+3\nb=2*3");
    AST::Parser p("def f():\n  a = 2");
    auto res = p.parse();
    int x;
}