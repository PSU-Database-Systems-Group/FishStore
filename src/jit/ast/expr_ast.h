// AST for operators and casting
//
// Created by Max Norfolk on 1/29/23.
#pragma once

#include "jit/ast/lit_ast.h"
#include "jit/datatypes/str_info.h"

#define ASSERT_BOTH_NUM ASSERT(isNumType(left->type()) && isNumType(right->type()), "Left: " + getName(left->type()) + ", Right: " + getName(right->type()))

#define ASSERT_BOTH_INT ASSERT(isIntType(left->type()) && isIntType(right->type()), "Left: " + getName(left->type()) + ", Right: " + getName(right->type()))


#define builderCreateFI(OP) ASSERT_BOTH_NUM; \
if (isIntType(return_type)) \
return llvm_consts::builder->Create##OP(left_val, right_val, "int" #OP "Tmp"); \
return llvm_consts::builder->CreateF##OP(left_val, right_val, "float" #OP "Tmp")

#define builderCreateCmp(OP) ASSERT_BOTH_NUM; \
if (isIntType(args_type)) \
return llvm_consts::builder->CreateICmpS##OP(left_val, right_val, "int" #OP "Tmp"); \
return llvm_consts::builder->CreateFCmpO##OP(left_val, right_val, "float" #OP "Tmp")

namespace fishstore::ezpsf::ast {

    enum class OperatorFlags : uint32_t {
        SAME = 1 << 10,
        BOOL = 1 << 11,
        INT = 1 << 12,
        DOUBLE = 1 << 13,
        DOUBLE_IF = 1 << 14, // double if one is a double
        STRING_IF = 1 << 15, // string if one is a string, else double if
    };

    inline constexpr uint32_t
    operator|(uint32_t
    f1,
    OperatorFlags f2
    ) {
    return f1 | static_cast
    <uint32_t>(f2);
}

inline constexpr uint32_t
operator|(OperatorFlags
f1,
OperatorFlags f2
) {
return static_cast
<uint32_t>(f1)
| static_cast
<uint32_t>(f2);
}

// no type should be set to higher than 1024 (1 << 10), as that is used by flags
enum class Operator : uint32_t {
    ERROR_OP = 0xffffffff,

    PRE_MINUS = 0 | OperatorFlags::SAME,
    PRE_NOT = 1 | OperatorFlags::BOOL,

    MULTI = 10 | OperatorFlags::DOUBLE_IF,
    DIV_INT = 11 | OperatorFlags::INT,
    DIV_FLOAT = 12 | OperatorFlags::DOUBLE,
    MOD = 13 | OperatorFlags::INT,

    PLUS = 20 | OperatorFlags::STRING_IF,
    MINUS = 21 | OperatorFlags::DOUBLE_IF,

    BIT_AND = 30 | OperatorFlags::INT,
    BIT_XOR = 31 | OperatorFlags::INT,
    BIT_OR = 32 | OperatorFlags::INT,

    EQ = 40 | OperatorFlags::BOOL,
    NEQ = 41 | OperatorFlags::BOOL,
    LT = 42 | OperatorFlags::BOOL,
    GT = 43 | OperatorFlags::BOOL,
    LT_EQ = 44 | OperatorFlags::BOOL,
    GT_EQ = 45 | OperatorFlags::BOOL,

    AND = 50 | OperatorFlags::BOOL,
    OR = 51 | OperatorFlags::BOOL,

