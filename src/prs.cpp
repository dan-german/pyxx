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
  case TokTy::int_const: return mu<IntConst>(stoi(eat()->value));
  case TokTy::bool_const: return mu<BoolConst>(eat()->value == "True");
  case TokTy::punct: {
    if (peek()->value == "if") return nullptr;
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
      return mu<Call>(id);
    }
    return mu<Name>(id);
  }
  default: break;
  }
}

u_ptr<Node> Parser::call(string& id) {
  eat({ }, "(");
  eat({ }, ")");
  return mu<Call>(id);
}

u_ptr<Node> Parser::id() {
  string id = eat()->value;
  if (peek()->value == "(") {
    return call(id);
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
    else
      break;
  }
  eat(TokTy::punct, ")");
  eat(TokTy::punct, ":");
  auto last = eat();
  auto stmts = parse();
  return mu<Fn>(id, mv(args), mv(stmts));
}

u_ptr<Node> Parser::ret() {
  eat(TokTy::punct, "return");
  return mu<Ret>(expr());
}

// 1 if true else 2
u_ptr<Node> Parser::condExpr(u_ptr<Node>& tValue) {
  auto p = peek();
  auto test = expr({ "else" });
  auto p2 = peek();
  eat(TokTy::punct, "else");
  auto fValue = expr();
  return mu<CondExpr>(mv(tValue), mv(fValue), mv(test));
}

u_ptr<Node> Parser::expr(const u_set<string>& terminators) {
  deque<u_ptr<Node>> nodes;
  nodes.push_back(uop());
  if (peek()->value == "if") {
    eat({ }, "if");
    auto trueVal = mv(nodes.back()); nodes.pop_back();
    return condExpr(trueVal);
  }
  deque<string> ops;
  auto p = peek();
  while (peek() and not (peek()->isNewline() or terminators.contains(peek()->value))) {
    auto nextOp = peek()->value;
    if (nextOp == "if") {
      while (not ops.empty()) {
        auto left = mv(nodes.front()); nodes.pop_front();
        auto right = mv(nodes.front()); nodes.pop_front();
        string op = mv(ops.front()); ops.pop_front();
        nodes.push_front(mu<BOp>(mv(left), op, mv(right)));
      }
      eat({ }, "if");
      auto trueVal = mv(nodes.back()); nodes.pop_back();
      return condExpr(trueVal);
    }
    ops.push_back(eat()->value);
    nodes.push_back(uop());
    while (peek() and (nextTakesPrec(ops.back()) or nextAssociatesRight())) {
      auto left = mv(nodes.back()); nodes.pop_back();
      nodes.push_back(mu<BOp>(mv(left), eat()->value, uop()));
    }
  }
  while (not ops.empty()) {
    auto left = mv(nodes.front()); nodes.pop_front();
    auto right = mv(nodes.front()); nodes.pop_front();
    string op = mv(ops.front()); ops.pop_front();
    nodes.push_front(mu<BOp>(mv(left), op, mv(right)));
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
  return mu<If>(mv(test), mv(then), mv(else_));
}
}; // namespace AST