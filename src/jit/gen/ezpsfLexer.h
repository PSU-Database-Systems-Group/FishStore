
// Generated from ezpsf.g4 by ANTLR 4.10.1

#pragma once


#include "antlr4-runtime.h"




class  ezpsfLexer : public antlr4::Lexer {
public:
  enum {
    PERIOD = 1, COMMA = 2, SEMICOLON = 3, COLON = 4, ARROW = 5, QUESTION = 6, 
    STR_T = 7, BYTE_T = 8, INT32_T = 9, INT64_T = 10, UINT32_T = 11, UINT64_T = 12, 
    FLOAT_T = 13, DOUBLE_T = 14, BOOL_T = 15, DEFAULT = 16, RETURN = 17, 
    IF = 18, ELIF = 19, ELSE = 20, WHILE = 21, FOR = 22, BREAK = 23, TRUE = 24, 
    FALSE = 25, LIT_NULL = 26, JSON = 27, INT = 28, FLOAT = 29, STRING = 30, 
    COMMENT = 31, BLOCK_COMMENT = 32, WHITESPACE = 33, OPEN_BRACE = 34, 
    CLOSE_BRACE = 35, OPEN_SQUARE = 36, CLOSE_SQUARE = 37, OPEN_PAREN = 38, 
    CLOSE_PAREN = 39, BAR = 40, PLUS = 41, MINUS = 42, MULTI = 43, FLOAT_DIV = 44, 
    INT_DIV = 45, MOD = 46, EQUALS = 47, LT = 48, LT_EQ = 49, GT = 50, GT_EQ = 51, 
    EQ = 52, NEQ = 53, BIT_AND = 54, BIT_XOR = 55, AND = 56, OR = 57, NOT = 58
  };

  explicit ezpsfLexer(antlr4::CharStream *input);

  ~ezpsfLexer() override;


  std::string getGrammarFileName() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const std::vector<std::string>& getChannelNames() const override;

  const std::vector<std::string>& getModeNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;

  const antlr4::atn::ATN& getATN() const override;

  // By default the static state used to implement the lexer is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:

  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

};

