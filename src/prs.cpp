#include "prs.h"
#include <print>

namespace AST {
std::vector<Node*> Parser::parse() {
  while (tokenizer.peek()) {
    auto token = *tokenizer.eat();
    if (token.first == TokenType::int_const) {
      if (stack.size()) {
        if (auto bop = dynamic_cast<BOp*>(stack.back())) {
          stack.pop_back();
          bop->right = new IntConst(std::get<int>(token.second));
          if (auto var = dynamic_cast<Var*>(stack.back())) {
            var->value = bop;
            stack.pop_back();
            if (stack.empty()) {
              result.push_back(var);
            }
          }
        }
      }
      stack.push_back(new IntConst(std::get<int>(token.second)));
    } else if (token.first == TokenType::op) { 
      if (stack.size()) {
        if (auto ptr = dynamic_cast<IntConst*>(stack.back())) {
          auto left = stack.back(); stack.pop_back();
          stack.push_back(new BOp(left, std::get<char>(token.second)));
        }
      }
    } else if (token.first == TokenType::type) {
      if (tokenizer.peek() && tokenizer.peek()->first == TokenType::id) {
        std::string id = std::get<std::string>(tokenizer.peek()->second);
        stack.push_back(new Var(id, std::get<std::string>(token.second)));
        tokenizer.eat();
      }
    }
  }
  return result;
}
}; // namespace AST