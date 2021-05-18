
grammar Lua;

@header {
    #include <map>
    #include "intermediate/symtab/Symtab.h"
    #include "intermediate/type/Typespec.h"
    using namespace intermediate::symtab;
    using namespace intermediate::type;
}
chunk
    : block EOF
    ;

block
    : stat* retstat?
    ;

stat
    : ';'
    | assignStat
    | functioncall
    | repeatStat
    | printStat
    | ifStat
    | functiondef
    ;

printStat
	: 'print' printArguments;

printArguments 
	: '(' exp (',' exp)* ')' ;
	
retstat
    : 'return' exp? ';'?
    ;

assignStat
	: var_ '=' exp
	;
	
repeatStat
	: 'repeat' block 'until' exp
	;
	
ifStat
	: 'if' exp 'then' block ('elseif' exp 'then' block)* ('else' block)? 'end'
	;
	

varlist
    : var_ (',' var_)*
    ;

explist
    : exp (',' exp)*
    ;

exp
    : 'nil' | 'false' | 'true'
    | number
    | string
    | functioncall
    | prefixexp
    | exp operatorMulDiv exp
    | exp operatorAddSub exp
    | exp operatorComparison exp
    ;

prefixexp
    : varOrExp nameAndArgs*
    ;

functiondef
    : 'function' funcname funcbody
    ;
    
funcname
    : NAME ('.' NAME)* (':' NAME)?
    ;

funcbody
    : '(' parlist? ')' block 'end'
    ;

parlist
    : varlist (',' '...')? | '...'
    ;
    
functioncall
    : varOrExp nameAndArgs+
    ;

varOrExp
    : var_ | '(' exp ')'
    ;

var_	locals [SymtabEntry *entry = nullptr]
    : NAME
    ;

nameAndArgs
    : (':' NAME)? args
    ;

args
    : '(' explist? ')' | string
    ;

operatorOr
	: 'or';

operatorAnd
	: 'and';

operatorComparison
	: '<' | '>' | '<=' | '>=' | '~=' | '==';

operatorAddSub
	: '+' | '-';

operatorMulDiv
	: '*' | '/' ;

number
    : INT 
    ;

string
    : NORMALSTRING | CHARSTRING | LONGSTRING
    ;

// LEXER

NAME
    : [a-zA-Z_][a-zA-Z_0-9]*
    ;

NORMALSTRING
    : '"' ( EscapeSequence | ~('\\'|'"') )* '"'
    ;

CHARSTRING
    : '\'' ( EscapeSequence | ~('\''|'\\') )* '\''
    ;

LONGSTRING
    : '[' NESTED_STR ']'
    ;

fragment
NESTED_STR
    : '=' NESTED_STR '='
    | '[' .*? ']'
    ;

INT
    : Digit+
    ;

fragment
EscapeSequence
    : '\\' [abfnrtvz"'\\]
    | '\\' '\r'? '\n'
    | DecimalEscape
    ;
fragment
DecimalEscape
    : '\\' Digit
    | '\\' Digit Digit
    | '\\' [0-2] Digit Digit
    ;
fragment
Digit
    : [0-9]
    ;
    
COMMENT
    : '--[' NESTED_STR ']' -> channel(HIDDEN)
    ;
LINE_COMMENT
    : '--'
    (                                               // --
    | '[' '='*                                      // --[==
    | '[' '='* ~('='|'['|'\r'|'\n') ~('\r'|'\n')*   // --[==AA
    | ~('['|'\r'|'\n') ~('\r'|'\n')*                // --AAA
    ) ('\r\n'|'\r'|'\n'|EOF)
    -> channel(HIDDEN)
    ;
WS
    : [ \t\u000C\r\n]+ -> skip
    ;