
// Generated from ezpsf.g4 by ANTLR 4.10.1

#pragma once


#include "antlr4-runtime.h"
#include "ezpsfVisitor.h"


/**
 * This class provides an empty implementation of ezpsfVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  ezpsfBaseVisitor : public ezpsfVisitor {
public:

  virtual std::any visitPsf(ezpsfParser::PsfContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCastExpr(ezpsfParser::CastExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPlusMinusExpr(ezpsfParser::PlusMinusExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitOrExpr(ezpsfParser::OrExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitParenExpr(ezpsfParser::ParenExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBitXorExpr(ezpsfParser::BitXorExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCmpExpr(ezpsfParser::CmpExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitIndexExpr(ezpsfParser::IndexExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBitOrExpr(ezpsfParser::BitOrExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPreUnaryExpr(ezpsfParser::PreUnaryExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTernaryExpr(ezpsfParser::TernaryExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBarExpr(ezpsfParser::BarExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMultiDivExpr(ezpsfParser::MultiDivExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBitAndExpr(ezpsfParser::BitAndExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLitExpr(ezpsfParser::LitExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitIdExpr(ezpsfParser::IdExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAndExpr(ezpsfParser::AndExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLiteral(ezpsfParser::LiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAnyId(ezpsfParser::AnyIdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitType(ezpsfParser::TypeContext *ctx) override {
    return visitChildren(ctx);
  }


};

