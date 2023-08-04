// visitor for single line PSFs
//
// Created by Max Norfolk on 5/25/23.

#pragma once

#include <antlr4-runtime.h>

#include "jit/gen/ezpsfLexer.h"
#include "jit/gen/ezpsfParser.h"
#include "jit/gen/ezpsfVisitor.h"

#include "jit/ast/ast.h"


namespace fishstore::ezpsf::ast {

    class EzPsfVisitor : public ezpsfVisitor {
    public:
        explicit EzPsfVisitor() {
            ast::EzPsfInit();
        }

        std::unique_ptr<PsfFunctionAst> createPsf(ezpsfParser::PsfContext *ctx) {
            static int anon_psf_counter = 0;
            VISIT_START
                std::string psf_name = ctx->JSON() == nullptr ? "anonymousPsf" + std::to_string(anon_psf_counter++)
                                                              : ctx->JSON()->getText();
                auto *default_value = ctx->expr().size() == 1 ? nullptr : std::any_cast<ExprAst *>(visit(ctx->expr(1)));
                auto ret_expr = std::any_cast<ExprAst *>(visit(ctx->expr(0)));
                return PsfFunctionAst::create(psf_name, default_value, ret_expr);
            VISIT_END
        }

        // do not use visitPsf, instead use createPsf(). This is because the ANTLR4 generated code must return a
        // std::any, to work with the builtin generated code. We want to return a unique_ptr<> which is not copyable,
        // and thus cannot be stored in a std::any.
        std::any visitPsf(ezpsfParser::PsfContext *ctx) override {
            LOG_RET("visitPsf should not be used. Instead use createPsf");
        }

        std::any visitCastExpr(ezpsfParser::CastExprContext *ctx) override {
            VISIT_START
                auto expr = std::any_cast<ExprAst *>(visit(ctx->expr()));
                auto type = std::any_cast<DataType>(visitType(ctx->type()));
                return (ExprAst *) new CastExprAst(expr, type);
            VISIT_END
        }

        std::any visitIndexExpr(ezpsfParser::IndexExprContext *ctx) override {
            LOG_RET("Index is not yet supported!");
        }

        std::any visitPreUnaryExpr(ezpsfParser::PreUnaryExprContext *ctx) override {
            VISIT_START
                auto expr = std::any_cast<ExprAst *>(visit(ctx->expr()));
                Operator op;
                switch (ctx->op->getType()) {
                    case ezpsfParser::MINUS:
                        op = Operator::PRE_MINUS;
                        break;
                    case ezpsfParser::NOT:
                        op = Operator::PRE_NOT;
                        break;
                    default:
                        LOG("unknown operator switched on in visitPreUnaryExpr");
                        op = Operator::ERROR_OP;
                }
                return (ExprAst *) new UnaryOpExprAst(expr, op);
            VISIT_END
        }

        std::any visitTernaryExpr(ezpsfParser::TernaryExprContext *ctx) override {
            VISIT_START
                auto bool_expr = std::any_cast<ExprAst *>(visit(ctx->expr(0)));
                auto true_expr = std::any_cast<ExprAst *>(visit(ctx->expr(1)));
                auto false_expr = std::any_cast<ExprAst *>(visit(ctx->expr(2)));
                return (ExprAst *) new TernaryOpExprAst(bool_expr, true_expr, false_expr);
            VISIT_END
        }

        std::any visitPlusMinusExpr(ezpsfParser::PlusMinusExprContext *ctx) override {
            VISIT_START
                auto left = std::any_cast<ExprAst *>(visit(ctx->expr(0)));
                auto right = std::any_cast<ExprAst *>(visit(ctx->expr(1)));
                Operator op = ctx->op->getType() == ezpsfParser::PLUS ? Operator::PLUS : Operator::MINUS;
                return (ExprAst *) new BinaryOpExprAst(left, right, op);
            VISIT_END
        }

        std::any visitMultiDivExpr(ezpsfParser::MultiDivExprContext *ctx) override {
            VISIT_START
                auto left = std::any_cast<ExprAst *>(visit(ctx->expr(0)));
                auto right = std::any_cast<ExprAst *>(visit(ctx->expr(1)));
                Operator op;
                switch (ctx->op->getType()) {
                    case ezpsfParser::MULTI:
                        op = Operator::MULTI;
                        break;
                    case ezpsfParser::FLOAT_DIV:
                        op = Operator::DIV_FLOAT;
                        break;
                    case ezpsfParser::INT_DIV:
                        op = Operator::DIV_INT;
                        break;
                    case ezpsfParser::MOD:
                        op = Operator::MOD;
                        break;
                    default:
                        LOG("unreachable state in switching on operator");
                        op = Operator::ERROR_OP;
                }
                return (ExprAst *) new BinaryOpExprAst(left, right, op);
            VISIT_END
        }

        std::any visitCmpExpr(ezpsfParser::CmpExprContext *ctx) override {
            VISIT_START
                auto left = std::any_cast<ExprAst *>(visit(ctx->expr(0)));
                auto right = std::any_cast<ExprAst *>(visit(ctx->expr(1)));
                Operator op;
                switch (ctx->op->getType()) {
                    case ezpsfParser::EQ:
                        op = Operator::EQ;
                        break;
                    case ezpsfParser::NEQ:
                        op = Operator::NEQ;
                        break;
                    case ezpsfParser::LT:
                        op = Operator::LT;
                        break;
                    case ezpsfParser::GT:
                        op = Operator::GT;
                        break;
                    case ezpsfParser::LT_EQ:
                        op = Operator::LT_EQ;
                        break;
                    case ezpsfParser::GT_EQ:
                        op = Operator::GT_EQ;
                        break;
                    default:
                        LOG("unreachable state in switching on operator");
                        op = Operator::ERROR_OP;
                }
                return (ExprAst *) new BinaryOpExprAst(left, right, op);
            VISIT_END
        }

