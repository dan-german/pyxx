#include "prs.h"
#include <print>
using namespace std;

const unordered_map<string, int> PREC_MAP = {
  { "+", 0 }, { "-", 0 },
  { "*", 1 }, { "/", 1 }
};
const unordered_set<string> ASSIGN_OPS { "=", "+=", "-=" };

namespace AST {
void Parser::handleInt(const Token &token) {
  if (stack.empty()) {
    stack.push_back(new IntConst(stoi(token.second)));
    return;
  }
  // if top of stack is a bop
  if (auto bop = dynamic_cast<BOp *>(stack.back())) {
    stack.pop_back();
    bop->right = new IntConst(stoi(token.second));
    stack.push_back(bop);
  } else if (auto var = dynamic_cast<Var *>(stack.back())) {
    stack.push_back(new IntConst(stoi(token.second)));
    return;
  }
}

void Parser::skipSpace() {
  while (lexer.peek() && lexer.peek()->first == TokenType::punct && lexer.peek()->second == " ")
    lexer.eat();
}

void Parser::handleId(const Token &token) {
  Var *var = new Var(token.second);
  stack.push_back(var);
  skipSpace();
  if (lexer.peek()->first == TokenType::op) {
    var->op = lexer.eat()->second;
  }
}

void Parser::handlePunct(const Token &token) {
  if (token.second == "n") {
    stamp();
  } else if (token.second == "def") {
    skipSpace();
    string id = lexer.eat(TokenType::id)->second;
    lexer.eat({},"(");
    lexer.eat({},")");
    lexer.eat({},":");
    lexer.eat({},"n");
    Fn* fn = new Fn(id, {});
    stack.push_back(fn);
  }
}

void Parser::parseBody() { 

}

void Parser::handleOp(const Token &token) {
  if (auto ptr = dynamic_cast<IntConst *>(stack.back())) {
    auto left = stack.back(); stack.pop_back(); // found var, replace value with bop
    BOp *bop = new BOp(left, token.second);
    if (auto var = dynamic_cast<Var *>(left)) {
      var->value = bop;
    }
    stack.push_back(bop);
  } else if (auto bop = dynamic_cast<BOp *>(stack.back())) {
    if (PREC_MAP.at(token.second) > PREC_MAP.at(bop->op)) {
      bop->right = new BOp(bop->right, token.second);
      stack.push_back(bop->right);
    } else {
      auto left = stack.back(); stack.pop_back();
      stack.push_back(new BOp(left, token.second));
    }
  }
}

void Parser::stamp() {
  if (stack.size() > 1) {
    if (auto var = dynamic_cast<Var *>(stack[statement_index++])) {
      var->value = stack.back(); stack.pop_back();
    }
  }
}

vector<Node *> Parser::parse() {
  while (lexer.peek()) {
    const Token token = lexer.eat().value();
    print_tok(token);
    switch (token.first) {
    case TokenType::id: handleId(token); break;
    case TokenType::op: handleOp(token); break;
    case TokenType::type: break;
    case TokenType::punct: handlePunct(token); break;
    case TokenType::int_const: handleInt(token); break;
    default: break;
    }
  }

  stamp();
  return stack;
}
}; // namespace AST