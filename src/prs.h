#pragma once
#include "lex.h"
#include <memory>

namespace AST {

struct Node {
  virtual bool isEqual(const Node &other) const = 0;
  bool operator==(const Node &other) const { return isEqual(other); }
  virtual ~Node() = default;
};

struct Arg: Node {

};

struct Fn: Node {
  std::string id;
  std::vector<Node *> args;
  Fn(std::string id, std::vector<Node *> args = { }): id(id), args(args) { }
  bool isEqual(const Node &other) const override {
    const Fn *cast = dynamic_cast<const Fn *>(&other);
    if (!cast) return false;
    if (cast->id != id) return false;
    for (size_t i = 0; i < args.size(); i++) {
      if (*cast->args[i] != *args[i]) return false;
    }
    return true;
  }
};

struct IntConst: public Node {
  int value;
  IntConst(int value): value(value) { }
  bool isEqual(const Node &other) const override {
    const IntConst *cast = dynamic_cast<const IntConst *>(&other);
    return cast && cast->value == value;
  }
};

struct UOp: public Node {
  char op;
  UOp(char op): op(op) { }

  bool isEqual(const Node &other) const override {
    const UOp *cast = dynamic_cast<const UOp *>(&other);
    return cast && cast->op == op;
  }
};

struct Var: public Node {
  std::string id;
  std::string op;
  Node *value;

  Var(std::string id, Node *value = 0): id(id), value(value) { }

  bool isEqual(const Node &other) const override {
    const Var *cast_other = dynamic_cast<const Var *>(&other);
    return cast_other
      && cast_other->id == id
      && *cast_other->value == *value;
  }
};

struct BOp: public Node {
  Node *left;
  std::string op;
  Node *right;

  BOp(Node *left, std::string op, Node *right = 0): left(left), op(op), right(right) { }

  bool isEqual(const Node &other) const override {
    const BOp *cast = dynamic_cast<const BOp *>(&other);
    return cast
      && *cast->left == *left
      && cast->op == op
      && *cast->right == *right;
  }
};

class Parser {
private:
  int statement_index = 0;
  std::vector<Node *> stack;
  Lex lexer;
  std::vector<Node *> result;
  void handleInt(const Token &token);
  void handleId(const Token &token);
  void handlePunct(const Token &token);
  void handleOp(const Token &token);
  void stamp();
  void skipSpace();
  void parseBody();
public:
  Parser(const std::string &code): lexer(code) { }
  std::vector<Node *> parse();
};
}; // namespace AST