        std::any visitBarExpr(ezpsfParser::BarExprContext *ctx) override {
            VISIT_START
                auto expr = std::any_cast<ExprAst *>(visit(ctx->expr()));
                return (ExprAst *) new UnaryOpExprAst(expr, Operator::BAR);
            VISIT_END
        }

        std::any visitBitXorExpr(ezpsfParser::BitXorExprContext *ctx) override {
            VISIT_START
                auto left = std::any_cast<ExprAst *>(visit(ctx->expr(0)));
                auto right = std::any_cast<ExprAst *>(visit(ctx->expr(1)));
                return (ExprAst *) new BinaryOpExprAst(left, right, Operator::BIT_XOR);
            VISIT_END
        }

        std::any visitBitOrExpr(ezpsfParser::BitOrExprContext *ctx) override {
            VISIT_START
                auto left = std::any_cast<ExprAst *>(visit(ctx->expr(0)));
                auto right = std::any_cast<ExprAst *>(visit(ctx->expr(1)));
                return (ExprAst *) new BinaryOpExprAst(left, right, Operator::BIT_OR);
            VISIT_END
        }

        std::any visitBitAndExpr(ezpsfParser::BitAndExprContext *ctx) override {
            VISIT_START
                auto left = std::any_cast<ExprAst *>(visit(ctx->expr(0)));
                auto right = std::any_cast<ExprAst *>(visit(ctx->expr(1)));
                return (ExprAst *) new BinaryOpExprAst(left, right, Operator::BIT_AND);
            VISIT_END
        }

        std::any visitOrExpr(ezpsfParser::OrExprContext *ctx) override {
            VISIT_START
                auto left = std::any_cast<ExprAst *>(visit(ctx->expr(0)));
                auto right = std::any_cast<ExprAst *>(visit(ctx->expr(1)));
                return (ExprAst *) new ShortCircuitOpExprAst(left, right, Operator::OR);
            VISIT_END
        }

        std::any visitAndExpr(ezpsfParser::AndExprContext *ctx) override {
            VISIT_START
                auto left = std::any_cast<ExprAst *>(visit(ctx->expr(0)));
                auto right = std::any_cast<ExprAst *>(visit(ctx->expr(1)));
                return (ExprAst *) new ShortCircuitOpExprAst(left, right, Operator::AND);
            VISIT_END
        }

        /////////////////////////////////////////////////////
        // Filler Expressions
        /////////////////////////////////////////////////////

        std::any visitIdExpr(ezpsfParser::IdExprContext *ctx) override {
            return visitAnyId(ctx->anyId());
        }

        std::any visitLitExpr(ezpsfParser::LitExprContext *ctx) override {
            return visitLiteral(ctx->literal());
        }

        std::any visitParenExpr(ezpsfParser::ParenExprContext *ctx) override {
            return visit(ctx->expr());
        }

        /////////////////////////////////////////////////////
        // Literals
        /////////////////////////////////////////////////////

        std::any visitAnyId(ezpsfParser::AnyIdContext *ctx) override {
            return (ExprAst *) new JsonRefAst(ctx->JSON()->getText(), std::any_cast<DataType>(visitType(ctx->type())));
        }

        std::any visitLiteral(ezpsfParser::LiteralContext *ctx) override {
            VISIT_START
                if (ctx->INT() != nullptr)
                    return (ExprAst *) new LitInt32Ast(std::stoi(ctx->getText())); // TODO use Int64 if too long
                else if (ctx->FLOAT() != nullptr)
                    return (ExprAst *) new LitDoubleAst(std::stod(ctx->getText()));
                else if (ctx->STRING() != nullptr) {
                    std::string text = ctx->getText();
                    return (ExprAst *) new LitStringAst(text.substr(1, text.length() - 1));
                } else if (ctx->TRUE() != nullptr)
                    return (ExprAst *) new LitBoolAst(true);
                else if (ctx->FALSE() != nullptr)
                    return (ExprAst *) new LitBoolAst(false);
                else if (ctx->LIT_NULL() != nullptr)
                    LOG("NULL has not yet been implemented!");
                LOG_RET("literal not found!");
            VISIT_END
        }

        std::any visitType(ezpsfParser::TypeContext *ctx) override {
            VISIT_START
                if (ctx->STR_T() != nullptr)
                    return DataType::STR_T;
                else if (ctx->BYTE_T() != nullptr)
                    return DataType::INT8_T;
                else if (ctx->INT32_T() != nullptr)
                    return DataType::INT32_T;
                else if (ctx->INT64_T() != nullptr)
                    return DataType::INT64_T;
                else if (ctx->UINT32_T() != nullptr)
                    return DataType::UINT32_T;
                else if (ctx->UINT64_T() != nullptr)
                    return DataType::UINT64_T;
                else if (ctx->FLOAT_T() != nullptr)
                    return DataType::FLOAT_T;
                else if (ctx->DOUBLE_T() != nullptr)
                    return DataType::DOUBLE_T;
                else if (ctx->BOOL_T() != nullptr)
                    return DataType::BOOL_T;

                LOG("DataType not recognized!");
                return DataType::ERROR_T;
            VISIT_END
        }
    };
}