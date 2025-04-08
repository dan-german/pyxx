#include "prs.h"
#include <print>
#include <algorithm>
using namespace std;

namespace ast {
vec<u_ptr<Node>> Parser::parse() {
  vec<u_ptr<Node>> result;
  auto p = peek();
  size_t indent = peek()->space;
  while (peek() && peek()->space == indent) {
    while (peek() && !peek()->isNewline()) {
      if (peek()->value == "def") {
        result.push_back(fn());
      } else if (peek()->type == TokTy::id) {
        result.push_back(id());
      } else if (peek()->value == "return") {
        result.push_back(ret());
      } else if (peek()->value == "if") {
        result.push_back(if_());
      }
    }
    eat();
    int new_indent = peek()->space;
    if (indent != new_indent) return result;
  }
  return result;
}

u_ptr<Node> Parser::uop() {
  auto p = peek();
  switch (peek()->type) {
  case TokTy::int_const: return make_unique<IntConst>(stoi(eat()->value));
  case TokTy::punct: {
    eat(TokTy::punct, "(");
    auto e = expr({ ")" });
    eat(TokTy::punct, ")");
    return e;
  }
  case TokTy::id: {
    string id = eat()->value;
    if (peek() && peek()->value == "(") {
      eat(TokTy::punct, "(");
      eat(TokTy::punct, ")");
      return make_unique<Call>(id);
    }
    return make_unique<Name>(id);
  }
  default: break;
  }
}

u_ptr<Node> Parser::call(string& id) {
  eat({ }, "(");
  eat({ }, ")");
  return make_unique<Call>(id);
}

u_ptr<Node> Parser::id() {
  string id = eat()->value;
  if (peek()->value == "(") {
    return call(id);
  }
  return make_unique<Var>(id, eat()->value, mv(expr()));
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
    else
      break;
  }
  eat(TokTy::punct, ")");
  eat(TokTy::punct, ":");
  auto last = eat();
  auto stmts = parse();
  return make_unique<Fn>(id, mv(args), mv(stmts));
}

u_ptr<Node> Parser::ret() {
  eat(TokTy::punct, "return");
  return make_unique<Ret>(expr());
}

u_ptr<Node> Parser::expr(const u_set<string>& terminators) {
  deque<u_ptr<Node>> nodes;
  nodes.push_back(uop());
  deque<string> ops;
  while (peek() and not (peek()->isNewline() or terminators.contains(peek()->value))) {
    ops.push_back(eat()->value);
    nodes.push_back(uop());
    while (peek() and (nextTakesPrec(ops.back()) or nextAssociatesRight())) {
      auto left = mv(nodes.back()); nodes.pop_back();
      nodes.push_back(make_unique<BOp>(mv(left), eat()->value, uop()));
    }
  }
  while (not ops.empty()) {
    auto left = mv(nodes.front()); nodes.pop_front();
    auto right = mv(nodes.front()); nodes.pop_front();
    string op = mv(ops.front()); ops.pop_front();
    nodes.push_front(make_unique<BOp>(mv(left), op, mv(right)));
  }
  return mv(nodes.back());
}

u_ptr<Node> Parser::if_() {
  eat({ }, "if");
  auto test = expr({ ":" });
  eat({ }, ":");
  eat(TokTy::punct, "n");
  print("\n");
  auto then = parse();
  vec<u_ptr<Node>> else_;
  if (peek()->value == "else") {
    eat({ }, "else");
    eat({ }, ":");
    eat(TokTy::punct, "n");
    else_ = parse();
  }
  return make_unique<If>(mv(test), mv(then), mv(else_));
}
}; // namespace AST