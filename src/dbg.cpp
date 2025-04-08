#include "vst.h"
#include "utils.h"
#include <format>
#include <print>
#include "dbg.h"
#include "ast_models.h"

using namespace ast;
using namespace std;

namespace dbg {
string getMinimalPresentation(const Node* node) {
  if (auto bop = dc<const BOp>(node)) {
    return format("BOp({})", bop->op);
  } else if (auto var = dc<const Var>(node)) {
    return format("Var({}, op({}))", var->id, var->op);
  } else if (auto fn = dc<const Fn>(node)) {
    return format("Fn {}({})", fn->id, vecToStr(fn->args));
  }
  return string(*node);
}

void printTree(const Node* n) {
  string res = "";
  vst::preorder(n, [&res](const Node* node, int level) {
    res += string(level * 2, ' ') + getMinimalPresentation(node) + '\n';
  });
  print("{}", res);
}
}