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
#include "llvm/IR/PassManager.h"
#include "llvm/Transforms/Utils/Mem2Reg.h"
#include "llvm/Transforms/Utils.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Analysis/LoopAnalysisManager.h"
#include "llvm/Analysis/CGSCCPassManager.h"
#include <ranges>

#include "prs.h"
#include <vst.h>
#include "gen.h"
#include "utils.h"

using namespace llvm;
using namespace std;
using namespace ast;

namespace gen {
u_map<const Node*, llvm::Value*> nodeMap;
u_map<string, llvm::Value*> nameMap;
llvm::IntegerType* INT32;
llvm::IntegerType* INT1;

llvm::Value* getValueForNode(Node* node, IRBuilder<>& builder) {
  if (const IntLiteral* num = dc<const IntLiteral>(node)) {
    return ConstantInt::get(INT32, num->value);
  }
  llvm::Value* value = nullptr;
  if (nodeMap.contains(node)) {
    return nodeMap[node];
  } else if (auto var = dc<Name>(node)) {
    return builder.CreateLoad(INT32, nameMap[var->id]);
  }
}

llvm::Value* ret(const Ret* var, llvm::IRBuilder<>& builder, llvm::Module& module) {
  if (auto name = dc<Name>(var->value)) {
    auto load = builder.CreateLoad(INT32, nameMap[name->id]);
    builder.CreateRet(load);
  } else if (auto call = dc<Call>(var->value)) {
    builder.CreateRet(builder.CreateCall(module.getFunction("f")));
  } else if (auto num = dc<IntLiteral>(var->value)) {
    builder.CreateRet(ConstantInt::get(INT32, num->value));
  } else { 
    builder.CreateRet(nodeMap[var->value.get()]);
  }
}

llvm::Value* var(const Var* var, llvm::IRBuilder<>& builder) {
  if (auto num = dc<const IntLiteral>(var->value)) {
    builder.CreateStore(builder.getInt32(num->value), nameMap[var->id]);
  } else if (auto bop = dc<BOp>(var->value)) {
    builder.CreateStore(nodeMap[bop], nameMap[var->id]);
  } else if (auto condExpr = dc<CondExpr>(var->value)) {
    builder.CreateStore(nodeMap[condExpr], nameMap[var->id]);
  }
}

llvm::Value* bop(const BOp* statement, llvm::IRBuilder<>& builder) {
  llvm::Value* last;
  if (const BOp* bop = dc<const BOp>(statement)) {
    auto left = getValueForNode(bop->left.get(), builder);
    auto right = getValueForNode(bop->right.get(), builder);
    if (bop->op == "*") {
      nodeMap[bop] = builder.CreateMul(left, right);
    } else if (bop->op == "+") {
      nodeMap[bop] = builder.CreateAdd(left, right);
    } else if (bop->op == "-") {
      nodeMap[bop] = builder.CreateSub(left, right);
    } else if (bop->op == "/") {
      nodeMap[bop] = builder.CreateSDiv(left, right);
    } else if (bop->op == "==") {
      nodeMap[bop] = builder.CreateICmpEQ(left, right);
    }
    last = nodeMap[bop];
  }
  return last;
}

void if_(const ast::If* if_, llvm::LLVMContext& context, llvm::Function* function, llvm::IRBuilder<>& builder, llvm::Module& module) {
  llvm::Value* lastTestOp = nullptr;
  vst::postorder(if_->test.get(), [&](const Node* child) {
    lastTestOp = emitStmt(child, builder, module);
  });
  llvm::BasicBlock* thenBlock = llvm::BasicBlock::Create(context, "then", function);
  llvm::BasicBlock* elseBlock = llvm::BasicBlock::Create(context, "else", function);
  llvm::BasicBlock* mergeBlock = llvm::BasicBlock::Create(context, "endif", function);
  builder.CreateCondBr(lastTestOp, thenBlock, elseBlock);
  builder.SetInsertPoint(thenBlock);
  for (auto& stmt : if_->then) {
    vst::postorder(stmt.get(), [&](const Node* child) {
      emitStmt(child, builder, module);
    });
  }
  builder.CreateBr(mergeBlock);
  builder.SetInsertPoint(elseBlock);
  for (auto& stmt : if_->else_) {
    vst::postorder(stmt.get(), [&](const Node* child) {
      emitStmt(child, builder, module);
    });
  }
  builder.CreateBr(mergeBlock);
  builder.SetInsertPoint(mergeBlock);
}

llvm::Value* condExpr(const CondExpr* condExpr, IRBuilder<>& builder, llvm::Module& module) {
  auto test = emitStmt(condExpr->test.get(), builder, module);
  auto trueExpr = emitStmt(condExpr->trueExpr.get(), builder, module);
  auto falseExpr = emitStmt(condExpr->falseExpr.get(), builder, module);
  return nodeMap[condExpr] = builder.CreateSelect(test, trueExpr, falseExpr);
}

llvm::Value* emitStmt(const Node* stmt, IRBuilder<>& builder, llvm::Module& module) {
  if (auto b = dc<const BOp>(stmt)) {
    return bop(b, builder);
  } else if (auto v = dc<const Var>(stmt)) {
    return var(v, builder);
  } else if (auto r = dc<const Ret>(stmt)) {
    return ret(r, builder, module);
  } else if (auto c = dc<const IntLiteral>(stmt)) {
    return ConstantInt::get(INT32, c->value);
  } else if (auto c = dc<const BoolLiteral>(stmt)) {
    return ConstantInt::get(INT1, c->value);
  } else if (auto c = dc<const CondExpr>(stmt)) {
    return condExpr(c, builder, module);
  }
}

void createFunction(Fn* fn, IRBuilder<>& builder, LLVMContext& context, Module& module) {
  nodeMap.clear();
  nameMap.clear();
  INT32 = builder.getInt32Ty();
  INT1 = builder.getInt1Ty();
  llvm::SmallVector<llvm::Type*> argTypes(fn->args.size(), INT32);
  FunctionType* functionType = FunctionType::get(INT32, argTypes, false);
  Function* function = Function::Create(functionType, Function::ExternalLinkage, fn->id, module);
  builder.SetInsertPoint(BasicBlock::Create(context, "entry", function));

  // handle args
  for (int i = 0; i < fn->args.size(); i++) {
    auto irArg = function->getArg(i);
    irArg->setName(fn->args[i]->id);
  }

  // handle args allocations
  for (int i = 0; i < fn->args.size(); i++) {
    nodeMap[fn->args[i].get()] = builder.CreateAlloca(INT32);
    nameMap[function->getArg(i)->getName().str()] = nodeMap[fn->args[i].get()];
  }

  // create allocations
  for (auto& statement : fn->body) {
    if (auto var = dc<Var>(statement)) {
      if (var->op != "=" or nameMap.contains(var->id)) continue;
      nodeMap[var] = builder.CreateAlloca(INT32);
      nameMap[var->id] = nodeMap[var];
    }
  }

  for (auto& stmt : fn->body) {
    if (auto i = dc<const If>(stmt)) {
      if_(i, context, function, builder, module);
    } else {
      vst::postorder(stmt.get(), [&](const Node* child) {
        emitStmt(child, builder, module);
      });
    }
  }
}

void handlePasses(Module& module) {
  // Create the analysis managers.
  // These must be declared in this order so that they are destroyed in the
  // correct order due to inter-analysis-manager references.
  LoopAnalysisManager LAM;
  FunctionAnalysisManager FAM;
  CGSCCAnalysisManager CGAM;
  ModuleAnalysisManager MAM;

  // Create the new pass manager builder.
  // Take a look at the PassBuilder constructor parameters for more
  // customization, e.g. specifying a TargetMachine or various debugging
  // options.
  PassBuilder PB;

  // Register all the basic analyses with the managers.
  PB.registerModuleAnalyses(MAM);
  PB.registerCGSCCAnalyses(CGAM);
  PB.registerFunctionAnalyses(FAM);
  PB.registerLoopAnalyses(LAM);
  PB.crossRegisterProxies(LAM, FAM, CGAM, MAM);

  // Create the pass manager.
  ModulePassManager MPM = PB.buildPerModuleDefaultPipeline(OptimizationLevel::O0);
  MPM.addPass(createModuleToFunctionPassAdaptor(PromotePass()));

  // Optimize the IR!
  MPM.run(module, MAM);
}

string emit(vector<unique_ptr<Node>> ast, string moduleName) {
  llvm::InitializeNativeTarget();
  llvm::InitializeNativeTargetAsmParser();
  llvm::InitializeNativeTargetAsmPrinter();

  LLVMContext context;
  IRBuilder builder(context);
  Module module(moduleName, context);

  for (auto& node : ast) {
    if (auto fn = dc<Fn>(node)) {
      createFunction(fn, builder, context, module);
    }
  }
  // handlePasses(module);
  SmallString<1024> res;
  raw_svector_ostream os(res);
  module.print(os, nullptr);
  return string(res);
}

string emitMC(llvm::Module& module) {
  llvm::outs() << "\n// Assembly:\n";

  // Generate assembly for the native target
  auto targetTriple = llvm::sys::getDefaultTargetTriple();
  module.setTargetTriple(Triple(targetTriple));

  std::string error;
  auto target = llvm::TargetRegistry::lookupTarget(targetTriple, error);

  if (!target) {
    llvm::errs() << "Error looking up target: " << error << "\n";
    return "";
  }

  llvm::TargetOptions opt;
  auto RM = std::optional<llvm::Reloc::Model>();
  auto targetMachine = target->createTargetMachine(targetTriple, "generic", "", opt, RM, { }, CodeGenOptLevel::None);

  module.setDataLayout(targetMachine->createDataLayout());

  // Configure the pass pipeline for generating assembly
  llvm::legacy::PassManager pass;

  // Use stdout instead of a file
  SmallString<1024> res;
  // string res;
  raw_svector_ostream os(res);
  if (targetMachine->addPassesToEmitFile(pass, os, nullptr, CodeGenFileType::AssemblyFile)) {
    llvm::errs() << "Target machine can't emit a file of this type\n";
    return "";
  }

  // Run the pass pipeline
  pass.run(module);
  return string(res);
}
} // namespace gen