#include "lex.h"
#include <algorithm>
#include "utils.h"
using namespace std;

const u_set<string> PUNCT = { " ", ";", "\n", "def", "(", ")", "return", "if", "and", "or", "else" };
const u_set<string> OPS = { "+", "-", "*", "/", "=", "+=", "-=", "*=", "/=", "==", "!=" };

Lex::Lex(const string& input): input(input) {
  nextTok = getNextToken();
}

optional<Tok> Lex::getNextToken() {
  if (pos == input.size())
    return { };

  while (pos < input.size()) {
    // skip comments
    while (input[pos] == '#') {
      while (pos < input.size() && input[pos] != '\n') 
        pos++;
      pos++;
    }

    char c = input[pos];
    int space = 0;
    while (input[pos] == ' ') {
      pos++;
      space++;
    }
    string value { };
    TokTy type;
    if (isalpha(input[pos])) {
      while (isalnum(input[pos]))
        value += input[pos++];

      type = TokTy::id;
      if (PUNCT.contains(value)) {
        type = TokTy::punct;
      } else if (value == "True" || value == "False") {
        type = TokTy::bool_literal;
      }
    } else if (isdigit(input[pos])) {
      while (pos < input.size() && isdigit(input[pos]))
        value += input[pos++];
      type = TokTy::int_literal;
    } else if (input[pos] == '\0') {
      return Tok(TokTy::punct, "null", space);
    } else {
      if (pos + 2 < input.size() && OPS.contains(input.substr(pos, 2))) {
        value = input.substr(pos, 2);
        pos += 2;
      } else {
        value = string(1, input[pos++]);
      }
      type = OPS.contains(value) ? TokTy::op : TokTy::punct;
      value = value == "\n" ? "n" : value;
    }
    return Tok(type, value, space);
  }
  return { };
}

optional<Tok> Lex::eat(optional<TokTy> expectedType, optional<string> expectedValue) {
  currToken = nextTok;
  if (!currToken) return { };
  if (expectedType && currToken->type != *expectedType) {
    throw runtime_error("Expected token of type: " + tokToStr(*expectedType) + ", got " + tokToStr(currToken->type));
  }
  if (expectedValue && currToken->value != *expectedValue) {
    throw runtime_error("Expected token of value: " + *expectedValue + ", got " + currToken->value);
  }
  nextTok = getNextToken();
  return currToken;
}

void print_tok(const Tok& t, string msg) {
  constexpr int PADDING = 15;
  print("{}{:<{}} {} {}\n", msg, tokToStr(t.type), PADDING, t.value, t.space);
}

std::string tokToStr(TokTy tp) {
  switch (tp) {
  case TokTy::id: return "ID";
  case TokTy::op: return "OP";
  case TokTy::type: return "TYPE";
  case TokTy::int_literal: return "CONST (INT)";
  case TokTy::bool_literal: return "CONST (BOOL)";
  case TokTy::punct: return "PUNCT";
  default: return "NOT IMPLEMENTED";
  }
}