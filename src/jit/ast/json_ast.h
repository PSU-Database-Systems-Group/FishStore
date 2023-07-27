// References a json field
//
// Created by Max Norfolk on 5/25/23.

#pragma once

#include <utility>
#include <vector>

#include "jit/ast/expr_ast.h"
#include "jit/ezpsf_exceptions.h"

namespace fishstore::ezpsf::ast {

    class JsonRefAst : public ExprAst {
    public:
        JsonRefAst(std::string json, DataType type) : json(std::move(json)), data_type(type), value(nullptr) {
            used_ids.emplace_back(this);
        }

        static std::vector<JsonRefAst *> used_ids;

        static void reset() {
            used_ids.clear();
        }

        // before calling code, the PSF code (psf_ast.h) should set the corresponding llvm::Value* with the return
        // of calling the C/C++ function. Additionally the llvm::Value* is not owned by this object, so it should
        // not be deleted by this destructor.
        llvm::Value *code() override {
            NULL_CHECK(value);
            return value;
        }

        DataType type() override {
            return data_type;
        }

        std::string json;
        DataType data_type;
        llvm::Value *value;
    };

    std::vector<JsonRefAst *> JsonRefAst::used_ids = {};
}