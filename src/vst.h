#pragma once
#include "ast_models.h"
#include <functional>
namespace vst {
void postorder(ast::Node* head, std::function<void(ast::Node*)> visitor);
};
