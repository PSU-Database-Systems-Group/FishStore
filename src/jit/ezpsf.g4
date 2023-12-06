grammar ezpsf;
// ANTLR grammer file
// to create the generated files (gen/*) call this with antlr4 Version 4.10.1
// antlr4 -visitor -no-listener -o gen -Dlanguage=Cpp ezpsf.g4

// single line psfs
psf
    : (JSON ARROW)? expr (DEFAULT OPEN_PAREN expr CLOSE_PAREN)?
    ;

expr
    : anyId                                                 # idExpr
    | literal                                               # litExpr
    | OPEN_PAREN expr CLOSE_PAREN                           # parenExpr
    | BAR expr BAR                                          # barExpr
    | expr OPEN_SQUARE type CLOSE_SQUARE                    # castExpr
    | expr OPEN_SQUARE expr CLOSE_SQUARE                    # indexExpr
    | op = (MINUS | NOT) expr                               # preUnaryExpr
    | expr op=(MULTI | FLOAT_DIV | INT_DIV | MOD) expr      # multiDivExpr
    | expr op=(PLUS | MINUS) expr                           # plusMinusExpr
    | expr op=(EQ | NEQ | LT | GT | LT_EQ | GT_EQ) expr     # cmpExpr
    | expr BIT_AND expr                                     # bitAndExpr
    | expr BIT_XOR expr                                     # bitXorExpr
    | expr BAR expr                                         # bitOrExpr
    | expr AND expr                                         # andExpr
    | expr OR expr                                          # orExpr
    | <assoc=right> expr QUESTION expr COLON expr           # ternaryExpr
    ;

literal
    : INT
    | FLOAT
    | STRING
    | TRUE
    | FALSE
    | LIT_NULL
    ;

anyId
    : OPEN_PAREN type CLOSE_PAREN JSON
    ;

type
    : STR_T
    | BYTE_T
    | INT32_T
    | INT64_T
    | UINT32_T
    | UINT64_T
    | FLOAT_T
    | DOUBLE_T
    | BOOL_T
    ;


////////////////////////////////////////////////////////////////////////////////////////////////////////
// Tokens
////////////////////////////////////////////////////////////////////////////////////////////////////////

PERIOD		: '.';
COMMA		: ',';
SEMICOLON	: ';';
COLON	    : ':';
ARROW       : '=>';
QUESTION    : '?';


// TYPES
STR_T		: 'Str' | 'String';
BYTE_T		: 'Byte'  | 'Int8';
INT32_T		: 'Int32' | 'Int';
INT64_T		: 'Int64' | 'Long';

UINT32_T	: 'UInt32';
UINT64_T    : 'UInt64';

FLOAT_T     : 'Float';
DOUBLE_T	: 'Double';
BOOL_T		: 'Bool';


// keyword
DEFAULT     : 'default';


RETURN		: 'return';

IF		    : 'if';
ELIF		: 'elif';
ELSE		: 'else';
WHILE		: 'while';
FOR		    : 'for';
BREAK		: 'break';

TRUE		: 'TRUE';
FALSE		: 'FALSE';
LIT_NULL	: 'NULL';

JSON	    : [a-zA-Z_]([a-zA-Z0-9_.]*[a-zA-Z0-9])?;

// literals
INT		: [0-9][0-9_]*;
FLOAT	: INT? '.' INT 'f'?
		| INT '.' 'f'?
		| INT 'f'
		;

STRING		: ('"' (~["\r\n] | '\\"')* '"')
            | ('\'' (~['\r\n] | '\\\'')* '\'');

COMMENT		    : '#' ~ [\r\n]* -> skip;
BLOCK_COMMENT	: '#--' .*? '--#' -> skip;
WHITESPACE	    : [ \t\r\n] -> skip;

OPEN_BRACE	: '{';
CLOSE_BRACE	: '}';

OPEN_SQUARE : '[';
CLOSE_SQUARE: ']';

OPEN_PAREN	: '(';
CLOSE_PAREN	: ')';

BAR		    : '|';

PLUS		: '+';
MINUS		: '-';
MULTI		: '*';
FLOAT_DIV	: '/';
INT_DIV		: '//';
MOD		    : '%';
EQUALS		: '=';

LT		    : '<';
LT_EQ		: '<=';
GT		    : '>';
GT_EQ		: '>=';
EQ		    : '==';
NEQ	      	: '!=';

BIT_AND     : '&';
BIT_XOR     : '^';

AND         : '&&';
OR          : '||';
NOT         : '!';