// The LLVM elements used by the AST to generate IR code.
//
// See EzPsfInit() in ast.h for variable initialization.
//
// Created by Max Norfolk on 3/27/23.


#pragma once

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>

namespace fishstore::ezpsf::llvm_consts {
    static std::unique_ptr<llvm::LLVMContext> ctx;
    static std::unique_ptr<llvm::IRBuilder<>> builder;
    static std::unique_ptr<llvm::Module> module;
}