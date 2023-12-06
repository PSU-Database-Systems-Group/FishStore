// Main AST file to #include. Including this will include all other necessary AST files. It also provides the
// EzPsfInit() function which will set up everything necessary to start using the AST classes.
//
// Created by Max Norfolk on 2/3/23.

#pragma once

#include "jit/ast/abstract_ast.h"
#include "jit/ast/expr_ast.h"
#include "jit/ast/lit_ast.h"
#include "jit/ast/psf_ast.h"

#include "jit/datatypes/conversion.h"


namespace fishstore::ezpsf::ast {

    // Initializes everything using a given adapter
    void EzPsfInit() {
        /////////////////////////////////////////////////////
        // Setup JIT stuff
        /////////////////////////////////////////////////////

        llvm_consts::ctx = std::make_unique<llvm::LLVMContext>();
        llvm_consts::builder = std::make_unique<llvm::IRBuilder<>>(*llvm_consts::ctx);
        llvm_consts::module = std::make_unique<llvm::Module>("EzPsf", *llvm_consts::ctx);

        llvm::PointerType *llvm_ptr_ty = llvm::PointerType::getUnqual(*llvm_consts::ctx);


        // create the string struct
        llvm::StructType::create(
                {llvm_ptr_ty, llvmConvertType(DataType::UINT64_T)},
                getName(DataType::STR_T), false
        );

        /////////////////////////////////////////////////////
        // Create Nullable Structs
        /////////////////////////////////////////////////////

        createNullableStruct(DataType::BOOL_T);
        createNullableStruct(DataType::INT32_T);
        createNullableStruct(DataType::INT64_T);
        createNullableStruct(DataType::DOUBLE_T);
        createNullableStruct(DataType::STR_T);

        /////////////////////////////////////////////////////
        // Extract Stuff from Fields function
        /////////////////////////////////////////////////////

        // (void* field, uint64_t id, void* Nullable...)
        llvm::Type *getFuncArgs[] = {llvm_ptr_ty, llvmConvertType(DataType::UINT64_T), llvm_ptr_ty};

        auto *ft_get = llvm::FunctionType::get(llvm::Type::getVoidTy(*llvm_consts::ctx), getFuncArgs, false);


        // create function prototypes
        llvm::Function::Create(ft_get, llvm::Function::ExternalLinkage, type_conversion::REF_GET_BOOL,
                               *llvm_consts::module);
        llvm::Function::Create(ft_get, llvm::Function::ExternalLinkage, type_conversion::REF_GET_INT32,
                               *llvm_consts::module);
        llvm::Function::Create(ft_get, llvm::Function::ExternalLinkage, type_conversion::REF_GET_INT64,
                               *llvm_consts::module);
        llvm::Function::Create(ft_get, llvm::Function::ExternalLinkage, type_conversion::REF_GET_DOUBLE,
                               *llvm_consts::module);
        llvm::Function::Create(ft_get, llvm::Function::ExternalLinkage, type_conversion::REF_GET_STR,
                               *llvm_consts::module);




        /////////////////////////////////////////////////////
        // String Manipulation Functions
        /////////////////////////////////////////////////////

        llvm::FunctionType *str_len_ft = llvm::FunctionType::get(llvmConvertType(DataType::UINT64_T),
                                                                 {llvmConvertType(DataType::STR_T)},
                                                                 false);
        llvm::FunctionType *str_cmp_ft = llvm::FunctionType::get(llvmConvertType(DataType::INT32_T),
                                                                 {llvmConvertType(DataType::STR_T)},
                                                                 false);

        llvm::Function::Create(str_len_ft, llvm::Function::ExternalLinkage, str_info::REF_STR_LEN,
                               *llvm_consts::module);
        llvm::Function::Create(str_cmp_ft, llvm::Function::ExternalLinkage, str_info::REF_STR_CMP,
                               *llvm_consts::module);
    }
}