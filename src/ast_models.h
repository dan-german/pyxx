#pragma once
#include <memory>
#include <string>
#include <vector>
#include <print>
#include "utils.h"

namespace ast {

struct Node {
  virtual ~Node() {}
  virtual bool isEqual(const Node& other) const = 0;
  bool operator==(const Node& other) const { return isEqual(other); }
  virtual operator std::string() const = 0;
  void print() const;
};

u_ptr<Node> fold(u_ptr<Node>&& node);

struct Arg: Node {
  std::string id;
  bool isEqual(const Node& other) const override;
  operator std::string() const override;
  Arg(std::string id);
};

struct Fn: Node {
  std::string id;
  vec<u_ptr<Arg>> args;
  vec<u_ptr<Node>> body;
  Fn(std::string id, vec<u_ptr<Arg>> args, vec<u_ptr<Node>> statements = { });
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
  u_ptr<Node> value;
  Var(std::string id, std::string op, u_ptr<Node> value = { });
  bool isEqual(const Node& other) const override;
  operator std::string() const override;
};

struct BOp: public Node {
  u_ptr<Node> left;
  std::string op;
  u_ptr<Node> right;
  BOp(u_ptr<Node> left, std::string op, u_ptr<Node> right = { });
  bool isEqual(const Node& other) const override;
  operator std::string() const override;
};

struct Call: public Node {
  std::string id;
  Call(std::string& id);
  bool isEqual(const Node& other) const override;
  operator std::string() const override;
};

struct Ret: Node {
  u_ptr<Node> value;
  Ret(u_ptr<Node> retVal);
  bool isEqual(const Node& other) const override;
  operator std::string() const override;
};

struct If: Node {
  u_ptr<Node> test;
  vec<u_ptr<Node>> then;
  vec<u_ptr<Node>> else_;
  If(u_ptr<Node> test, vec<u_ptr<Node>> then, vec<u_ptr<Node>> else_);
  bool isEqual(const Node& other) const override;
  operator std::string() const override;
};
};