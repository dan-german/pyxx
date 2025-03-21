#pragma once
#include <tok.h>
#include <memory>

namespace AST {

struct Node {
  virtual bool isEqual(const Node &other) const = 0;
  bool operator==(const Node &other) const { return isEqual(other); }
  virtual ~Node() = default;
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
  std::string type;
  Node *value;

  Var(std::string id, std::string type, Node *value = 0): id(id), type(type), value(value) { }

  bool isEqual(const Node &other) const override {
    const Var *cast_other = dynamic_cast<const Var *>(&other);
    return cast_other
      && cast_other->id == id
      && cast_other->type == type
      && *cast_other->value == *value;
  }
};

struct BOp: public Node {
  Node *left;
  char op;
  Node *right;

  BOp(Node *left, char op, Node *right = 0): left(left), op(op), right(right) { }

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
  std::vector<Node *> stack;
  Tokenizer tokenizer;
  std::vector<Node *> result;
public:
  Parser(std::string &code): tokenizer(code) { }
  Parser(std::string code): tokenizer(code) { }
  std::vector<Node *> parse();
};
}; // namespace AST