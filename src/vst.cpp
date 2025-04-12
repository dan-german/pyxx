#include <vst.h>
#include <vector>
#include <utility>
#include "utils.h"
#include "ast_models.h"

using namespace ast;
using namespace std;

namespace {
vec<const Node*> getChildren(const Node* parent) {
  vec<const Node*> children;
  if (auto bop = dc<const BOp>(parent)) {
    if (bop->right) children.push_back(bop->right.get());
    if (bop->left) children.push_back(bop->left.get());
  } else if (auto var = dc<const Var>(parent)) {
    children.push_back(var->value.get());
  } else if (auto ret = dc<const Ret>(parent)) {
    children.push_back(ret->value.get());
  } else if (auto fn = dc<const Fn>(parent)) {
    for (auto& stmt : fn->body)
      children.push_back(stmt.get());
    reverse(begin(children), end(children));
  }
  return children;
}
}

namespace vst {
void postorder(const Node* head, function<void(const Node*)> visitor) {
  vec<pair<const Node*, bool>> stack { { head, false } };
  while (stack.size()) {
    auto& top = stack.back();
    bool alreadyProcessed = top.second;
    if (alreadyProcessed) {
      visitor(top.first);
      stack.pop_back();
    } else {
      top.second = true;
      for (auto child : getChildren(top.first)) stack.push_back({ child, false });
    }
  }
}

void preorder(const Node* head, function<void(const Node*, int)> visitor) {
  vec<pair<const Node*, int>> stack { { head, 0 } };
  while (stack.size()) {
    auto top = pop(stack);
    visitor(top.first, top.second);
    for (auto child : getChildren(top.first)) stack.push_back({ child, top.second + 1 });
  }
}
}