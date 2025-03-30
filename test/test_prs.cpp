#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "prs.h"
#include <vector>
#include <memory>
using namespace std;
using namespace AST;

TEST(prs, bop) {
#define EXPECT_EQ_EXPR(input,expected) EXPECT_EQ(string(*Parser(input).expr()),expected);
  EXPECT_EQ_EXPR("a+a", "BOp(Name(a),+,Name(a))");
  EXPECT_EQ_EXPR("a+a+a", "BOp(BOp(Name(a),+,Name(a)),+,Name(a))");
  EXPECT_EQ_EXPR("a*a+a", "BOp(BOp(Name(a),*,Name(a)),+,Name(a))");
  EXPECT_EQ_EXPR("a+a*a", "BOp(Name(a),+,BOp(Name(a),*,Name(a)))");
  EXPECT_EQ_EXPR("(a+a)*a", "BOp(BOp(Name(a),+,Name(a)),*,Name(a))");
  EXPECT_EQ_EXPR("a*(a+a)", "BOp(Name(a),*,BOp(Name(a),+,Name(a)))");
}

TEST(prs, defs) {
#define EXPECT_EQ_VAR(input,expected) EXPECT_EQ(string(*Parser(input).parse()[0]),expected);
  EXPECT_EQ_VAR("a=a+1","Var(a,value=BOp(Name(a),+,Int(1)))");
  EXPECT_EQ_VAR("a=1","Var(a,value=Int(1))");
  EXPECT_EQ_VAR("a=x()","Var(a,value=Call(x))");
}