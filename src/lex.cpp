#include "lex.h"
#include <algorithm>

using namespace std;

const unordered_set<string> PUNCT = { " ", ";", "\n", "def", "(", ")" };
const unordered_set<string> OPS = { "+", "-", "*", "/", "=" };

optional<Token> Lex::getNextToken() {
  if (pos == input.size())
    return { };

  while (pos < input.size()) {
    if (isalpha(input[pos])) {
      return parseId();
    } else if (isdigit(input[pos])) {
      return parseIntConst();
    } else {
      char c = input[pos++];
      TokenType type = OPS.contains(string(1, c)) ? TokenType::op : TokenType::punct;
      string value = c == '\n' ? "n" : string(1, c);
      return Token { type, value };
    }
    return { };
  }
}

Token Lex::parseIntConst() {
  string value { };
  while (pos < input.size() && isdigit(input[pos]))
    value += input[pos++];
  return { TokenType::int_const, value };
}

Token Lex::parseId() {
  string value { };
  while (pos < input.size() && isalnum(input[pos]))
    value += input[pos++];
  if (PUNCT.contains(value)) return { TokenType::punct, value };
  return { TokenType::id, value };
}

Lex::Lex(const string &input): input(input) {
  nextToken = getNextToken();
}

optional<Token> Lex::eat(optional<TokenType> expectedType, optional<string> expectedValue) {
  currToken = nextToken;
  if (expectedType && currToken->first != expectedType) {
    std::print("Expected token of type: {}, got {}\n", getTokenTypeString(*expectedType), getTokenTypeString(currToken->first));
    throw 1;
  }
  if (expectedValue && currToken->second != expectedValue) {
    std::print("Expected token of value: {}, got {}\n", *expectedValue, currToken->second);
    throw 1;
  }
  nextToken = getNextToken();
  return currToken;
}

void print_tok(const Token &t) {
  constexpr int PADDING = 15;
  print("{:<{}} {}\n", getTokenTypeString(t.first), PADDING, t.second);
}