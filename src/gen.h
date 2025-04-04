#pragma once
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/TargetParser/Host.h"
#include "llvm/IR/LegacyPassManager.h"

#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/MC/TargetRegistry.h"
#include <ranges>

#include "prs.h"
#include <print>

using namespace llvm;
using namespace std;
using namespace ast;

namespace gen {
llvm::Value* getValueForNode(Node* node);
llvm::Value* handleBOpStmt(BOp* statement, llvm::IRBuilder<>& builder);
void emit(vector<unique_ptr<ast::Node>> ast, std::string moduleName = "");
void createFunction(Fn* fn, IRBuilder<>& builder, LLVMContext& context, Module& module);
};