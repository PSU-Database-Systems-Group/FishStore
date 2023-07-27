
// Generated from ezpsf.g4 by ANTLR 4.10.1


#include "ezpsfVisitor.h"

#include "ezpsfParser.h"


using namespace antlrcpp;

using namespace antlr4;

namespace {

struct EzpsfParserStaticData final {
  EzpsfParserStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  EzpsfParserStaticData(const EzpsfParserStaticData&) = delete;
  EzpsfParserStaticData(EzpsfParserStaticData&&) = delete;
  EzpsfParserStaticData& operator=(const EzpsfParserStaticData&) = delete;
  EzpsfParserStaticData& operator=(EzpsfParserStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

std::once_flag ezpsfParserOnceFlag;
EzpsfParserStaticData *ezpsfParserStaticData = nullptr;

void ezpsfParserInitialize() {
  assert(ezpsfParserStaticData == nullptr);
  auto staticData = std::make_unique<EzpsfParserStaticData>(
    std::vector<std::string>{
      "psf", "expr", "literal", "anyId", "type"
    },
    std::vector<std::string>{
      "", "'.'", "','", "';'", "':'", "'=>'", "'\\u003F'", "", "", "", "", 
      "'UInt32'", "'UInt64'", "'Float'", "'Double'", "'Bool'", "'default'", 
      "'return'", "'if'", "'elif'", "'else'", "'while'", "'for'", "'break'", 
      "'TRUE'", "'FALSE'", "'NULL'", "", "", "", "", "", "", "", "'{'", 
      "'}'", "'['", "']'", "'('", "')'", "'|'", "'+'", "'-'", "'*'", "'/'", 
      "'//'", "'%'", "'='", "'<'", "'<='", "'>'", "'>='", "'=='", "'!='", 
      "'&'", "'^'", "'&&'", "'||'", "'!'"
    },
    std::vector<std::string>{
      "", "PERIOD", "COMMA", "SEMICOLON", "COLON", "ARROW", "QUESTION", 
      "STR_T", "BYTE_T", "INT32_T", "INT64_T", "UINT32_T", "UINT64_T", "FLOAT_T", 
      "DOUBLE_T", "BOOL_T", "DEFAULT", "RETURN", "IF", "ELIF", "ELSE", "WHILE", 
      "FOR", "BREAK", "TRUE", "FALSE", "LIT_NULL", "JSON", "INT", "FLOAT", 
      "STRING", "COMMENT", "BLOCK_COMMENT", "WHITESPACE", "OPEN_BRACE", 
      "CLOSE_BRACE", "OPEN_SQUARE", "CLOSE_SQUARE", "OPEN_PAREN", "CLOSE_PAREN", 
      "BAR", "PLUS", "MINUS", "MULTI", "FLOAT_DIV", "INT_DIV", "MOD", "EQUALS", 
      "LT", "LT_EQ", "GT", "GT_EQ", "EQ", "NEQ", "BIT_AND", "BIT_XOR", "AND", 
      "OR", "NOT"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,58,92,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,1,0,1,0,3,0,13,8,0,
  	1,0,1,0,1,0,1,0,1,0,1,0,3,0,21,8,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  	1,1,1,1,1,1,1,1,1,3,1,36,8,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5,1,78,8,1,10,1,12,
  	1,81,9,1,1,2,1,2,1,3,1,3,1,3,1,3,1,3,1,4,1,4,1,4,0,1,2,5,0,2,4,6,8,0,
  	6,2,0,42,42,58,58,1,0,43,46,1,0,41,42,1,0,48,53,2,0,24,26,28,30,1,0,7,
  	15,103,0,12,1,0,0,0,2,35,1,0,0,0,4,82,1,0,0,0,6,84,1,0,0,0,8,89,1,0,0,
  	0,10,11,5,27,0,0,11,13,5,5,0,0,12,10,1,0,0,0,12,13,1,0,0,0,13,14,1,0,
  	0,0,14,20,3,2,1,0,15,16,5,16,0,0,16,17,5,38,0,0,17,18,3,2,1,0,18,19,5,
  	39,0,0,19,21,1,0,0,0,20,15,1,0,0,0,20,21,1,0,0,0,21,1,1,0,0,0,22,23,6,
  	1,-1,0,23,36,3,6,3,0,24,36,3,4,2,0,25,26,5,38,0,0,26,27,3,2,1,0,27,28,
  	5,39,0,0,28,36,1,0,0,0,29,30,5,40,0,0,30,31,3,2,1,0,31,32,5,40,0,0,32,
  	36,1,0,0,0,33,34,7,0,0,0,34,36,3,2,1,10,35,22,1,0,0,0,35,24,1,0,0,0,35,
  	25,1,0,0,0,35,29,1,0,0,0,35,33,1,0,0,0,36,79,1,0,0,0,37,38,10,9,0,0,38,
  	39,7,1,0,0,39,78,3,2,1,10,40,41,10,8,0,0,41,42,7,2,0,0,42,78,3,2,1,9,
  	43,44,10,7,0,0,44,45,7,3,0,0,45,78,3,2,1,8,46,47,10,6,0,0,47,48,5,54,
  	0,0,48,78,3,2,1,7,49,50,10,5,0,0,50,51,5,55,0,0,51,78,3,2,1,6,52,53,10,
  	4,0,0,53,54,5,40,0,0,54,78,3,2,1,5,55,56,10,3,0,0,56,57,5,56,0,0,57,78,
  	3,2,1,4,58,59,10,2,0,0,59,60,5,57,0,0,60,78,3,2,1,3,61,62,10,1,0,0,62,
  	63,5,6,0,0,63,64,3,2,1,0,64,65,5,4,0,0,65,66,3,2,1,1,66,78,1,0,0,0,67,
  	68,10,12,0,0,68,69,5,36,0,0,69,70,3,8,4,0,70,71,5,37,0,0,71,78,1,0,0,
  	0,72,73,10,11,0,0,73,74,5,36,0,0,74,75,3,2,1,0,75,76,5,37,0,0,76,78,1,
  	0,0,0,77,37,1,0,0,0,77,40,1,0,0,0,77,43,1,0,0,0,77,46,1,0,0,0,77,49,1,
  	0,0,0,77,52,1,0,0,0,77,55,1,0,0,0,77,58,1,0,0,0,77,61,1,0,0,0,77,67,1,
  	0,0,0,77,72,1,0,0,0,78,81,1,0,0,0,79,77,1,0,0,0,79,80,1,0,0,0,80,3,1,
  	0,0,0,81,79,1,0,0,0,82,83,7,4,0,0,83,5,1,0,0,0,84,85,5,38,0,0,85,86,3,
  	8,4,0,86,87,5,39,0,0,87,88,5,27,0,0,88,7,1,0,0,0,89,90,7,5,0,0,90,9,1,
  	0,0,0,5,12,20,35,77,79
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  ezpsfParserStaticData = staticData.release();
}

}

ezpsfParser::ezpsfParser(TokenStream *input) : ezpsfParser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

ezpsfParser::ezpsfParser(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  ezpsfParser::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *ezpsfParserStaticData->atn, ezpsfParserStaticData->decisionToDFA, ezpsfParserStaticData->sharedContextCache, options);
}

ezpsfParser::~ezpsfParser() {
  delete _interpreter;
}

const atn::ATN& ezpsfParser::getATN() const {
  return *ezpsfParserStaticData->atn;
}

std::string ezpsfParser::getGrammarFileName() const {
  return "ezpsf.g4";
}

const std::vector<std::string>& ezpsfParser::getRuleNames() const {
  return ezpsfParserStaticData->ruleNames;
}

const dfa::Vocabulary& ezpsfParser::getVocabulary() const {
  return ezpsfParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView ezpsfParser::getSerializedATN() const {
  return ezpsfParserStaticData->serializedATN;
}


//----------------- PsfContext ------------------------------------------------------------------

ezpsfParser::PsfContext::PsfContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ezpsfParser::ExprContext *> ezpsfParser::PsfContext::expr() {
  return getRuleContexts<ezpsfParser::ExprContext>();
}

ezpsfParser::ExprContext* ezpsfParser::PsfContext::expr(size_t i) {
  return getRuleContext<ezpsfParser::ExprContext>(i);
}

tree::TerminalNode* ezpsfParser::PsfContext::JSON() {
  return getToken(ezpsfParser::JSON, 0);
}

tree::TerminalNode* ezpsfParser::PsfContext::ARROW() {
  return getToken(ezpsfParser::ARROW, 0);
}

tree::TerminalNode* ezpsfParser::PsfContext::DEFAULT() {
  return getToken(ezpsfParser::DEFAULT, 0);
}

tree::TerminalNode* ezpsfParser::PsfContext::OPEN_PAREN() {
  return getToken(ezpsfParser::OPEN_PAREN, 0);
}

tree::TerminalNode* ezpsfParser::PsfContext::CLOSE_PAREN() {
  return getToken(ezpsfParser::CLOSE_PAREN, 0);
}


size_t ezpsfParser::PsfContext::getRuleIndex() const {
  return ezpsfParser::RulePsf;
}


std::any ezpsfParser::PsfContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ezpsfVisitor*>(visitor))
    return parserVisitor->visitPsf(this);
  else
    return visitor->visitChildren(this);
}

ezpsfParser::PsfContext* ezpsfParser::psf() {
  PsfContext *_localctx = _tracker.createInstance<PsfContext>(_ctx, getState());
  enterRule(_localctx, 0, ezpsfParser::RulePsf);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(12);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ezpsfParser::JSON) {
      setState(10);
      match(ezpsfParser::JSON);
      setState(11);
      match(ezpsfParser::ARROW);
    }
    setState(14);
    expr(0);
    setState(20);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ezpsfParser::DEFAULT) {
      setState(15);
      match(ezpsfParser::DEFAULT);
      setState(16);
      match(ezpsfParser::OPEN_PAREN);
      setState(17);
      expr(0);
      setState(18);
      match(ezpsfParser::CLOSE_PAREN);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExprContext ------------------------------------------------------------------

ezpsfParser::ExprContext::ExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t ezpsfParser::ExprContext::getRuleIndex() const {
  return ezpsfParser::RuleExpr;
}

void ezpsfParser::ExprContext::copyFrom(ExprContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- CastExprContext ------------------------------------------------------------------

ezpsfParser::ExprContext* ezpsfParser::CastExprContext::expr() {
  return getRuleContext<ezpsfParser::ExprContext>(0);
}

tree::TerminalNode* ezpsfParser::CastExprContext::OPEN_SQUARE() {
  return getToken(ezpsfParser::OPEN_SQUARE, 0);
}

ezpsfParser::TypeContext* ezpsfParser::CastExprContext::type() {
  return getRuleContext<ezpsfParser::TypeContext>(0);
}

tree::TerminalNode* ezpsfParser::CastExprContext::CLOSE_SQUARE() {
  return getToken(ezpsfParser::CLOSE_SQUARE, 0);
}

ezpsfParser::CastExprContext::CastExprContext(ExprContext *ctx) { copyFrom(ctx); }


std::any ezpsfParser::CastExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ezpsfVisitor*>(visitor))
    return parserVisitor->visitCastExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- PlusMinusExprContext ------------------------------------------------------------------

std::vector<ezpsfParser::ExprContext *> ezpsfParser::PlusMinusExprContext::expr() {
  return getRuleContexts<ezpsfParser::ExprContext>();
}

ezpsfParser::ExprContext* ezpsfParser::PlusMinusExprContext::expr(size_t i) {
  return getRuleContext<ezpsfParser::ExprContext>(i);
}

tree::TerminalNode* ezpsfParser::PlusMinusExprContext::PLUS() {
  return getToken(ezpsfParser::PLUS, 0);
}

tree::TerminalNode* ezpsfParser::PlusMinusExprContext::MINUS() {
  return getToken(ezpsfParser::MINUS, 0);
}

ezpsfParser::PlusMinusExprContext::PlusMinusExprContext(ExprContext *ctx) { copyFrom(ctx); }


std::any ezpsfParser::PlusMinusExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ezpsfVisitor*>(visitor))
    return parserVisitor->visitPlusMinusExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- OrExprContext ------------------------------------------------------------------

std::vector<ezpsfParser::ExprContext *> ezpsfParser::OrExprContext::expr() {
  return getRuleContexts<ezpsfParser::ExprContext>();
}

ezpsfParser::ExprContext* ezpsfParser::OrExprContext::expr(size_t i) {
  return getRuleContext<ezpsfParser::ExprContext>(i);
}

tree::TerminalNode* ezpsfParser::OrExprContext::OR() {
  return getToken(ezpsfParser::OR, 0);
}

ezpsfParser::OrExprContext::OrExprContext(ExprContext *ctx) { copyFrom(ctx); }


std::any ezpsfParser::OrExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ezpsfVisitor*>(visitor))
    return parserVisitor->visitOrExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ParenExprContext ------------------------------------------------------------------

