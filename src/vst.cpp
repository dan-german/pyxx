#include <Vst.h>
#include <vector>

using namespace ast;
using namespace std;

namespace vst {
void postorder(Node* head, std::function<void(ast::Node*)> visitor) {
  vector<pair<Node*, bool>> stack { { head, false } };
  while (not stack.empty()) {
    auto& top = stack.back();
    if (top.second) {
      visitor(top.first);
      stack.pop_back();
    } else {
      top.second = true;
      if (auto bop = dynamic_cast<BOp*>(top.first)) {
        if (bop->left) stack.push_back({ bop->left.get(), false });
        if (bop->right) stack.push_back({ bop->right.get(), false });
      }
    }
  }
}
};