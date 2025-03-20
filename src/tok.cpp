#include "tok.h"
#include <algorithm>

const std::unordered_set<std::string> types = { "int" };
const std::unordered_set<char> punctuations = { ';' };
const std::unordered_set<char> unary = { '+', '-', '*', '/', '=' };

std::optional<Token> Tokenizer::getNextToken() {
  if (pos == input.size())
    return { };
  while (pos < input.size()) {
    // skip whitespace
    while (input[pos] == whitespace) pos++;
    if (isalpha(input[pos])) {
      return parseId();
    } else if (isdigit(input[pos])) {
      return parseIntConst();
    } else {
      TokenType type;
      if (unary.contains(input[pos])) {
        type = TokenType::op;
      } else if (punctuations.contains(input[pos])) {
        type = TokenType::punct;
      }
      return Token { type, input[pos++] };
    }
  }
  return { };
}

Token Tokenizer::parseIntConst() {
  std::string value { };
  while (pos < input.size() && isdigit(input[pos]))
    value += input[pos++];
  return { TokenType::int_const, std::stoi(value) };
}

Token Tokenizer::parseId() {
  std::string id { };
  while (pos < input.size() && isalnum(input[pos]))
    id.push_back(input[pos++]);
  TokenType type = types.contains(id) ? TokenType::type : TokenType::id;
  return { type, id };
}

Tokenizer::Tokenizer(std::string &input): input(input) {
  nextToken = getNextToken();
}

std::optional<Token> Tokenizer::curr() {
  return currToken;
}

std::optional<Token> Tokenizer::eat() {
  currToken = nextToken;
  nextToken = getNextToken();
  return currToken;
}

std::optional<Token> Tokenizer::peek() {
  return nextToken;
}

void print_tok(Token &t) {
  constexpr int PADDING = 15;
  if (std::holds_alternative<std::string>(t.second)) {
    std::print("{:<{}} {}\n", getTokenTypeString(t.first), PADDING, std::get<std::string>(t.second));
  } else if (std::holds_alternative<char>(t.second)) {
    std::print("{:<{}} {}\n", getTokenTypeString(t.first), PADDING, std::get<char>(t.second));
  } else {
    std::print("{:<{}} {}\n", getTokenTypeString(t.first), PADDING, std::get<int>(t.second));
  }
}