tree::TerminalNode* ezpsfParser::ParenExprContext::OPEN_PAREN() {
  return getToken(ezpsfParser::OPEN_PAREN, 0);
}

ezpsfParser::ExprContext* ezpsfParser::ParenExprContext::expr() {
  return getRuleContext<ezpsfParser::ExprContext>(0);
}

tree::TerminalNode* ezpsfParser::ParenExprContext::CLOSE_PAREN() {
  return getToken(ezpsfParser::CLOSE_PAREN, 0);
}

ezpsfParser::ParenExprContext::ParenExprContext(ExprContext *ctx) { copyFrom(ctx); }


std::any ezpsfParser::ParenExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ezpsfVisitor*>(visitor))
    return parserVisitor->visitParenExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- BitXorExprContext ------------------------------------------------------------------

std::vector<ezpsfParser::ExprContext *> ezpsfParser::BitXorExprContext::expr() {
  return getRuleContexts<ezpsfParser::ExprContext>();
}

ezpsfParser::ExprContext* ezpsfParser::BitXorExprContext::expr(size_t i) {
  return getRuleContext<ezpsfParser::ExprContext>(i);
}

tree::TerminalNode* ezpsfParser::BitXorExprContext::BIT_XOR() {
  return getToken(ezpsfParser::BIT_XOR, 0);
}

