#pragma once
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <string_view>
#include <print>
#include <variant>

#define whitespace ' '

enum class TokenType: char {
  id,
  op,
  type,
  int_const,
  punct,
  COUNT
};

constexpr std::string_view getTokenTypeString(TokenType tp) {
  switch (tp) {
  case TokenType::id: return "ID";
  case TokenType::op: return "OP";
  case TokenType::type: return "TYPE";
  case TokenType::int_const: return "CONST (INT)";
  case TokenType::punct: return "ID";
  default: return "NOT IMPLEMENTED";
  }
}

using Token = std::pair<TokenType, std::variant<char, std::string, int>>;

void print_tok(Token &t);

class Tokenizer {
private:
  std::optional<Token> currToken;
  std::optional<Token> nextToken;
  std::string &input;
  size_t pos = 0;
  std::optional<Token> getNextToken();
  Token parseId();
  Token parseIntConst();
public:
  inline bool eatable() { return pos < input.size() && input[pos] != whitespace; }
  Tokenizer(std::string &input);
  std::optional<Token> curr();
  std::optional<Token> eat();
  std::optional<Token> peek();
};