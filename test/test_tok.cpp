#include <gtest/gtest.h>
#include "tok.h"
#include <vector>

TEST(tok, CurrPeekAndEat) {
  std::string code = "int a = 2;";
  Tokenizer tokenizer(code);
  EXPECT_EQ(tokenizer.curr(), std::optional<Token>());
  EXPECT_EQ(tokenizer.peek(), Token(TokenType::type, "int"));
  EXPECT_EQ(tokenizer.eat(), Token(TokenType::type, "int"));
  EXPECT_EQ(tokenizer.eat(), Token(TokenType::id, "a"));

  EXPECT_EQ(tokenizer.peek(), Token(TokenType::op, '='));
  EXPECT_EQ(tokenizer.eat(), Token(TokenType::op, '='));
  EXPECT_EQ(tokenizer.curr(), Token(TokenType::op, '='));

  EXPECT_EQ(tokenizer.eat(), Token(TokenType::int_const, 2));
  EXPECT_EQ(tokenizer.eat(), Token(TokenType::punct, ';'));
  EXPECT_EQ(tokenizer.eat(), std::optional<Token>());
  EXPECT_EQ(tokenizer.curr(), std::optional<Token>());
}

TEST(tok, Arithmetic) { 
  std::string code = "int a=1*2+3;";
  std::vector<std::optional<Token>> tokens;
  Tokenizer tokenizer(code);
  while (tokenizer.peek()) tokens.push_back(tokenizer.eat());
  
  std::vector<std::optional<Token>> expected = { 
    Token(TokenType::type, "int"),
    Token(TokenType::id, "a"),
    Token(TokenType::op, '='),
    Token(TokenType::int_const, 1),
    Token(TokenType::op, '*'),
    Token(TokenType::int_const, 2),
    Token(TokenType::op, '+'),
    Token(TokenType::int_const, 3),
    Token(TokenType::punct, ';')
  };
  EXPECT_EQ(expected, tokens);
}