ezpsfParser::BitXorExprContext::BitXorExprContext(ExprContext *ctx) { copyFrom(ctx); }


std::any ezpsfParser::BitXorExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ezpsfVisitor*>(visitor))
    return parserVisitor->visitBitXorExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- CmpExprContext ------------------------------------------------------------------

std::vector<ezpsfParser::ExprContext *> ezpsfParser::CmpExprContext::expr() {
  return getRuleContexts<ezpsfParser::ExprContext>();
}

ezpsfParser::ExprContext* ezpsfParser::CmpExprContext::expr(size_t i) {
  return getRuleContext<ezpsfParser::ExprContext>(i);
}

tree::TerminalNode* ezpsfParser::CmpExprContext::EQ() {
  return getToken(ezpsfParser::EQ, 0);
}

tree::TerminalNode* ezpsfParser::CmpExprContext::NEQ() {
  return getToken(ezpsfParser::NEQ, 0);
}

tree::TerminalNode* ezpsfParser::CmpExprContext::LT() {
  return getToken(ezpsfParser::LT, 0);
}

tree::TerminalNode* ezpsfParser::CmpExprContext::GT() {
  return getToken(ezpsfParser::GT, 0);
}

tree::TerminalNode* ezpsfParser::CmpExprContext::LT_EQ() {
  return getToken(ezpsfParser::LT_EQ, 0);
}

