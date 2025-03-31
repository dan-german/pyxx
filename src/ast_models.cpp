#include "ast_models.h"
#include <format>
using namespace std;

namespace AST {
Fn::Fn(string id, vector<unique_ptr<Node>> args,
  vector<unique_ptr<Node>> statements)
  : id(id), args(std::move(args)), body(std::move(statements)) {
}

bool Fn::isEqual(const Node& other) const {
  const Fn* cast = dynamic_cast<const Fn*>(&other);
  if (!cast) return false;
  if (cast->id != id) return false;
  for (size_t i = 0; i < args.size(); i++)
    if (*cast->args[i] != *args[i]) return false;
  for (size_t i = 0; i < body.size(); i++)
    if (*cast->body[i] != *body[i]) return false;
  return true;
}

Fn::operator string() const {
  string argsStr = "";
  for (auto& arg : args) argsStr += string(*arg) + ',';
  if (argsStr.size()) argsStr.pop_back();
  string bodyStr = "";
  for (auto& stmt : body) bodyStr += string(*stmt) + ',';
  if (bodyStr.size()) bodyStr.pop_back();
  return format("Fn({},args={},body={})", id, argsStr, bodyStr);
}

Name::Name(string value): id(value) { }

bool Name::isEqual(const Node& other) const {
  const Name* cast = dynamic_cast<const Name*>(&other);
  return cast && cast->id == id;
}

Name::operator string() const {
  return format("Name({})", id);
}

IntConst::IntConst(int value): value(value) { }

bool IntConst::isEqual(const Node& other) const {
  const IntConst* cast = dynamic_cast<const IntConst*>(&other);
  return cast && cast->value == value;
}

IntConst::operator string() const {
  return format("Int({})", value);
}

UOp::UOp(char op): op(op) { }

bool UOp::isEqual(const Node& other) const {
  const UOp* cast = dynamic_cast<const UOp*>(&other);
  return cast && cast->op == op;
}

UOp::operator string() const {
  return format("");
}

Var::Var(string id, string op, unique_ptr<Node> value)
  : id(id), op(op), value(std::move(value)) {
}

bool Var::isEqual(const Node& other) const {
  const Var* cast_other = dynamic_cast<const Var*>(&other);
  return cast_other
    && cast_other->id == id
    && *cast_other->value == *value;
}

Var::operator string() const {
  return format("Var({},op={},value={})", id, op, string(*value));
}

BOp::BOp(unique_ptr<Node> left, string op, unique_ptr<Node> right)
  : left(std::move(left)), op(op), right(std::move(right)) {
}

bool BOp::isEqual(const Node& other) const {
  const BOp* cast = dynamic_cast<const BOp*>(&other);
  return cast
    && *cast->left == *left
    && cast->op == op
    && *cast->right == *right;
}

BOp::operator string() const {
  return format("BOp({},{},{})", string(*left), op, string(*right));
}

Call::Call(string& id): id(std::move(id)) {

}

bool Call::isEqual(const Node& other) const {
  const Call* cast = dynamic_cast<const Call*>(&other);
  return cast->id == id;
}

Call::operator string() const {
  return format("Call({})", id);
}

Ret::Ret(unique_ptr<Node> retVal): retVal(std::move(retVal)) { }

bool Ret::isEqual(const Node& other) const {
  auto cast = dynamic_cast<const Ret*>(&other);
  return cast and cast->retVal == retVal;
}

Ret::operator std::string() const {
  return format("Ret({})", string(*retVal));
}
}