#include <format>
#include <cassert>
#include "ast_models.h"
#include "utils.h"
#include "dbg.h"
#include <cassert>
using namespace std;

namespace ast {
void Node::print() const { dbg::printTree(this); }

u_ptr<Node> fold(u_ptr<Node>&& node) {
  auto bop = dc<BOp>(node);
  if (!bop) return mv(node);

  auto left = fold(mv(bop->left));
  auto right = fold(mv(bop->right));

  if (auto lc = dc<IntConst>(left.get()))
    if (auto rc = dc<IntConst>(right.get())) {
      if (bop->op == "*") {
        return mu<IntConst>(lc->value * rc->value);
      } else if (bop->op == "+") {
        return mu<IntConst>(lc->value + rc->value);
      } else if (bop->op == "-") {
        return mu<IntConst>(lc->value - rc->value);
      } else if (bop->op == "/") {
        return mu<IntConst>(lc->value / rc->value);
      }
    }

  bop->left = mv(left);
  bop->right = mv(right);
  return mv(node);
}

Arg::Arg(string id): id(id) { }

bool Arg::isEqual(const Node& other) const {
  assert(false);
  return false;
}

Arg::operator string() const { return id; }

Fn::Fn(string id, vec<u_ptr<Arg>> args, vec<u_ptr<Node>> statements): id(id), args(mv(args)), body(mv(statements)) {

}

bool Fn::isEqual(const Node& other) const {
  const Fn* cast = dc<const Fn>(&other);
  if (!cast) return false;
  if (cast->id != id) return false;
  for (size_t i = 0; i < args.size(); i++)
    if (*cast->args[i] != *args[i]) return false;
  for (size_t i = 0; i < body.size(); i++)
    if (*cast->body[i] != *body[i]) return false;
  return true;
}

Fn::operator string() const {
  return format("Fn({},args={},body={})", id, dbg::vecToStr(args), dbg::vecToStr(body));
}

Name::Name(string value): id(value) { }

bool Name::isEqual(const Node& other) const {
  const Name* cast = dc<const Name>(&other);
  return cast && cast->id == id;
}

Name::operator string() const {
  return format("Name({})", id);
}

IntConst::IntConst(int value): value(value) { }

bool IntConst::isEqual(const Node& other) const {
  const IntConst* cast = dc<const IntConst>(&other);
  return cast && cast->value == value;
}

IntConst::operator string() const {
  return format("Int({})", value);
}

BoolConst::BoolConst(bool value): value(value) { }

bool BoolConst::isEqual(const Node& other) const {
  assert(false);
}

BoolConst::operator string() const {
  return format("Bool({})", value);
}

UOp::UOp(char op): op(op) { }

bool UOp::isEqual(const Node& other) const {
  const UOp* cast = dc<const UOp>(&other);
  return cast && cast->op == op;
}

UOp::operator string() const {
  return format("");
}

Var::Var(string id, string op, u_ptr<Node> value)
  : id(id), op(op), value(mv(value)) {
}

bool Var::isEqual(const Node& other) const {
  const Var* cast_other = dc<const Var>(&other);
  return cast_other
    && cast_other->id == id
    && *cast_other->value == *value;
}

Var::operator string() const {
  return format("Var({},op=({}),value={})", id, op, string(*value));
}

BOp::BOp(u_ptr<Node> left, string op, u_ptr<Node> right)
  : left(mv(left)), op(op), right(mv(right)) {
}

bool BOp::isEqual(const Node& other) const {
  const BOp* cast = dc<const BOp>(&other);
  return cast
    && *cast->left == *left
    && cast->op == op
    && *cast->right == *right;
}

BOp::operator string() const {
  return format("BOp({},{},{})", string(*left), op, string(*right));
}

Call::Call(string& id): id(mv(id)) {

}

bool Call::isEqual(const Node& other) const {
  const Call* cast = dc<const Call>(&other);
  return cast->id == id;
}

Call::operator string() const {
  return format("Call({})", id);
}

Ret::Ret(u_ptr<Node> retVal): value(mv(retVal)) { }

bool Ret::isEqual(const Node& other) const {
  auto cast = dc<const Ret>(&other);
  return cast and cast->value == value;
}

Ret::operator std::string() const {
  return format("Ret({})", string(*value));
}

If::If(u_ptr<Node> test, vec<u_ptr<Node>> then, vec<u_ptr<Node>> else_): test(mv(test)), then(mv(then)), else_(mv(else_)) { }

bool If::isEqual(const Node& other) const {
  auto cast = dc<const If>(&other);
  return test == cast->test;
}

If::operator std::string() const {
  return format("If(test={},then={},else={})", string(*test), dbg::vecToStr(then), dbg::vecToStr(else_));
}

CondExpr::CondExpr(u_ptr<Node> trueExpr, u_ptr<Node> falseExpr, u_ptr<Node> test): trueExpr(mv(trueExpr)), falseExpr(mv(falseExpr)), test(mv(test)) { }
bool CondExpr::isEqual(const Node& other) const {
  assert(false);

}

CondExpr::operator std::string() const { 
  return format("CondExpr({} if {} else {})", string(*trueExpr), string(*test), string(*falseExpr)); 
}
};