tree::TerminalNode* ezpsfParser::CmpExprContext::GT_EQ() {
  return getToken(ezpsfParser::GT_EQ, 0);
}

ezpsfParser::CmpExprContext::CmpExprContext(ExprContext *ctx) { copyFrom(ctx); }


std::any ezpsfParser::CmpExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ezpsfVisitor*>(visitor))
    return parserVisitor->visitCmpExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- IndexExprContext ------------------------------------------------------------------

std::vector<ezpsfParser::ExprContext *> ezpsfParser::IndexExprContext::expr() {
  return getRuleContexts<ezpsfParser::ExprContext>();
}

ezpsfParser::ExprContext* ezpsfParser::IndexExprContext::expr(size_t i) {
  return getRuleContext<ezpsfParser::ExprContext>(i);
}

tree::TerminalNode* ezpsfParser::IndexExprContext::OPEN_SQUARE() {
  return getToken(ezpsfParser::OPEN_SQUARE, 0);
}

tree::TerminalNode* ezpsfParser::IndexExprContext::CLOSE_SQUARE() {
  return getToken(ezpsfParser::CLOSE_SQUARE, 0);
}

ezpsfParser::IndexExprContext::IndexExprContext(ExprContext *ctx) { copyFrom(ctx); }


std::any ezpsfParser::IndexExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ezpsfVisitor*>(visitor))
    return parserVisitor->visitIndexExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- BitOrExprContext ------------------------------------------------------------------

std::vector<ezpsfParser::ExprContext *> ezpsfParser::BitOrExprContext::expr() {
  return getRuleContexts<ezpsfParser::ExprContext>();
}

ezpsfParser::ExprContext* ezpsfParser::BitOrExprContext::expr(size_t i) {
  return getRuleContext<ezpsfParser::ExprContext>(i);
}

tree::TerminalNode* ezpsfParser::BitOrExprContext::BAR() {
  return getToken(ezpsfParser::BAR, 0);
}

ezpsfParser::BitOrExprContext::BitOrExprContext(ExprContext *ctx) { copyFrom(ctx); }


std::any ezpsfParser::BitOrExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ezpsfVisitor*>(visitor))
    return parserVisitor->visitBitOrExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- PreUnaryExprContext ------------------------------------------------------------------

ezpsfParser::ExprContext* ezpsfParser::PreUnaryExprContext::expr() {
  return getRuleContext<ezpsfParser::ExprContext>(0);
}

tree::TerminalNode* ezpsfParser::PreUnaryExprContext::MINUS() {
  return getToken(ezpsfParser::MINUS, 0);
}

