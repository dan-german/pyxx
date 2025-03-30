#include <gtest/gtest.h>
#include "lex.h"
#include <vector>
#include <utility>
using namespace std;

inline void expectToken(Lex &lex, int space, string value, TokType type) {
  EXPECT_EQ(lex.peek(), Tok(type, value, space));
  EXPECT_EQ(lex.eat(), Tok(type, value, space));
  EXPECT_EQ(lex.curr(), Tok(type, value, space));
}

TEST(lex, CurrPeekAndEat) {
  string code =
    "def f():\n"
    "  a=1*2"
    "  b+=1";
  Lex lex(code);
  auto empty = optional<Tok>();
  EXPECT_EQ(lex.curr(), empty);

  expectToken(lex, 0, "def", TokType::punct);
  expectToken(lex, 1, "f", TokType::id);
  expectToken(lex, 0, "(", TokType::punct);
  expectToken(lex, 0, ")", TokType::punct);
  expectToken(lex, 0, ":", TokType::punct);
  expectToken(lex, 0, "n", TokType::punct);
  expectToken(lex, 2, "a", TokType::id);
  expectToken(lex, 0, "=", TokType::op);
  expectToken(lex, 0, "1", TokType::int_const);
  expectToken(lex, 0, "*", TokType::op);
  expectToken(lex, 0, "2", TokType::int_const);
  expectToken(lex, 2, "b", TokType::id);
  expectToken(lex, 0, "+=", TokType::op);
  expectToken(lex, 0, "1", TokType::int_const);

  EXPECT_EQ(lex.peek(), empty);
  EXPECT_EQ(lex.eat(), empty);
  EXPECT_EQ(lex.curr(), empty);
}