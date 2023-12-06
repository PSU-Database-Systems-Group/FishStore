// List of abstract classes to extend
//
// Created by Max Norfolk on 7/24/23.


#pragma once

#include "jit/datatypes/types.h"

namespace fishstore::ezpsf::ast {
    class ExprAst {
    public:
        virtual ~ExprAst() = default;

        virtual llvm::Value *code() = 0;

        virtual DataType type() = 0;
    };
}