tree::TerminalNode* ezpsfParser::PreUnaryExprContext::NOT() {
  return getToken(ezpsfParser::NOT, 0);
}

ezpsfParser::PreUnaryExprContext::PreUnaryExprContext(ExprContext *ctx) { copyFrom(ctx); }


std::any ezpsfParser::PreUnaryExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ezpsfVisitor*>(visitor))
    return parserVisitor->visitPreUnaryExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- TernaryExprContext ------------------------------------------------------------------

std::vector<ezpsfParser::ExprContext *> ezpsfParser::TernaryExprContext::expr() {
  return getRuleContexts<ezpsfParser::ExprContext>();
}

ezpsfParser::ExprContext* ezpsfParser::TernaryExprContext::expr(size_t i) {
  return getRuleContext<ezpsfParser::ExprContext>(i);
}

tree::TerminalNode* ezpsfParser::TernaryExprContext::QUESTION() {
  return getToken(ezpsfParser::QUESTION, 0);
}

tree::TerminalNode* ezpsfParser::TernaryExprContext::COLON() {
  return getToken(ezpsfParser::COLON, 0);
}

ezpsfParser::TernaryExprContext::TernaryExprContext(ExprContext *ctx) { copyFrom(ctx); }


std::any ezpsfParser::TernaryExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ezpsfVisitor*>(visitor))
    return parserVisitor->visitTernaryExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- BarExprContext ------------------------------------------------------------------

std::vector<tree::TerminalNode *> ezpsfParser::BarExprContext::BAR() {
  return getTokens(ezpsfParser::BAR);
}

tree::TerminalNode* ezpsfParser::BarExprContext::BAR(size_t i) {
  return getToken(ezpsfParser::BAR, i);
}

ezpsfParser::ExprContext* ezpsfParser::BarExprContext::expr() {
  return getRuleContext<ezpsfParser::ExprContext>(0);
}

ezpsfParser::BarExprContext::BarExprContext(ExprContext *ctx) { copyFrom(ctx); }


std::any ezpsfParser::BarExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ezpsfVisitor*>(visitor))
    return parserVisitor->visitBarExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- MultiDivExprContext ------------------------------------------------------------------

std::vector<ezpsfParser::ExprContext *> ezpsfParser::MultiDivExprContext::expr() {
  return getRuleContexts<ezpsfParser::ExprContext>();
}

ezpsfParser::ExprContext* ezpsfParser::MultiDivExprContext::expr(size_t i) {
  return getRuleContext<ezpsfParser::ExprContext>(i);
}

tree::TerminalNode* ezpsfParser::MultiDivExprContext::MULTI() {
  return getToken(ezpsfParser::MULTI, 0);
}

tree::TerminalNode* ezpsfParser::MultiDivExprContext::FLOAT_DIV() {
  return getToken(ezpsfParser::FLOAT_DIV, 0);
}

tree::TerminalNode* ezpsfParser::MultiDivExprContext::INT_DIV() {
  return getToken(ezpsfParser::INT_DIV, 0);
}

tree::TerminalNode* ezpsfParser::MultiDivExprContext::MOD() {
  return getToken(ezpsfParser::MOD, 0);
}

ezpsfParser::MultiDivExprContext::MultiDivExprContext(ExprContext *ctx) { copyFrom(ctx); }


std::any ezpsfParser::MultiDivExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ezpsfVisitor*>(visitor))
    return parserVisitor->visitMultiDivExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- BitAndExprContext ------------------------------------------------------------------

std::vector<ezpsfParser::ExprContext *> ezpsfParser::BitAndExprContext::expr() {
  return getRuleContexts<ezpsfParser::ExprContext>();
}

ezpsfParser::ExprContext* ezpsfParser::BitAndExprContext::expr(size_t i) {
  return getRuleContext<ezpsfParser::ExprContext>(i);
}

tree::TerminalNode* ezpsfParser::BitAndExprContext::BIT_AND() {
  return getToken(ezpsfParser::BIT_AND, 0);
}

ezpsfParser::BitAndExprContext::BitAndExprContext(ExprContext *ctx) { copyFrom(ctx); }


std::any ezpsfParser::BitAndExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ezpsfVisitor*>(visitor))
    return parserVisitor->visitBitAndExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- LitExprContext ------------------------------------------------------------------

ezpsfParser::LiteralContext* ezpsfParser::LitExprContext::literal() {
  return getRuleContext<ezpsfParser::LiteralContext>(0);
}

ezpsfParser::LitExprContext::LitExprContext(ExprContext *ctx) { copyFrom(ctx); }


