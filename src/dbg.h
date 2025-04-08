#pragma once
namespace ast { struct Node; }

namespace dbg {
void printTree(const ast::Node* n);

template<typename T>
std::string vecToStr(const vec<u_ptr<T>>& v) {
  std::string str = "[";
  for (auto& arg : v) str += std::string(*arg) + ',';
  if (v.size()) str.pop_back();
  return str + "]";
}
}