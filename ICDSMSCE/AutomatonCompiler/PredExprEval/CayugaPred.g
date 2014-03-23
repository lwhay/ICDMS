//obtained from http://www.imada.sdu.dk/~morling/issue4.htm

header{
#include <iostream>
using namespace std;

#include "AutomatonCompiler/PredExprEval/BasicAST.h"
#include "AutomatonCompiler/PredExprEval/BinaryCompArithOpAST.h"
#include "AutomatonCompiler/PredExprEval/DataTypeAST.h"
#include "AutomatonCompiler/PredExprEval/AttrAST.h"

using namespace Cayuga::AutomatonCompiler::PredExprEval;
}

options { 
    language="Cpp"; 
}

class P extends Parser;
options {
    buildAST = true;
    k=4;
    ASTLabelType = "RefBasicAST";
}



tokens {
    //COMP_OP<AST=BinaryCompArithOpAST>;

    EQ<AST=BinaryCompArithOpAST>;
    NE<AST=BinaryCompArithOpAST>;
    LE<AST=BinaryCompArithOpAST>;
    OP_LT<AST=BinaryCompArithOpAST>;
    GE<AST=BinaryCompArithOpAST>;
    GT<AST=BinaryCompArithOpAST>;
    

	PLUS<AST=BinaryCompArithOpAST>;
	MINUS<AST=BinaryCompArithOpAST>;
	MUL<AST=BinaryCompArithOpAST>;
	DIV<AST=BinaryCompArithOpAST>;
	MOD<AST=BinaryCompArithOpAST>;
	EXP<AST=BinaryCompArithOpAST>;

	INT<AST=DataTypeAST>;
	FLOAT<AST=DataTypeAST>;
	STRING<AST=DataTypeAST>;

	ATTR_OR_FUNC_NAME<AST=AttrAST>;
}



startRule
    :   arith_exp (EQ^ | NE^ | LE^ | OP_LT^ | GT^ | GE^) arith_exp
;

arith_exp
    :  term ( (PLUS^ | MINUS^) term )*
;

term 
      : factor ( (MUL^ | DIV^) factor )*
    ;

factor 
      : factor2 ( (EXP^ | MOD^) factor2 )*
    ;

factor2
    :   OPEN_PAREN! arith_exp CLOSE_PAREN!
    |   const_lit
    |   (ATTR_OR_FUNC_NAME^ DOT)? ATTR_OR_FUNC_NAME //this is a hack: the first
                            //ATTR_OR_FUNC_NAME can only take value EV or NODE
    |   ATTR_OR_FUNC_NAME^ OPEN_PAREN (arith_exp (COMMA! arith_exp)*)? CLOSE_PAREN!
;

const_lit
    :   INT | FLOAT | STRING
;


// attr : ATTR_OR_FUNC_NAME
// ;

// funcName : ATTR_OR_FUNC_NAME
// ;


//  src
//      :   "EV" | "event"
//      |   "NODE" | "node"
//  ;

// ts
//     :   "START" | "start"
//     |   "END" | "end"
// ;

/*
startRule
    :   ATTR_OR_FUNC_NAME
        //{cout<<"Hi there, "<<n<<endl;}
    ;
*/


class L extends Lexer;
options {
    filter=WS;
    charVocabulary = '\3'..'\177';
    k=4;
}


LE : "<=";
OP_LT : "<";
GE : ">=";
GT : ">";
EQ : "=";
NE : "!=";

PLUS : '+';
MINUS : '-';
MUL : '*';
DIV : '/';
MOD : '%';
EXP : '^';

OPEN_PAREN: '(';
CLOSE_PAREN: ')';
DOT: '.';

COMMA: ',';

protected
LETTER : ('a'..'z'|'A'..'Z');

protected
DIGIT : '0'..'9';

protected
INT :;

protected
FLOAT :;

//INT : (DIGIT)+;

//FLOAT : (DIGIT)+ DOT (DIGIT)+;

protected
Exponent
    : 'e' ( '+' | '-' )? (DIGIT)+
    ;


Number
    :
      ( (DIGIT)+ (DOT | 'e') ) => (DIGIT)+ ( DOT (DIGIT)* (Exponent)? | Exponent) { _ttype = FLOAT; }
    | (DIGIT)+ { _ttype = INT; }
    ;


STRING : '\'' (~('"' | '\''))* '\'' ;

ATTR_OR_FUNC_NAME : LETTER (LETTER | DIGIT | '_' )*;


protected
NEWLINE
    :   '\r' '\n'   // DOS
    |   '\n'        // UNIX
    ;

protected
WS  :   (   ' '
        |   '\t'
        |   '\r' '\n' { newline(); }
        |   '\n'      { newline(); }
        |   '\r'      { newline(); }
        )
        //{$setType(Token.SKIP);} //ignore this token
    ;
