#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "prs.h"
#include <vector>
#include <memory>
using namespace std;
using namespace ast;

TEST(prs, bop) {
#define EXPECT_EQ_EXPR(input,expected) EXPECT_EQ(string(*Parser(input).expr()),expected);
  EXPECT_EQ_EXPR("a+a", "BOp(Name(a),+,Name(a))");
  EXPECT_EQ_EXPR("a+a+a", "BOp(BOp(Name(a),+,Name(a)),+,Name(a))");
  EXPECT_EQ_EXPR("a*a+a", "BOp(BOp(Name(a),*,Name(a)),+,Name(a))");
  EXPECT_EQ_EXPR("a+a*a", "BOp(Name(a),+,BOp(Name(a),*,Name(a)))");
  EXPECT_EQ_EXPR("(a+a)*a", "BOp(BOp(Name(a),+,Name(a)),*,Name(a))");
  EXPECT_EQ_EXPR("a*(a+a)", "BOp(Name(a),*,BOp(Name(a),+,Name(a)))");
  EXPECT_EQ_EXPR("a==1 and b!=3", "BOp(BOp(Name(a),==,Int(1)),and,BOp(Name(b),!=,Int(3)))");
}

TEST(prs, var) {
#define EXPECT_EQ_VAR(input,expected) EXPECT_EQ(string(*Parser(input).parse()[0]),expected);
  EXPECT_EQ_VAR("a=1", "Var(a,op=(=),value=Int(1))");
  EXPECT_EQ_VAR("a=x()", "Var(a,op=(=),value=Call(x))");
  EXPECT_EQ_VAR("a=a+1", "Var(a,op=(=),value=BOp(Name(a),+,Int(1)))");
  EXPECT_EQ_VAR("a+=a+=a+=a", "Var(a,op=(+=),value=BOp(Name(a),+=,BOp(Name(a),+=,Name(a))))");
}

TEST(prs, conditionalExpression) {
#define EXPECT_EQ_VAR(input,expected) EXPECT_EQ(string(*Parser(input).expr()),expected);
  EXPECT_EQ_VAR("1 if True else 2", "CondExpr(Int(1) if Bool(true) else Int(2))");
  EXPECT_EQ_VAR("a+b if a==b else a-b", "CondExpr(BOp(Name(a),+,Name(b)) if BOp(Name(a),==,Name(b)) else BOp(Name(a),-,Name(b)))");
}

TEST(prs, fold) {
#define EXPECT_EQ_FOLD(input,expected) EXPECT_EQ(string(*fold(Parser(input).expr())), expected);
  EXPECT_EQ_FOLD("4*4", "Int(16)");
  EXPECT_EQ_FOLD("4+4", "Int(8)");
  EXPECT_EQ_FOLD("4-4", "Int(0)");
  EXPECT_EQ_FOLD("4/4", "Int(1)");
  EXPECT_EQ_FOLD("2+3*a+b", "BOp(BOp(Int(2),+,BOp(Int(3),*,Name(a))),+,Name(b))");
  EXPECT_EQ_FOLD("2+3*(a+b)", "BOp(Int(2),+,BOp(Int(3),*,BOp(Name(a),+,Name(b))))");
}

TEST(prs, ret) {
  EXPECT_EQ(string(*Parser(
    "def f():\n"
    "  return a"
  ).parse()[0]), "Fn(f,args=[],body=[Ret(Name(a))])");
  EXPECT_EQ(string(*Parser(
    "def f():\n"
    "  return 1 * 2 + 3"
  ).parse()[0]), "Fn(f,args=[],body=[Ret(BOp(BOp(Int(1),*,Int(2)),+,Int(3)))])");
  EXPECT_EQ(string(*Parser(
    "def f():\n"
    "  return x()"
  ).parse()[0]), "Fn(f,args=[],body=[Ret(Call(x))])");
}

TEST(prs, if_) {
  EXPECT_EQ(string(*Parser(
    "if x == 1:\n"
    "  return a\n"
  ).parse()[0]), "If(test=BOp(Name(x),==,Int(1)),then=[Ret(Name(a))],else=[])");
  EXPECT_EQ(string(*Parser(
    "if x == 1:\n"
    "  a = 2\n"
    "  return a\n"
    "else:\n"
    "  b = 3\n"
    "  return b\n"
  ).parse()[0]), "If(test=BOp(Name(x),==,Int(1)),then=[Var(a,op=(=),value=Int(2)),Ret(Name(a))],else=[Var(b,op=(=),value=Int(3)),Ret(Name(b))])");
}