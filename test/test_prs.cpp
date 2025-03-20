#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "prs.h"
#include <vector>

TEST(prs, parse) {
  std::string code = "int a=2*3;";
  AST::Parser p(code);
  std::vector<AST::Node *> res = p.parse();
  std::vector<AST::Node *> expected = { new AST::Var("a", "int", new AST::BOp(new AST::IntConst(2), '*', new AST::IntConst(3))) };
  EXPECT_EQ(res.size(), 1);
  EXPECT_TRUE(*res[0] == *expected[0]);
}