
// Generated from ezpsf.g4 by ANTLR 4.10.1

#pragma once


#include "antlr4-runtime.h"
#include "ezpsfParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by ezpsfParser.
 */
class  ezpsfVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by ezpsfParser.
   */
    virtual std::any visitPsf(ezpsfParser::PsfContext *context) = 0;

    virtual std::any visitCastExpr(ezpsfParser::CastExprContext *context) = 0;

    virtual std::any visitPlusMinusExpr(ezpsfParser::PlusMinusExprContext *context) = 0;

    virtual std::any visitOrExpr(ezpsfParser::OrExprContext *context) = 0;

    virtual std::any visitParenExpr(ezpsfParser::ParenExprContext *context) = 0;

    virtual std::any visitBitXorExpr(ezpsfParser::BitXorExprContext *context) = 0;

    virtual std::any visitCmpExpr(ezpsfParser::CmpExprContext *context) = 0;

    virtual std::any visitIndexExpr(ezpsfParser::IndexExprContext *context) = 0;

    virtual std::any visitBitOrExpr(ezpsfParser::BitOrExprContext *context) = 0;

    virtual std::any visitPreUnaryExpr(ezpsfParser::PreUnaryExprContext *context) = 0;

    virtual std::any visitTernaryExpr(ezpsfParser::TernaryExprContext *context) = 0;

    virtual std::any visitBarExpr(ezpsfParser::BarExprContext *context) = 0;

    virtual std::any visitMultiDivExpr(ezpsfParser::MultiDivExprContext *context) = 0;

    virtual std::any visitBitAndExpr(ezpsfParser::BitAndExprContext *context) = 0;

    virtual std::any visitLitExpr(ezpsfParser::LitExprContext *context) = 0;

    virtual std::any visitIdExpr(ezpsfParser::IdExprContext *context) = 0;

    virtual std::any visitAndExpr(ezpsfParser::AndExprContext *context) = 0;

    virtual std::any visitLiteral(ezpsfParser::LiteralContext *context) = 0;

    virtual std::any visitAnyId(ezpsfParser::AnyIdContext *context) = 0;

    virtual std::any visitType(ezpsfParser::TypeContext *context) = 0;


};

