#pragma once
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <string_view>
#include <print>

#define whitespace ' '

enum class TokType: char {
  id,
  op,
  type,
  int_const,
  punct,
  COUNT
};

std::string tokToStr(TokType tp);

struct Tok {
  TokType type;
  std::string value;
  int space;
  bool operator==(const Tok &other) const = default;
  inline bool isNewline() const { return type == TokType::punct && value == "n"; }
  Tok(TokType type, std::string value, int space): type(type), value(value), space(space) {}
};

void print_tok(const Tok &t, std::string msg = "");

class Lex {
private:
  std::optional<Tok> currToken;
  std::optional<Tok> nextTok;
  const std::string &input;
  size_t pos = 0;
  std::optional<Tok> getNextToken();
public:
  Lex(const std::string &input);
  std::optional<Tok> eat(std::optional<TokType> expected_type = { }, std::optional<std::string> expected_value = { });
  inline const std::optional<Tok> &curr() const { return currToken; }
  inline const std::optional<Tok> &peek() const { return nextTok; }
};