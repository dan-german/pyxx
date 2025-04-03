#include "ast_models.h"
#include <format>
#include "utils.h"
using namespace std;

namespace ast {
string vecToStr(const vec<up<Node>>& v) {
  string str = "[";
  for (auto& arg : v) str += string(*arg) + ',';
  if (v.size()) str.pop_back();
  return str + "]";
}

Fn::Fn(string id, vec<up<Node>> args,
  vec<up<Node>> statements)
  : id(id), args(mv(args)), body(mv(statements)) {
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
  return format("Fn({},args={},body={})", id, vecToStr(args), vecToStr(body));
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

UOp::UOp(char op): op(op) { }

bool UOp::isEqual(const Node& other) const {
  const UOp* cast = dc<const UOp>(&other);
  return cast && cast->op == op;
}

UOp::operator string() const {
  return format("");
}

Var::Var(string id, string op, up<Node> value)
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

BOp::BOp(up<Node> left, string op, up<Node> right)
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

Ret::Ret(up<Node> retVal): value(mv(retVal)) { }

bool Ret::isEqual(const Node& other) const {
  auto cast = dc<const Ret>(&other);
  return cast and cast->value == value;
}

Ret::operator std::string() const {
  return format("Ret({})", string(*value));
}

If::If(up<Node> test, vec<up<Node>> then, vec<up<Node>> else_): test(mv(test)), then(mv(then)), else_(mv(else_)) { }

bool If::isEqual(const Node& other) const {
  auto cast = dc<const If>(&other);
  if (test != cast->test) return false;
}

If::operator std::string() const {
  return format("If(test={},then={},else={})", string(*test), vecToStr(then), vecToStr(else_));
}
}