// AST for literal values
//
// Created by Max Norfolk on 2/2/23.


#pragma once

#include <cstdint>
#include <string>
#include <utility>

#include <llvm/IR/Constants.h>

#include "jit/ast/abstract_ast.h"


namespace fishstore::ezpsf::ast {

    class LitInt8Ast : public ExprAst {
    public:
        explicit LitInt8Ast(const int8_t value) : value(value) {}

        DataType type() override {
            return DataType::INT8_T;
        }

        llvm::Value *code() override {
            return llvm::ConstantInt::get(llvmConvertType(DataType::INT8_T), value, true);
        }

        const int8_t value;
    };

    class LitInt32Ast : public ExprAst {
    public:
        explicit LitInt32Ast(const int32_t value) : value(value) {}

        DataType type() override {
            return DataType::INT32_T;
        }

        llvm::Constant *code() override {
            return llvm::ConstantInt::get(llvmConvertType(DataType::INT32_T), value, true);
        }

        const int32_t value;
    };

    class LitInt64Ast : public ExprAst {
    public:
        explicit LitInt64Ast(const int64_t value) : value(value) {}

        DataType type() override {
            return DataType::INT64_T;
        }

        llvm::Constant *code() override {
            return llvm::ConstantInt::get(llvmConvertType(DataType::INT64_T), value, true);
        }

        const int64_t value;
    };

    class LitDoubleAst : public ExprAst {
    public:
        explicit LitDoubleAst(const double value) : value(value) {}

        DataType type() override {
            return DataType::DOUBLE_T;
        }

        llvm::Constant *code() override {
            return llvm::ConstantFP::get(llvmConvertType(DataType::DOUBLE_T), value);
        }

        const double value;
    };

    class LitBoolAst : public ExprAst {
    public:
        explicit LitBoolAst(const bool value) : value(value) {}

        DataType type() override {
            return DataType::BOOL_T;
        }

        llvm::ConstantInt *code() override {
            return llvm::ConstantInt::getBool(*llvm_consts::ctx, value);
        }

        const bool value;
    };

    class LitStringAst : public ExprAst {
    public:
        explicit LitStringAst(std::string value) : value(std::move(value)) {
        }

        DataType type() override {
            return DataType::STR_T;
        }

        // creates a struct consisting of the string literal by creating a global string pointer, and then a constant
        // struct filled with the pointer followed by the size (see jit/conversion_declaration.h for definition of
        // c struct)
        llvm::Constant *code() override {
            auto lit_name = std::string("stringLit.") + std::string(value);
            auto ptr = llvm_consts::builder->CreateGlobalStringPtr(value, lit_name);
            auto size = LitInt64Ast(static_cast<int64_t>(value.size())).code();
            auto st = (llvm::StructType *) llvmConvertType(DataType::STR_T);

            return llvm::ConstantStruct::get(st, {ptr, size});
        }

        const std::string value;
    };
}