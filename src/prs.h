#pragma once
#include "lex.h"
#include <memory>
#include "ast_models.h"

namespace AST {

const std::unordered_map<std::string, int> PREC = {
  { "+", 0 }, { "-", 0 },
  { "*", 1 }, { "/", 1 }
};

const std::unordered_set<std::string> ASSIGN_OPS { "=", "+=", "-=" };

class Parser {
private:
  Lex lex;
  inline std::optional<Tok> eat(std::optional<TokTy> expected_type = { }, std::optional<std::string> expected_value = { }) { return lex.eat(expected_type, expected_value); };
  inline const std::optional<Tok>& peek() const { return lex.peek(); }
  inline bool nextTakesPrec(std::string& op) const { return peek() && PREC.contains(peek()->value) && PREC.at(peek()->value) > PREC.at(op); }
  inline bool nextAssociatesRight() const { return peek() && ASSIGN_OPS.contains(peek()->value); }
  std::unique_ptr<Node> ret();
  std::unique_ptr<Node> fn();
  std::unique_ptr<Node> id();
  std::unique_ptr<Node> uop();
  std::unique_ptr<Node> call(std::string& id);
public:
  std::unique_ptr<Node> expr(const std::unordered_set<std::string>& stopValues = {});
  std::vector<std::unique_ptr<Node>> parse();
  Parser(const std::string& code): lex(code) { }
};
}; // namespace AST