#include "prs.h"
#include <algorithm>
using namespace std;

namespace ast {
vec<u_ptr<Node>> Parser::parse() {
  vec<u_ptr<Node>> result;
  auto p = peek();
  size_t indent = peek()->space;
  while (peek() && peek()->space == indent) {
    while (peek() && !peek()->isNewline() && !peek()->isNullChar()) {
      if (peek()->value == "def") {
        result.push_back(fn());
      } else if (peek()->type == TokTy::id) {
        result.push_back(id());
      } else if (peek()->value == "return" && eat(TokTy::punct, "return")) {
        result.push_back(mu<Ret>(expr()));
      } else if (peek()->value == "if") {
        result.push_back(if_());
      }
    }
    eat();
    if (indent != peek()->space) return result;
  }
  return result;
}

u_ptr<Node> Parser::uop() {
  switch (peek()->type) {
  case TokTy::int_literal: return mu<IntLiteral>(stoi(eat()->value));
  case TokTy::bool_literal: return mu<BoolLiteral>(eat()->value == "True");
  case TokTy::punct: {
    if (peek()->value == "if") return nullptr;
    eat(TokTy::punct, "(");
    auto test = expr({ ")" });
    eat(TokTy::punct, ")");
    return test;
  }
  case TokTy::id: {
    string id = eat()->value;
    if (peek() && peek()->value == "(") {
      eat(TokTy::punct, "(");
      eat(TokTy::punct, ")");
      return mu<Call>(id);
    }
    return mu<Name>(id);
  }
  default: break;
  }
}

u_ptr<Node> Parser::id() {
  string id = eat()->value;
  if (peek()->value == "(") {
    eat({ }, "(");
    eat({ }, ")");
    return mu<Call>(id);
  }
  return mu<Var>(id, eat()->value, mv(expr()));
}

u_ptr<Node> Parser::fn() {
  eat(TokTy::punct, "def");
  string id = eat()->value;
  eat(TokTy::punct, "(");
  vec<u_ptr<Arg>> args;
  while (peek()->value != ")") {
    args.push_back(mu<Arg>(eat()->value));
    if (peek()->value == ",")
      eat();
    else break;
  }
  eat(TokTy::punct, ")");
  eat(TokTy::punct, ":");
  eat(TokTy::punct, "n");
  return mu<Fn>(id, mv(args), parse());
}

u_ptr<Node> Parser::condExpr(u_ptr<Node> tValue) {
  auto test = expr({ "else" });
  eat(TokTy::punct, "else");
  return mu<CondExpr>(mv(tValue), expr(), mv(test));
}

u_ptr<Node> Parser::expr(const u_set<string>& terminators) {
  deque<u_ptr<Node>> nodes;
  deque<string> ops;
  nodes.push_back(uop());
  if (peek()->value == "if" && eat({ }, "if"))
    return condExpr(pop(nodes));
  while (peek() && !(peek()->isNewline() || terminators.contains(peek()->value))) {
    if (peek()->value == "if" && eat({ }, "if")) {
      while (ops.size())
        nodes.push_front(mu<BOp>(pop_front(nodes), pop_front(ops), pop_front(nodes)));
      return condExpr(pop(nodes));
    }
    ops.push_back(eat()->value);
    nodes.push_back(uop());
    while (ops.size() && (nextTakesPrec(ops.back()) || nextAssociatesRight()))
      nodes.push_back(mu<BOp>(pop(nodes), eat()->value, uop()));
  }
  while (!ops.empty())
    nodes.push_front(mu<BOp>(pop_front(nodes), pop_front(ops), pop_front(nodes)));
  return mv(nodes.back());
}

u_ptr<Node> Parser::if_() {
  eat({ }, "if");
  auto test = expr({ ":" });
  eat({ }, ":");
  eat(TokTy::punct, "n");
  auto then = parse();
  vec<u_ptr<Node>> else_;
  if (peek()->value == "else") {
    eat({ }, "else");
    eat({ }, ":");
    eat(TokTy::punct, "n");
    else_ = parse();
  }
  return mu<If>(mv(test), mv(then), mv(else_));
}
}; // namespace AST