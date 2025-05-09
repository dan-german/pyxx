#pragma once
namespace ast { struct Node; }
#include <functional>

namespace vst {
void postorder(const ast::Node* head, std::function<void(const ast::Node*)> visitor);
void preorder(const ast::Node* head, std::function<void(const ast::Node*, int)> visitor);
};