    BAR = 60 | OperatorFlags::INT
};

// returns the return type for the given operator
OperatorFlags getReturnType(Operator op) {
    return static_cast<OperatorFlags>((uint32_t)
    op & (
            OperatorFlags::SAME |
            OperatorFlags::BOOL |
            OperatorFlags::INT |
            OperatorFlags::DOUBLE |
            OperatorFlags::DOUBLE_IF |
            OperatorFlags::STRING_IF
    ));
}

DataType fromOp(DataType t1, DataType t2, Operator op) {
    switch (getReturnType(op)) {

        case OperatorFlags::SAME:
            return t1;

        case OperatorFlags::BOOL:
            return DataType::BOOL_T;

        case OperatorFlags::DOUBLE:
            return DataType::DOUBLE_T;

        case OperatorFlags::STRING_IF:
            if (t1 == DataType::STR_T || t2 == DataType::STR_T)
                return DataType::STR_T;
            // FALL THROUGH to figure out double if
        case OperatorFlags::DOUBLE_IF:
            if (t1 == DataType::DOUBLE_T || t2 == DataType::DOUBLE_T)
                return DataType::DOUBLE_T;
            // FALL THROUGH to figure out int
        case OperatorFlags::INT:
            return (t1 == DataType::INT64_T || t2 == DataType::INT64_T)
                   ? DataType::INT64_T : DataType::INT32_T;
    }
    LOG("Could not determine return type!");
    return DataType::ERROR_T;
}

// casts a llvm Value to the target type based upon the current type of the value
llvm::Value *castToTarget(llvm::Value *value, DataType current, DataType target) {
    if (current == target)
        return value;

    bool current_is_int = isIntType(current);
    bool current_is_float = isFloatType(current);
    bool current_is_signed = isSignedType(current);

    bool target_is_int = isIntType(target);
    bool target_is_float = isFloatType(target);
    bool target_is_signed = isSignedType(target);

    if (current_is_int && target_is_float) {
        if (current_is_signed) {
            return llvm_consts::builder->CreateSIToFP(value, llvmConvertType(target), "signedIntToFp");
        } else {
            return llvm_consts::builder->CreateUIToFP(value, llvmConvertType(target), "unsignedIntToFp");
        }
    }

    if (current_is_float && target_is_int) {
        if (target_is_signed) {
            return llvm_consts::builder->CreateFPToSI(value, llvmConvertType(target), "fpToSignedInt");
        } else {
            return llvm_consts::builder->CreateFPToUI(value, llvmConvertType(target), "fpToUnsignedInt");
        }
    }

    if (current_is_int && target_is_int) {
        return llvm_consts::builder->CreateIntCast(value, llvmConvertType(target), target_is_signed,
                                                   "intToInt");
    }

    // At the moment this should not be used
    if (current_is_float && target_is_float) {
        LOG("Warning: Casting Float to Float(!?). Should be unreachable until FLOAT_T implemented!");
        return llvm_consts::builder->CreateFPCast(value, llvmConvertType(target), "fpToFp");
    }

    LOG_RET("Unable to transform type '" + getName(current) + "' to type '" + getName(target) + "'.");
}


// binary operators
class BinaryOpExprAst : public ExprAst {
public:
    BinaryOpExprAst(ExprAst *left, ExprAst *right, Operator op)
            : left(left), right(right),
              op(op), return_type(fromOp(left->type(), right->type(), op)) {}


    DataType type() override {
        return return_type;
    }

    ~BinaryOpExprAst() override {
        delete left;
        delete right;
    }

    llvm::Value *code() override {
        llvm::Value *left_val = left->code();
        llvm::Value *right_val = right->code();
        NULL_CHECK(left_val);
        NULL_CHECK(right_val);


        DataType left_old = left->type();
        DataType right_old = right->type();

        /////////////////////////////////////////////////////
        // Automatic casting
        /////////////////////////////////////////////////////

        DataType args_type;

        switch (return_type) {
            case DataType::INT32_T:
                args_type = DataType::INT32_T;
                break;

            case DataType::INT64_T:
                args_type = DataType::INT64_T;
                break;
                // if the return type is bool, then we want to convert to the same type
            case DataType::BOOL_T:

                // if the return type is bool, then we want to reuse the DOUBLE_IF logic
                args_type = fromOp(left_old, right_old, Operator::PLUS);
                break;
            case DataType::DOUBLE_T:
                args_type = DataType::DOUBLE_T;
                break;
            default:
                LOG_RET("Unsupported return type" + getName(return_type));
        }

        left_val = castToTarget(left_val, left_old, args_type);
        right_val = castToTarget(right_val, right_old, args_type);

        /////////////////////////////////////////////////////
        // Actual Operator Code
        /////////////////////////////////////////////////////

        switch (op) {
            case Operator::ERROR_OP:
                LOG_RET("Error Operator Found");
            case Operator::PRE_MINUS:
                LOG_RET("Operator Pre Minus is not Binary");
            case Operator::PRE_NOT:
                LOG_RET("Operator Pre Not is not Binary");
            case Operator::MULTI:
            builderCreateFI(Mul);
            case Operator::DIV_INT:
                ASSERT_BOTH_NUM;
                if (isSignedType(return_type))
                    return llvm_consts::builder->CreateSDiv(left_val, right_val, "intSDivTmp");
                return llvm_consts::builder->CreateUDiv(left_val, right_val, "intUDivTmp");
            case Operator::DIV_FLOAT:
                ASSERT_BOTH_NUM;
                return llvm_consts::builder->CreateFDiv(left_val, right_val, "floatDivTmp");
            case Operator::MOD:
                ASSERT_BOTH_NUM;
                ASSERT(isSignedType(return_type));
                return llvm_consts::builder->CreateSRem(left_val, right_val, "sModTmp");
                LOG_RET("Unsigned Mod");
            case Operator::PLUS:
            builderCreateFI(Add);
            case Operator::MINUS:
            builderCreateFI(Sub);
            case Operator::EQ:
                if (left->type() == DataType::STR_T) {
                    auto str_len = llvm_consts::module->getFunction(str_info::REF_STR_CMP);
                    auto func_call = llvm_consts::builder->CreateCall(str_len, {left_val, right_val}, "strCmpCall");
                    return llvm_consts::builder->CreateICmpEQ(func_call, LitInt32Ast(0).code(), "strEqTmp");
                }
                ASSERT_BOTH_NUM;
                if (isIntType(left->type()))
                    return llvm_consts::builder->CreateICmpEQ(left_val, right_val, "intEqTmp");
                return llvm_consts::builder->CreateFCmpOEQ(left_val, right_val, "floatEqTmp");
            case Operator::NEQ:
                ASSERT_BOTH_NUM;
                if (left->type() == DataType::STR_T) {
                    auto str_len = llvm_consts::module->getFunction(str_info::REF_STR_CMP);
                    auto func_call = llvm_consts::builder->CreateCall(str_len, {left_val, right_val}, "strCmpCall");
                    return llvm_consts::builder->CreateICmpNE(func_call, LitInt32Ast(0).code(), "strEqTmp");
                }
                if (isIntType(left->type()))
                    return llvm_consts::builder->CreateICmpNE(left_val, right_val, "intNeqTmp");
                return llvm_consts::builder->CreateFCmpONE(left_val, right_val, "floatNeqTmp");
            case Operator::LT:
            builderCreateCmp(LT);
            case Operator::GT:
            builderCreateCmp(GT);
            case Operator::LT_EQ:
            builderCreateCmp(LE);
            case Operator::GT_EQ:
            builderCreateCmp(GE);
            case Operator::BIT_AND:
                ASSERT_BOTH_INT;
                return llvm_consts::builder->CreateAnd(left_val, right_val, "andTmp");
            case Operator::BIT_XOR:
                ASSERT_BOTH_INT;
                return llvm_consts::builder->CreateXor(left_val, right_val, "xorTmp");
            case Operator::BIT_OR:
                ASSERT_BOTH_INT;
                return llvm_consts::builder->CreateOr(left_val, right_val, "orTmp");
            case Operator::BAR:
                LOG_RET("Operator Bar is not Binary");
            default:
                LOG_RET("Invalid operator!");
        }
    }

