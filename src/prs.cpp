#include "prs.h"
#include <print>

const std::unordered_map<char, int> prec_map = {
  { '+', 0 }, { '-', 0 },
  { '*', 1 }, { '/', 1 },
};

namespace AST {
std::vector<Node *> Parser::parse() {
  while (tokenizer.peek()) {
    auto token = *tokenizer.eat();
    if (token.first == TokenType::int_const) {
      if (stack.size()) {
        if (auto bop = dynamic_cast<BOp *>(stack.back())) {
          stack.pop_back();
          bop->right = new IntConst(std::get<int>(token.second));
          if (auto var = dynamic_cast<Var *>(bop)) {
            var->value = bop;
            stack.pop_back();
            if (stack.empty()) {
              result.push_back(var);
            }
          } else {
            stack.push_back(bop);
          }
        } else if (auto var = dynamic_cast<Var*>(stack.back())) {
          var->value = new IntConst(std::get<int>(token.second));
          stack.push_back(var->value);
        }
      } else {
        stack.push_back(new IntConst(std::get<int>(token.second)));
      }
    } else if (token.first == TokenType::op) {
      if (stack.size()) {
        char op = std::get<char>(token.second);
        if (auto ptr = dynamic_cast<IntConst *>(stack.back())) {
          auto left = stack.back(); stack.pop_back(); // found var, replace value with bop
          BOp *bop = new BOp(left, op);
          if (auto var = dynamic_cast<Var*>(stack.back())) {
            var->value = bop;
          }
          stack.push_back(bop);
        } else if (auto bop = dynamic_cast<BOp *>(stack.back())) {
          if (prec_map.at(op) > prec_map.at(bop->op)) {
            bop->right = new BOp(bop->right, op);
            stack.push_back(bop->right);
          } else { 
            auto left = stack.back(); stack.pop_back();
            stack.push_back(new BOp(left, op));
          }
        } // 1 + 2 + 3
      }
    } else if (token.first == TokenType::type) {
      if (tokenizer.peek() && tokenizer.peek()->first == TokenType::id) {
        std::string id = std::move(std::get<std::string>(tokenizer.eat()->second));
        char punct = std::get<char>(tokenizer.eat()->second);
        if (punct == '=') { 
          stack.push_back(new Var(id, std::get<std::string>(token.second)));
        }
      }
    } else if (token.first == TokenType::punct) {
      char punct = std::get<char>(token.second);
      if (punct == ';') {
        result.push_back(stack[0]);
        stack.clear();
      } else if (punct == '=') {
        // result.push_back(stack[0]);
      }
    }
  }
  return result;
}
}; // namespace AST