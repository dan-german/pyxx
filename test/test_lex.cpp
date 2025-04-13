#include <gtest/gtest.h>
#include "lex.h"
#include <vector>
#include <utility>
using namespace std;

inline void expectToken(Lex& lex, int space, string value, TokTy type) {
  EXPECT_EQ(lex.peek(), Tok(type, value, space));
  EXPECT_EQ(lex.eat(), Tok(type, value, space));
  EXPECT_EQ(lex.curr(), Tok(type, value, space));
}

TEST(lex, CurrPeekAndEat) {
  string code =
    "# this is a comment\n"
    "#\n"
    "def f():\n"
    "  a=1*2\n"
    "  b+=1\n"
    "  return True\0";

  Lex lex(code);
  auto empty = optional<Tok>();
  EXPECT_EQ(lex.curr(), empty);

  expectToken(lex, 0, "def", TokTy::punct);
  expectToken(lex, 1, "f", TokTy::id);
  expectToken(lex, 0, "(", TokTy::punct);
  expectToken(lex, 0, ")", TokTy::punct);
  expectToken(lex, 0, ":", TokTy::punct);
  expectToken(lex, 0, "n", TokTy::punct);
  expectToken(lex, 2, "a", TokTy::id);
  expectToken(lex, 0, "=", TokTy::op);
  expectToken(lex, 0, "1", TokTy::int_literal);
  expectToken(lex, 0, "*", TokTy::op);
  expectToken(lex, 0, "2", TokTy::int_literal);
  expectToken(lex, 0, "n", TokTy::punct);
  expectToken(lex, 2, "b", TokTy::id);
  expectToken(lex, 0, "+=", TokTy::op);
  expectToken(lex, 0, "1", TokTy::int_literal);
  expectToken(lex, 0, "n", TokTy::punct);
  expectToken(lex, 2, "return", TokTy::punct);
  expectToken(lex, 1, "True", TokTy::bool_literal);

  auto p = lex.peek();

  EXPECT_EQ(lex.eat(), empty);
  EXPECT_EQ(lex.curr(), empty);
}