    ExprAst *left;
    ExprAst *right;
    Operator op;
    DataType return_type;
};

class UnaryOpExprAst : public ExprAst {
public:
    UnaryOpExprAst(ExprAst *expr, Operator op) : expr(expr), op(op) {}

    DataType type() override {
        return fromOp(expr->type(), DataType::ERROR_T, op);
    }

    llvm::Value *code() override {
        auto expr_type = expr->type();
        switch (op) {
            case Operator::BAR: { // TODO => absolute value
                // string length
                ASSERT(expr_type == DataType::STR_T, getName(expr_type));
                // TODO replace this function call with just accessing the value with raw LLVM code
                auto temp_alloca = llvm_consts::builder->CreateAlloca(llvmConvertType(DataType::STR_T),
                                                                      nullptr, "tempStrAlloca");
                llvm_consts::builder->CreateStore(expr->code(), temp_alloca);
                auto gep_ptr = llvm_consts::builder->CreateStructGEP(llvmConvertType(DataType::STR_T), temp_alloca,
                                                                     1, "strLenPtr");
                auto len_as_uint64_t = llvm_consts::builder->CreateLoad(llvmConvertType(DataType::UINT64_T),
                                                                        gep_ptr, "loadStrLen");
                return llvm_consts::builder->CreateIntCast(len_as_uint64_t, llvmConvertType(DataType::INT32_T),
                                                           true, "strLenCastToInt32");
            }

            case Operator::PRE_MINUS:
                ASSERT(isNumType(expr_type));
                if (isIntType(expr_type))
                    return llvm_consts::builder->CreateNeg(expr->code(), "intNeg");
                return llvm_consts::builder->CreateFNeg(expr->code(), "floatNeg");

            case Operator::PRE_NOT:
                ASSERT(expr_type == DataType::BOOL_T);
                return llvm_consts::builder->CreateNot(expr->code(), "notTmp");
            default:
                LOG_RET("Operator is not a valid Unary operator!");
        }
    }

    ~UnaryOpExprAst() override {
        delete expr;
    }

    ExprAst *expr;
    Operator op;
};

class CastExprAst : public ExprAst {
public:
    CastExprAst(ExprAst *expr, DataType castType) : expr(expr), cast_type(castType) {}

    DataType type() override {
        return cast_type;
    }

    ~CastExprAst() override {
        delete expr;
    }

    llvm::Value *code() override {
        return castToTarget(expr->code(), expr->type(), cast_type);
    }

