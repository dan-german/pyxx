#include <gtest/gtest.h>
#include "tok.cpp"

TEST(tok, CurrPeekAndEat) {
  std::string code = "int a = 2;";
  Tokenizer tokenizer(code);
  EXPECT_EQ(tokenizer.curr(), std::optional<Token>({}));
  EXPECT_EQ(tokenizer.peek(), std::make_pair(TokenType::type, std::string("int")));
  EXPECT_EQ(tokenizer.eat(), std::make_pair(TokenType::type, std::string("int")));
  EXPECT_EQ(tokenizer.eat(), std::make_pair(TokenType::id, std::string("a")));
  EXPECT_EQ(tokenizer.peek(), std::make_pair(TokenType::punctuation, std::string("=")));
  EXPECT_EQ(tokenizer.eat(), std::make_pair(TokenType::punctuation, std::string("=")));
  EXPECT_EQ(tokenizer.eat(), std::make_pair(TokenType::int_literal, std::string("2")));
  EXPECT_EQ(tokenizer.eat(), std::make_pair(TokenType::punctuation, std::string(";")));
}