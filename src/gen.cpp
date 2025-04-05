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
#include <print>
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

llvm::Value* getValueForNode(Node* node) {
  if (auto num = dc<IntConst>(node)) return ConstantInt::get(INT32, num->value);
  if (nodeMap.contains(node)) return nodeMap[node];
  llvm::Value* value;
  if (auto var = dc<Name>(node)) {
    if (nameMap.contains(var->id)) return nameMap[var->id];
  }
  return nullptr;
}

llvm::Value* ret(const Ret* var, llvm::IRBuilder<>& builder, llvm::Module& module) {
  if (auto name = dc<Name>(var->value)) {
    auto load = builder.CreateLoad(INT32, nameMap[name->id]);
    builder.CreateRet(load);
  } else if (auto call = dc<Call>(var->value)) {
    builder.CreateRet(builder.CreateCall(module.getFunction("f")));
  } else if (auto bop = dc<BOp>(var->value)) {
    builder.CreateRet(nodeMap[bop]);
  }
}

llvm::Value* var(const Var* var, llvm::IRBuilder<>& builder) {
  if (auto num = dc<const IntConst>(var->value)) {
    builder.CreateStore(builder.getInt32(num->value), nodeMap[var]);
  } else if (auto bopStatement = dc<BOp>(var->value)) {
    builder.CreateStore(bop(bopStatement, builder), nameMap[var->id]);
  }
}

llvm::Value* bop(const BOp* statement, llvm::IRBuilder<>& builder) {
  llvm::Value* last;
  vst::postorder(statement, [&](const Node* child) {
    if (const BOp* bop = dc<const BOp>(child)) {
      auto left = builder.CreateLoad(INT32, getValueForNode(bop->left.get()));
      auto right = builder.CreateLoad(INT32, getValueForNode(bop->right.get()));
      if (bop->op == "*") {
        nodeMap[bop] = builder.CreateMul(left, right);
      } else if (bop->op == "+") {
        nodeMap[bop] = builder.CreateAdd(left, right);
      } else if (bop->op == "-") {
        nodeMap[bop] = builder.CreateSub(left, right);
      } else if (bop->op == "/") {
        nodeMap[bop] = builder.CreateSDiv(left, right);
      }
      last = nodeMap[bop];
    }
  });
  return last;
}

void emitStmt(const Node* stmt, IRBuilder<>& builder, llvm::Module& module) {
  if (auto b = dc<const BOp>(stmt)) {
    bop(b, builder);
  } else if (auto v = dc<const Var>(stmt)) {
    var(v, builder);
  } else if (auto r = dc<const Ret>(stmt)) {
    ret(r, builder, module);
  }
}

void createFunction(Fn* fn, IRBuilder<>& builder, LLVMContext& context, Module& module) {
  INT32 = builder.getInt32Ty();
  FunctionType* functionType = FunctionType::get(INT32, { }, false);
  Function* function = Function::Create(functionType, Function::ExternalLinkage, fn->id, module);
  builder.SetInsertPoint(BasicBlock::Create(context, "entry", function));

  for (auto& statement : fn->body) {
    if (auto var = dc<Var>(statement)) {
      if (var->op != "=" or nameMap.contains(var->id)) continue;
      nodeMap[var] = builder.CreateAlloca(INT32);
      nameMap[var->id] = nodeMap[var];
    }
  }

  for (auto& statement : fn->body) {
    vst::postorder(statement.get(), [&](const Node* child) { emitStmt(child, builder, module); });
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

void emit(vector<unique_ptr<Node>> ast, string moduleName) {
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
  llvm::outs() << "// LLVM IR:\n";
  module.print(llvm::outs(), nullptr);
  emitMC(module);
}

void emitMC(llvm::Module& module) {
  llvm::outs() << "\n// Assembly:\n";

  // Generate assembly for the native target
  auto targetTriple = llvm::sys::getDefaultTargetTriple();
  module.setTargetTriple(Triple(targetTriple));

  std::string error;
  auto target = llvm::TargetRegistry::lookupTarget(targetTriple, error);

  if (!target) {
    llvm::errs() << "Error looking up target: " << error << "\n";
    return;
  }

  llvm::TargetOptions opt;
  auto RM = std::optional<llvm::Reloc::Model>();
  auto targetMachine = target->createTargetMachine(targetTriple, "generic", "", opt, RM, { }, CodeGenOptLevel::None);

  module.setDataLayout(targetMachine->createDataLayout());

  // Configure the pass pipeline for generating assembly
  llvm::legacy::PassManager pass;

  // Use stdout instead of a file
  if (targetMachine->addPassesToEmitFile(pass, llvm::outs(), nullptr, CodeGenFileType::AssemblyFile)) {
    llvm::errs() << "Target machine can't emit a file of this type\n";
    return;
  }

  // Run the pass pipeline
  pass.run(module);
}
};