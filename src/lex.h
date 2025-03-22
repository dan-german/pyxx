#pragma once
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <string_view>
#include <print>

#define whitespace ' '

enum class TokenType: char {
  id,
  op,
  type,
  int_const,
  punct,
  COUNT
};

constexpr std::string getTokenTypeString(TokenType tp) {
  switch (tp) {
  case TokenType::id: return "ID";
  case TokenType::op: return "OP";
  case TokenType::type: return "TYPE";
  case TokenType::int_const: return "CONST (INT)";
  case TokenType::punct: return "PUNCT";
  default: return "NOT IMPLEMENTED";
  }
}

using Token = std::pair<TokenType, std::string>;

void print_tok(const Token &t);

class Lex {
private:
  std::optional<Token> currToken;
  std::optional<Token> nextToken;
  const std::string &input;
  size_t pos = 0;
  std::optional<Token> getNextToken();
  Token parseId();
  Token parseIntConst();
public:
  inline bool eatable() { return pos < input.size() && input[pos] != whitespace; }
  Lex(const std::string &input);
  std::optional<Token> eat(std::optional<TokenType> expected_type = { }, std::optional<std::string> expected_value = { });
  inline const std::optional<Token> &curr() { return currToken; }
  inline const std::optional<Token> &peek() { return nextToken; }
};