std::any ezpsfParser::LitExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ezpsfVisitor*>(visitor))
    return parserVisitor->visitLitExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- IdExprContext ------------------------------------------------------------------

ezpsfParser::AnyIdContext* ezpsfParser::IdExprContext::anyId() {
  return getRuleContext<ezpsfParser::AnyIdContext>(0);
}

ezpsfParser::IdExprContext::IdExprContext(ExprContext *ctx) { copyFrom(ctx); }


std::any ezpsfParser::IdExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ezpsfVisitor*>(visitor))
    return parserVisitor->visitIdExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- AndExprContext ------------------------------------------------------------------

std::vector<ezpsfParser::ExprContext *> ezpsfParser::AndExprContext::expr() {
  return getRuleContexts<ezpsfParser::ExprContext>();
}

ezpsfParser::ExprContext* ezpsfParser::AndExprContext::expr(size_t i) {
  return getRuleContext<ezpsfParser::ExprContext>(i);
}

tree::TerminalNode* ezpsfParser::AndExprContext::AND() {
  return getToken(ezpsfParser::AND, 0);
}

ezpsfParser::AndExprContext::AndExprContext(ExprContext *ctx) { copyFrom(ctx); }


std::any ezpsfParser::AndExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ezpsfVisitor*>(visitor))
    return parserVisitor->visitAndExpr(this);
  else
    return visitor->visitChildren(this);
}

ezpsfParser::ExprContext* ezpsfParser::expr() {
   return expr(0);
}

ezpsfParser::ExprContext* ezpsfParser::expr(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  ezpsfParser::ExprContext *_localctx = _tracker.createInstance<ExprContext>(_ctx, parentState);
  ezpsfParser::ExprContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 2;
  enterRecursionRule(_localctx, 2, ezpsfParser::RuleExpr, precedence);

    size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(35);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 2, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<IdExprContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;

      setState(23);
      anyId();
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<LitExprContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(24);
      literal();
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<ParenExprContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(25);
      match(ezpsfParser::OPEN_PAREN);
      setState(26);
      expr(0);
      setState(27);
      match(ezpsfParser::CLOSE_PAREN);
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<BarExprContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(29);
      match(ezpsfParser::BAR);
      setState(30);
      expr(0);
      setState(31);
      match(ezpsfParser::BAR);
      break;
    }

    case 5: {
      _localctx = _tracker.createInstance<PreUnaryExprContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(33);
      antlrcpp::downCast<PreUnaryExprContext *>(_localctx)->op = _input->LT(1);
      _la = _input->LA(1);
      if (!(_la == ezpsfParser::MINUS

      || _la == ezpsfParser::NOT)) {
        antlrcpp::downCast<PreUnaryExprContext *>(_localctx)->op = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(34);
      expr(10);
      break;
    }

    default:
      break;
    }
    _ctx->stop = _input->LT(-1);
    setState(79);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 4, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(77);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 3, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<MultiDivExprContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(37);

          if (!(precpred(_ctx, 9))) throw FailedPredicateException(this, "precpred(_ctx, 9)");
          setState(38);
          antlrcpp::downCast<MultiDivExprContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & ((1ULL << ezpsfParser::MULTI)
            | (1ULL << ezpsfParser::FLOAT_DIV)
            | (1ULL << ezpsfParser::INT_DIV)
            | (1ULL << ezpsfParser::MOD))) != 0))) {
            antlrcpp::downCast<MultiDivExprContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(39);
          expr(10);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<PlusMinusExprContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(40);

          if (!(precpred(_ctx, 8))) throw FailedPredicateException(this, "precpred(_ctx, 8)");
          setState(41);
          antlrcpp::downCast<PlusMinusExprContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!(_la == ezpsfParser::PLUS

          || _la == ezpsfParser::MINUS)) {
            antlrcpp::downCast<PlusMinusExprContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(42);
          expr(9);
          break;
        }

        case 3: {
          auto newContext = _tracker.createInstance<CmpExprContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(43);

          if (!(precpred(_ctx, 7))) throw FailedPredicateException(this, "precpred(_ctx, 7)");
          setState(44);
          antlrcpp::downCast<CmpExprContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & ((1ULL << ezpsfParser::LT)
            | (1ULL << ezpsfParser::LT_EQ)
            | (1ULL << ezpsfParser::GT)
            | (1ULL << ezpsfParser::GT_EQ)
            | (1ULL << ezpsfParser::EQ)
            | (1ULL << ezpsfParser::NEQ))) != 0))) {
            antlrcpp::downCast<CmpExprContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(45);
          expr(8);
          break;
        }

        case 4: {
          auto newContext = _tracker.createInstance<BitAndExprContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(46);

          if (!(precpred(_ctx, 6))) throw FailedPredicateException(this, "precpred(_ctx, 6)");
          setState(47);
          match(ezpsfParser::BIT_AND);
          setState(48);
          expr(7);
          break;
        }

        case 5: {
          auto newContext = _tracker.createInstance<BitXorExprContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(49);

          if (!(precpred(_ctx, 5))) throw FailedPredicateException(this, "precpred(_ctx, 5)");
          setState(50);
          match(ezpsfParser::BIT_XOR);
          setState(51);
          expr(6);
          break;
        }

        case 6: {
          auto newContext = _tracker.createInstance<BitOrExprContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(52);

          if (!(precpred(_ctx, 4))) throw FailedPredicateException(this, "precpred(_ctx, 4)");
          setState(53);
          match(ezpsfParser::BAR);
          setState(54);
          expr(5);
          break;
        }

        case 7: {
          auto newContext = _tracker.createInstance<AndExprContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(55);

          if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
          setState(56);
          match(ezpsfParser::AND);
          setState(57);
          expr(4);
          break;
        }

        case 8: {
          auto newContext = _tracker.createInstance<OrExprContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(58);

          if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
          setState(59);
          match(ezpsfParser::OR);
          setState(60);
          expr(3);
          break;
        }

        case 9: {
          auto newContext = _tracker.createInstance<TernaryExprContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(61);

          if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
          setState(62);
          match(ezpsfParser::QUESTION);
          setState(63);
          expr(0);
          setState(64);
          match(ezpsfParser::COLON);
          setState(65);
          expr(1);
          break;
        }

        case 10: {
          auto newContext = _tracker.createInstance<CastExprContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(67);

          if (!(precpred(_ctx, 12))) throw FailedPredicateException(this, "precpred(_ctx, 12)");
          setState(68);
          match(ezpsfParser::OPEN_SQUARE);
          setState(69);
          type();
          setState(70);
          match(ezpsfParser::CLOSE_SQUARE);
          break;
        }

        case 11: {
          auto newContext = _tracker.createInstance<IndexExprContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(72);

          if (!(precpred(_ctx, 11))) throw FailedPredicateException(this, "precpred(_ctx, 11)");
          setState(73);
          match(ezpsfParser::OPEN_SQUARE);
          setState(74);
          expr(0);
          setState(75);
          match(ezpsfParser::CLOSE_SQUARE);
          break;
        }

        default:
          break;
        } 
      }
      setState(81);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 4, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- LiteralContext ------------------------------------------------------------------

