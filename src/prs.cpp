#include "prs.h"
#include <print>
#include <algorithm>
using namespace std;

namespace AST {
vector<unique_ptr<Node>> Parser::parse() {
  vector<unique_ptr<Node>> result;
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
      }
    }
    eat();
    int new_indent = peek()->space;
    if (indent != new_indent) return result;
  }
  return result;
}

unique_ptr<Node> Parser::uop() {
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
      // TODO: args
      eat(TokTy::punct, ")");
      return make_unique<Call>(id);
    }
    return make_unique<Name>(id);
  }
  default: break;
  }
}

unique_ptr<Node> Parser::call(string& id) {
  eat({ }, "(");
  eat({ }, ")");
  return make_unique<Call>(id);
}

unique_ptr<Node> Parser::id() {
  string id = eat()->value;
  if (peek()->value == "(") {
    return call(id);
  }
  return make_unique<Var>(id, eat()->value, std::move(expr()));
}

unique_ptr<Node> Parser::fn() {
  eat(TokTy::punct, "def");
  string id = eat()->value;
  eat(TokTy::punct, "(");
  // TODO: args
  eat(TokTy::punct, ")");
  eat(TokTy::punct, ":");
  auto last = eat();
  auto stmts = parse();
  return make_unique<Fn>(id, vector<unique_ptr<Node>>(), std::move(stmts));
}

unique_ptr<Node> Parser::ret() {
  eat(TokTy::punct, "return");
  return make_unique<Ret>(expr());
}

unique_ptr<Node> Parser::expr(const unordered_set<string>& terminators) {
  deque<unique_ptr<Node>> nodes; 
  nodes.push_back(uop());
  deque<string> ops;
  while (peek() and not (peek()->isNewline() or terminators.contains(peek()->value))) { 
    ops.push_back(eat()->value);
    nodes.push_back(uop());
    while (peek() and (nextTakesPrec(ops.back()) or nextAssociatesRight())) { 
      auto left = std::move(nodes.back()); nodes.pop_back();
      nodes.push_back(make_unique<BOp>(std::move(left), eat()->value, uop()));
    }
  }
  while (not ops.empty()) { 
    auto left = std::move(nodes.front()); nodes.pop_front();
    auto right = std::move(nodes.front()); nodes.pop_front();
    string op = std::move(ops.front()); ops.pop_front();
    nodes.push_front(make_unique<BOp>(std::move(left), op, std::move(right)));
  }
  return std::move(nodes.back());
}
}; // namespace AST