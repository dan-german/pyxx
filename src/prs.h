#pragma once
#include "lex.h"
#include <memory>
#include "ast_models.h"
#include "utils.h"


namespace ast {

class Parser {
private:
  // https://en.cppreference.com/w/cpp/language/operator_precedence
  // Lower value means higher precedence
  const std::unordered_map<std::string, int> PREC = {
    { "*", 5 }, { "/", 5 },
    { "+", 6 }, { "-", 6 },
    { "!=", 10 }, { "==", 10 },
    { "or", 15 },
    { "and", 14 },
  };

  const std::unordered_set<std::string> ASSIGN_OPS { "=", "+=", "-=" };
  Lex lex;
  inline std::optional<Tok> eat(std::optional<TokTy> expected_type = { }, std::optional<std::string> expected_value = { }) {
    return lex.eat(expected_type, expected_value);
  };
  inline const std::optional<Tok>& peek() const { return lex.peek(); }
  inline bool nextTakesPrec(std::string& op) const { return peek() && PREC.contains(peek()->value) && PREC.at(peek()->value) < PREC.at(op); }
  inline bool nextAssociatesRight() const { return peek() && ASSIGN_OPS.contains(peek()->value); }
  u_ptr<Node> if_();
  u_ptr<Node> fn();
  u_ptr<Node> id();
  u_ptr<Node> uop();
  u_ptr<Node> condExpr(u_ptr<Node> tValue);
public:
  u_ptr<Node> expr(const std::unordered_set<std::string>& terminators = { });
  std::vector<u_ptr<Node>> parse();
  Parser(const std::string& code): lex(code) { }
};
}; // namespace AST