ezpsfParser::LiteralContext::LiteralContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ezpsfParser::LiteralContext::INT() {
  return getToken(ezpsfParser::INT, 0);
}

tree::TerminalNode* ezpsfParser::LiteralContext::FLOAT() {
  return getToken(ezpsfParser::FLOAT, 0);
}

tree::TerminalNode* ezpsfParser::LiteralContext::STRING() {
  return getToken(ezpsfParser::STRING, 0);
}

tree::TerminalNode* ezpsfParser::LiteralContext::TRUE() {
  return getToken(ezpsfParser::TRUE, 0);
}

tree::TerminalNode* ezpsfParser::LiteralContext::FALSE() {
  return getToken(ezpsfParser::FALSE, 0);
}

tree::TerminalNode* ezpsfParser::LiteralContext::LIT_NULL() {
  return getToken(ezpsfParser::LIT_NULL, 0);
}


size_t ezpsfParser::LiteralContext::getRuleIndex() const {
  return ezpsfParser::RuleLiteral;
}


std::any ezpsfParser::LiteralContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ezpsfVisitor*>(visitor))
    return parserVisitor->visitLiteral(this);
  else
    return visitor->visitChildren(this);
}

ezpsfParser::LiteralContext* ezpsfParser::literal() {
  LiteralContext *_localctx = _tracker.createInstance<LiteralContext>(_ctx, getState());
  enterRule(_localctx, 4, ezpsfParser::RuleLiteral);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(82);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << ezpsfParser::TRUE)
      | (1ULL << ezpsfParser::FALSE)
      | (1ULL << ezpsfParser::LIT_NULL)
      | (1ULL << ezpsfParser::INT)
      | (1ULL << ezpsfParser::FLOAT)
      | (1ULL << ezpsfParser::STRING))) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AnyIdContext ------------------------------------------------------------------

ezpsfParser::AnyIdContext::AnyIdContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ezpsfParser::AnyIdContext::OPEN_PAREN() {
  return getToken(ezpsfParser::OPEN_PAREN, 0);
}

