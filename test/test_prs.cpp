#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "prs.h"
#include <vector>

using namespace AST;

TEST(prs, parse) {
  {
    auto res = Parser("a=1+2+3\nb=2*3").parse();
    auto expected = std::vector<Node *> { new Var("a", new BOp(new BOp(new IntConst(1), "+", new IntConst(2)), "+", new IntConst(3))) };
    EXPECT_EQ(res.size(), 2);
    EXPECT_TRUE(*res[0] == *expected[0]);
  }
  // {
  //   auto res = Parser("").parse();
  //   auto expected = std::vector<Node *> {
  //     new Var( "a", "int",
  //       new BOp(
  //         new IntConst(2),
  //         '*',
  //         new IntConst(3)))
  //   };
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