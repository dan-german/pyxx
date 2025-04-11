#pragma once
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <string_view>
#include <print>

#define whitespace ' '

enum class TokTy: char {
  id,
  op,
  type,
  int_const,
  bool_const,
  punct,
  COUNT
};

std::string tokToStr(TokTy tp);

struct Tok {
  TokTy type;
  std::string value;
  int space;
  bool operator==(const Tok &other) const = default;
  inline bool isNewline() const { return type == TokTy::punct && value == "n"; }
  Tok(TokTy type, std::string value, int space): type(type), value(value), space(space) {}
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
  std::optional<Tok> eat(std::optional<TokTy> expected_type = { }, std::optional<std::string> expected_value = { });
  inline const std::optional<Tok> &curr() const { return currToken; }
  inline const std::optional<Tok> &peek() const { return nextTok; }
};