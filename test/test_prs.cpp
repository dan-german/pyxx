#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "prs.h"
#include <vector>

using namespace AST;

// a = 2 + 3;
// py++ - Python compiler with an LLVM backend, written in C++

TEST(prs, parse) {
  {
    auto res = Parser("int a=2+3;").parse();
    auto expected = std::vector<Node *> { new Var("a", "int", new BOp(new IntConst(2), '+', new IntConst(3))) };
    EXPECT_EQ(res.size(), 1);
    EXPECT_TRUE(*res[0] == *expected[0]);
  }
  // {
  //   auto res = Parser("1+2+3;").parse();
  //   auto expected = std::vector<Node *> { new Var("a", "int", new BOp(new IntConst(2), '*', new IntConst(3))) };
  //   EXPECT_EQ(res.size(), 1);
  //   EXPECT_TRUE(*res[0] == *expected[0]);
  // }
  // {
  //   auto res = Parser("1+2*3;").parse();
  //   auto expected = std::vector<Node *> { new Var("a", "int", new BOp(new IntConst(2), '*', new IntConst(3))) };
  //   EXPECT_EQ(res.size(), 1);
  //   EXPECT_TRUE(*res[0] == *expected[0]);
  // }
}