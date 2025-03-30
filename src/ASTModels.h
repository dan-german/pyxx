#pragma once
#include <memory>
#include <string>
#include <vector>

namespace AST {
struct Node {
  virtual bool isEqual(const Node& other) const = 0;
  bool operator==(const Node& other) const { return isEqual(other); }
  virtual ~Node() = default;
  virtual operator std::string() const = 0;
};

struct Arg: Node { };

struct Fn: Node {
  std::string id;
  std::vector<std::unique_ptr<Node>> args;
  std::vector<std::unique_ptr<Node>> body;
  Fn(std::string id, std::vector<std::unique_ptr<Node>> args = { },
    std::vector<std::unique_ptr<Node>> statements = { });
  bool isEqual(const Node& other) const override;
  operator std::string() const override;
};

struct Name: public Node {
  std::string id;
  Name(Name&&) = default;
  Name& operator=(Name&&) = default;
  Name(std::string value);
  bool isEqual(const Node& other) const override;
  operator std::string() const override;
};

struct IntConst: public Node {
  int value;
  IntConst(int value);
  bool isEqual(const Node& other) const override;
  operator std::string() const override;
};

struct UOp: public Node {
  char op;
  UOp(char op);
  bool isEqual(const Node& other) const override;
  operator std::string() const override;
};

struct Var: public Node {
  std::string id;
  std::string op;
  std::unique_ptr<Node> value;
  Var(std::string id, std::string op, std::unique_ptr<Node> value = { });
  bool isEqual(const Node& other) const override;
  operator std::string() const override;
};

struct BOp: public Node {
  std::unique_ptr<Node> left;
  std::string op;
  std::unique_ptr<Node> right;
  BOp(std::unique_ptr<Node> left, std::string op,
    std::unique_ptr<Node> right = { });
  bool isEqual(const Node& other) const override;
  operator std::string() const override;
};

struct Call: public Node { 
  std::string id;
  Call(std::string& id);
  bool isEqual(const Node& other) const override;
  operator std::string() const override;
};

}