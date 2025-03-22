#include <gtest/gtest.h>
#include "lex.h"
#include <vector>
#include <utility>

#define EXPECT_TOKEN(type,value) EXPECT_EQ(lex.peek(), Token(type,value));\
  EXPECT_EQ(lex.eat(), Token(type,value));\
  EXPECT_EQ(lex.curr(), Token(type,value));

TEST(tok, CurrPeekAndEat) {
  std::string code = "a=2*2\nb=80+60";
  Lex lex(code);
  EXPECT_EQ(lex.curr(), std::optional<Token>());

  EXPECT_TOKEN(TokenType::id, std::string("a"));
  EXPECT_TOKEN(TokenType::op, std::string("="));
  EXPECT_TOKEN(TokenType::int_const, std::string("2"));
  EXPECT_TOKEN(TokenType::op, std::string("*"));
  EXPECT_TOKEN(TokenType::int_const, std::string("2"));

  EXPECT_TOKEN(TokenType::punct, std::string("n"));

  EXPECT_TOKEN(TokenType::id, std::string("b"));
  EXPECT_TOKEN(TokenType::op, std::string("="));
  EXPECT_TOKEN(TokenType::int_const, std::string("80"));
  EXPECT_TOKEN(TokenType::op, std::string("+"));
  EXPECT_TOKEN(TokenType::int_const, std::string("60"));

  EXPECT_EQ(lex.peek(), std::optional<Token>());
  EXPECT_EQ(lex.eat(), std::optional<Token>());
  EXPECT_EQ(lex.curr(), std::optional<Token>());
}

// TEST(tok, Indentation) {
//   std::string code = "";
//   lex lex(code);
//   EXPECT_EQ(lex.curr(), std::optional<Token>());

//   EXPECT_TOKEN(TokenType::id, std::string("a"));
//   EXPECT_TOKEN(TokenType::op, char('='));
//   EXPECT_TOKEN(TokenType::int_const, 2);
//   EXPECT_TOKEN(TokenType::punct, char('n'));
//   EXPECT_TOKEN(TokenType::id, std::string("b"));
//   EXPECT_TOKEN(TokenType::op, char('='));
//   EXPECT_TOKEN(TokenType::int_const, 80);

//   EXPECT_EQ(lex.peek(), std::optional<Token>());
//   EXPECT_EQ(lex.eat(), std::optional<Token>());
//   EXPECT_EQ(lex.curr(), std::optional<Token>());
// }