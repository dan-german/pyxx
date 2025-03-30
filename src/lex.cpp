#include "lex.h"
#include <algorithm>
using namespace std;

const unordered_set<string> PUNCT = { " ", ";", "\n", "def", "(", ")" };
const unordered_set<string> OPS = { "+", "-", "*", "/", "=", "+=", "-=", "*=", "/=" };

Lex::Lex(const string &input): input(input) {
  nextTok = getNextToken();
}

optional<Tok> Lex::getNextToken() {
  if (pos == input.size())
    return { };

  while (pos < input.size()) {
    int space = 0;
    while (input[pos] == ' ') {
      pos++;
      space++;
    }
    string value { };
    TokType type;
    if (isalpha(input[pos])) {
      while (isalnum(input[pos]))
        value += input[pos++];
      type = PUNCT.contains(value) ? TokType::punct : TokType::id;
    } else if (isdigit(input[pos])) {
      while (pos < input.size() && isdigit(input[pos]))
        value += input[pos++];
      type = TokType::int_const;
    } else {
      if (pos + 2 < input.size() && OPS.contains(input.substr(pos, 2))) {
        value = input.substr(pos, 2);
        pos += 2;
      } else {
        value = string(1, input[pos++]);
      }
      type = OPS.contains(value) ? TokType::op : TokType::punct;
      value = value == "\n" ? "n" : value;
    }
    return Tok(type, value, space);
  }
  return { };
}

optional<Tok> Lex::eat(optional<TokType> expectedType, optional<string> expectedValue) {
  currToken = nextTok;
  if (!currToken) return { };
  if (expectedType && currToken->type != *expectedType) {
    throw runtime_error("Expected token of type: " + tokToStr(*expectedType) + ", got " + tokToStr(currToken->type));
  }
  if (expectedValue && currToken->value != *expectedValue) {
    throw runtime_error("Expected token of value: " + *expectedValue + ", got " + currToken->value);
  }
  nextTok = getNextToken();
  print_tok(*currToken, "eating: ");
  return currToken;
}

void print_tok(const Tok &t, string msg) {
  constexpr int PADDING = 15;
  print("{}{:<{}} {} {}\n", msg, tokToStr(t.type), PADDING, t.value, t.space);
}

std::string tokToStr(TokType tp) {
  switch (tp) {
  case TokType::id: return "ID";
  case TokType::op: return "OP";
  case TokType::type: return "TYPE";
  case TokType::int_const: return "CONST (INT)";
  case TokType::punct: return "PUNCT";
  default: return "NOT IMPLEMENTED";
  }
}