ezpsfParser::TypeContext* ezpsfParser::AnyIdContext::type() {
  return getRuleContext<ezpsfParser::TypeContext>(0);
}

tree::TerminalNode* ezpsfParser::AnyIdContext::CLOSE_PAREN() {
  return getToken(ezpsfParser::CLOSE_PAREN, 0);
}

tree::TerminalNode* ezpsfParser::AnyIdContext::JSON() {
  return getToken(ezpsfParser::JSON, 0);
}


size_t ezpsfParser::AnyIdContext::getRuleIndex() const {
  return ezpsfParser::RuleAnyId;
}


std::any ezpsfParser::AnyIdContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ezpsfVisitor*>(visitor))
    return parserVisitor->visitAnyId(this);
  else
    return visitor->visitChildren(this);
}

ezpsfParser::AnyIdContext* ezpsfParser::anyId() {
  AnyIdContext *_localctx = _tracker.createInstance<AnyIdContext>(_ctx, getState());
  enterRule(_localctx, 6, ezpsfParser::RuleAnyId);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(84);
    match(ezpsfParser::OPEN_PAREN);
    setState(85);
    type();
    setState(86);
    match(ezpsfParser::CLOSE_PAREN);
    setState(87);
    match(ezpsfParser::JSON);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TypeContext ------------------------------------------------------------------

ezpsfParser::TypeContext::TypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ezpsfParser::TypeContext::STR_T() {
  return getToken(ezpsfParser::STR_T, 0);
}

tree::TerminalNode* ezpsfParser::TypeContext::BYTE_T() {
  return getToken(ezpsfParser::BYTE_T, 0);
}

tree::TerminalNode* ezpsfParser::TypeContext::INT32_T() {
  return getToken(ezpsfParser::INT32_T, 0);
}

tree::TerminalNode* ezpsfParser::TypeContext::INT64_T() {
  return getToken(ezpsfParser::INT64_T, 0);
}

tree::TerminalNode* ezpsfParser::TypeContext::UINT32_T() {
  return getToken(ezpsfParser::UINT32_T, 0);
}

tree::TerminalNode* ezpsfParser::TypeContext::UINT64_T() {
  return getToken(ezpsfParser::UINT64_T, 0);
}

tree::TerminalNode* ezpsfParser::TypeContext::FLOAT_T() {
  return getToken(ezpsfParser::FLOAT_T, 0);
}

tree::TerminalNode* ezpsfParser::TypeContext::DOUBLE_T() {
  return getToken(ezpsfParser::DOUBLE_T, 0);
}

tree::TerminalNode* ezpsfParser::TypeContext::BOOL_T() {
  return getToken(ezpsfParser::BOOL_T, 0);
}


size_t ezpsfParser::TypeContext::getRuleIndex() const {
  return ezpsfParser::RuleType;
}


std::any ezpsfParser::TypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ezpsfVisitor*>(visitor))
    return parserVisitor->visitType(this);
  else
    return visitor->visitChildren(this);
}

ezpsfParser::TypeContext* ezpsfParser::type() {
  TypeContext *_localctx = _tracker.createInstance<TypeContext>(_ctx, getState());
  enterRule(_localctx, 8, ezpsfParser::RuleType);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(89);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << ezpsfParser::STR_T)
      | (1ULL << ezpsfParser::BYTE_T)
      | (1ULL << ezpsfParser::INT32_T)
      | (1ULL << ezpsfParser::INT64_T)
      | (1ULL << ezpsfParser::UINT32_T)
      | (1ULL << ezpsfParser::UINT64_T)
      | (1ULL << ezpsfParser::FLOAT_T)
      | (1ULL << ezpsfParser::DOUBLE_T)
      | (1ULL << ezpsfParser::BOOL_T))) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

bool ezpsfParser::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
  switch (ruleIndex) {
    case 1: return exprSempred(antlrcpp::downCast<ExprContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool ezpsfParser::exprSempred(ExprContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 9);
    case 1: return precpred(_ctx, 8);
    case 2: return precpred(_ctx, 7);
    case 3: return precpred(_ctx, 6);
    case 4: return precpred(_ctx, 5);
    case 5: return precpred(_ctx, 4);
    case 6: return precpred(_ctx, 3);
    case 7: return precpred(_ctx, 2);
    case 8: return precpred(_ctx, 1);
    case 9: return precpred(_ctx, 12);
    case 10: return precpred(_ctx, 11);

  default:
    break;
  }
  return true;
}

void ezpsfParser::initialize() {
  std::call_once(ezpsfParserOnceFlag, ezpsfParserInitialize);
}