    ExprAst *expr;
    DataType cast_type;
};

// Ternary operator (cond ? expr1 : expr2)
class TernaryOpExprAst : public ExprAst {
public:
    TernaryOpExprAst(ExprAst *bool_expr, ExprAst *true_expr, ExprAst *false_expr)
            : bool_expr(bool_expr),
              true_expr(true_expr),
              false_expr(false_expr) {
        auto t_type = true_expr->type();
        auto f_type = false_expr->type();
        // TODO loosen expression with automatic casting ints => doubles as necessary
        ASSERT(t_type == t_type, "True Expr: " + getName(t_type) = ". False Expr: " + getName(f_type));
        ASSERT(bool_expr->type() == DataType::BOOL_T, "Bool Expr Type: " + getName(bool_expr->type()));
    }

    ~TernaryOpExprAst() override {
        delete bool_expr;
        delete true_expr;
        delete false_expr;
    }

    llvm::Value *code() override {
        auto llvm_bool = bool_expr->code();
        auto parent = llvm_consts::builder->GetInsertBlock()->getParent();

        // basic blocks to merge and branch for each expression
        auto bb_true = llvm::BasicBlock::Create(*llvm_consts::ctx, "ternary_true", parent);
        auto bb_false = llvm::BasicBlock::Create(*llvm_consts::ctx, "ternary_false", parent);
        auto bb_merge = llvm::BasicBlock::Create(*llvm_consts::ctx, "ternary_merge", parent);
        llvm_consts::builder->CreateCondBr(llvm_bool, bb_true, bb_false);

        // true expr basic block
        llvm_consts::builder->SetInsertPoint(bb_true);
        auto llvm_true = true_expr->code();
        llvm_consts::builder->CreateBr(bb_merge);

        // false expr basic block
        llvm_consts::builder->SetInsertPoint(bb_false);
        auto llvm_false = false_expr->code();
        llvm_consts::builder->CreateBr(bb_merge);

        // merged basic block
        llvm_consts::builder->SetInsertPoint(bb_merge);
        auto phi = llvm_consts::builder->CreatePHI(llvmConvertType(type()), 2, "phiTernary");
        phi->addIncoming(llvm_true, bb_true);
        phi->addIncoming(llvm_false, bb_false);
        return phi;
    }

    DataType type() override {
        return true_expr->type();
    }

    ExprAst *bool_expr;
    ExprAst *true_expr;
    ExprAst *false_expr;
};

// operator that will evaluate the first expression, and potentially skip evaluating the second expression
// based upon the return of the first expression.
class ShortCircuitOpExprAst : public ExprAst {
public:
    ShortCircuitOpExprAst(ExprAst *first_expr, ExprAst *second_expr, Operator op) : first_expr(first_expr),
                                                                                    second_expr(second_expr),
                                                                                    op(op) {
        ASSERT(first_expr->type() == DataType::BOOL_T, getName(first_expr->type()));
        ASSERT(second_expr->type() == DataType::BOOL_T, getName(second_expr->type()));
    }

    ~ShortCircuitOpExprAst() override {
        delete first_expr;
        delete second_expr;
    };

    llvm::Value *code() override {
        auto first_value = first_expr->code();
        auto parent = llvm_consts::builder->GetInsertBlock()->getParent();

        // basic blocks to merge and branch for each expression
        auto bb_try_fir = llvm_consts::builder->GetInsertBlock();
        auto bb_try_sec = llvm::BasicBlock::Create(*llvm_consts::ctx, "short_circuit_try", parent);
        auto bb_merge = llvm::BasicBlock::Create(*llvm_consts::ctx, "short_circuit_merge", parent);

        // short-circuiting code
        switch (op) {
            case Operator::AND:
                llvm_consts::builder->CreateCondBr(first_value, bb_try_sec, bb_merge);
                break;
            case Operator::OR:
                llvm_consts::builder->CreateCondBr(first_value, bb_merge, bb_try_sec);
                break;
            default:
                LOG_RET("Operator is not defined for short circuiting!");
        }

        // failed to short circuit
        llvm_consts::builder->SetInsertPoint(bb_try_sec);
        auto second_value = second_expr->code();
        llvm_consts::builder->CreateBr(bb_merge);

        // merge value
        llvm_consts::builder->SetInsertPoint(bb_merge);
        auto phi = llvm_consts::builder->CreatePHI(llvmConvertType(type()), 2, "phiShortCircuit");
        phi->addIncoming(first_value, bb_try_fir);
        phi->addIncoming(second_value, bb_try_sec);
        return phi;
    }

    DataType type() override {
        return DataType::BOOL_T;
    }

    ExprAst *first_expr;
    ExprAst *second_expr;

    Operator op;
};

}