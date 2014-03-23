header{
    #include "CayugaQLAST.h"
    using namespace Cayuga::QLCompiler;

    #include <iostream>
    using namespace std;
}

options { 
    language="Cpp"; 
}

class P2 extends Parser;
options {
    buildAST = true;
    k=4;
    ASTLabelType = "RefCayugaQLAST";
}

tokens {
	QUERY;
	RESULT_CLAUSE;
	SELECT_CLAUSE;
    TERM_EXPR;
    STREAM_EXPR;

	STREAM_NAME;
//	STREAM_NAME_ALIAS;
//  UNION_QUERY;
    UNARY_EXPR;
    UNARY_OPS;
    FILTER_EXPR;
//    ALPHA_EXPR;
//    BINARY_EXPR;
    SEQ_OP;
    MU_OP;

    PRED_EXPR;
    AND_EXPR;
    ATOM_PRED;
    DUR_PRED;
//    NEG_ATOM_PRED;
//    COMPARE_EXPR;
    PLUS_MINUS_EXPR;
    MUL_DIV_EXPR;
    EXP_MOD_EXPR;
    CONST_LIT_EXPR;
    ATTR_EXPR;

    ATTR_NAME;
    STREAM_ATTR_NAME;

    FUNC_EXPR;
}

query: (selectClause)? streamExpr (resultClause)? {## = #(#[QUERY, ""], ##);}
;

resultClause: publish! streamName {##->setType(RESULT_CLAUSE);}
;

//select clause
selectClause : select! selectList from! {## = #(#[SELECT_CLAUSE, ""], ##);}
;

selectList : termOrAny (COMMA! termOrAny)*;

termOrAny : any
        | termExpr
;

termExpr : term (as! attrName)? {## = #(#[TERM_EXPR, ""], ##);}
;

streamName : STRING;
attrName : STRING;

//from clause
//streamExpr : streamExprBody {## = #(#[STREAM_EXPR, ""], ##);}

streamExpr : unaryExpr (binaryOp unaryExpr)* {## = #(#[STREAM_EXPR, ""], ##);}
;

//streamExpr : unaryExpr binaryExpr {## = #(#[STREAM_EXPR, ""], ##);};

//binaryExpr : binaryOp unaryExpr binaryExpr {## = #(#[BINARY_EXPR, ""], ##);}
//    | ;

unaryExpr : unaryOps  primaryExpr {## = #(#[UNARY_EXPR, ""], ##);}
        | primaryExpr {## = #(#[UNARY_EXPR, ""], ##);}
;

primaryExpr : streamName {##->setType(STREAM_NAME);}
           | OPEN_PAREN! query CLOSE_PAREN! 
;

unaryOps : (selectionOp)+ {## = #(#[UNARY_OPS, ""], ##);}
;

selectionOp : filter! OPEN_CUR_PAREN! predExpr CLOSE_CUR_PAREN! {## = #(#[FILTER_EXPR, ""], ##);};

//alphaOp : alpha OPEN_CUR_PAREN! termExpr CLOSE_CUR_PAREN!;

//ISSUE: without paranthesis, there is inifinite recursion bewteen binaryExpr below and streamExpr
//binaryExpr : OPEN_PAREN! streamExpr CLOSE_PAREN! binaryOp OPEN_PAREN! streamExpr CLOSE_PAREN!; 

binaryOp : sequenceOp {## = #(#[SEQ_OP, ""], ##);} 
    | muOp {## = #(#[MU_OP, ""], ##);} 
;

sequenceOp : next! OPEN_CUR_PAREN! (predExpr)? CLOSE_CUR_PAREN!;

muOp : fold! OPEN_CUR_PAREN! (muArgs)? CLOSE_CUR_PAREN!;

//predExpr is the filter predicate
//unaryOps is the rebind predicate
//termExpr is the aggregate specification
muArgs : (predExpr)? COMMA (predExpr)? COMMA (muTerms)?;

muTerms: termExpr (COMMA termExpr)*
;

predExpr : andExpr ( or_kw! andExpr )* {## = #(#[PRED_EXPR, ""], ##);};

andExpr
      : literal ( and_kw! literal )* {## = #(#[AND_EXPR, ""], ##);}
    ;
 
//literal : atomPred | not literal;

literal : term COMP_OP term {## = #(#[ATOM_PRED, ""], ##);}
//    | not_kw literal {## = #(#[NEG_ATOM_PRED, ""], ##);}
    | dur! COMP_OP number (time_unit)* {## = #(#[DUR_PRED, ""], ##);}
;

time_unit : year | month | day | hour | min | sec;

term :  m_term ( (PLUS | MINUS) m_term )* {## = #(#[PLUS_MINUS_EXPR, ""], ##);}
;

m_term 
      : factor ( (MUL | DIV) factor )* {## = #(#[MUL_DIV_EXPR, ""], ##);}
    ;

factor 
      : factor2 ( (EXP | MOD) factor2 )* {## = #(#[EXP_MOD_EXPR, ""], ##);}
    ;

factor2
    :   MINUS^ factor2
    |   const_lit {## = #(#[CONST_LIT_EXPR, ""], ##);}
    |   attrVar
    |   func OPEN_PAREN! (term)? (COMMA term)* CLOSE_PAREN! {## = #(#[FUNC_EXPR, ""], ##);}
    |   OPEN_PAREN! predExpr CLOSE_PAREN!
;

attrVar : (ATTR_VAR_MARKER (number)? DOT!)? attrName {## = #(#[ATTR_EXPR, ""], ##);}
;

const_lit
    :  number  | QUOTED_STRING
;

number: INT | FLOAT;

func : STRING;

//sum | max | min | avg | count;


//key words
publish
    :   "PUBLISH" | "publish"
;

union_kw
    :   "UNION" | "union"
;

select 
    :   "SELECT" | "select"
;

as
    :   "AS" | "as"
;

any : MUL;


from
    :   "FROM" | "from"
;

next 
    :   "NEXT" | "next"
;

fold 
    :   "FOLD" | "fold"
;

filter 
    :   "FILTER" | "filter"
;

// alpha 
//     :   "ALPHA" | "alpha"
// ;

// sum 
//     :   "SUM" | "sum"
// ;

// max 
//     :   "MAX" | "max"
// ;

// min 
//     :   "MIN" | "min"
// ;

// avg
//     :   "AVG" | "avg"
// ;

// count
//     :   "COUNT" | "count"
// ;

and_kw
    :   "AND" | "and"
;

or_kw
    :   "OR" | "or"
;

not_kw
    :   "NOT" | "not"
;

dur 
    : "DUR" | "dur"
;

year : "YEAR" | "year";

month : "MONTH" | "month";

day : "DAY" | "day";

hour : "HOUR" | "hour";

min : "MIN" | "min";

sec : "SEC" | "sec";

class L2 extends Lexer;
options {
    filter=WS;
    charVocabulary = '\3'..'\177';
    k=4;
}

COMP_OP: EQ | NE | LE | OP_LT | GT | GE;

ATTR_VAR_MARKER : '$';

COMMA : ',';

protected LE : "<=";
protected OP_LT : "<";
protected GE : ">=";
protected GT : ">";
protected EQ : "=";
protected NE : "!=";

PLUS : '+';
MINUS : '-';
MUL : '*';
DIV : '/';
MOD : '%';
EXP : '^';

OPEN_PAREN: '(';
CLOSE_PAREN: ')';

OPEN_CUR_PAREN: '{';
CLOSE_CUR_PAREN: '}';


DOT : '.';

protected
LETTER : ('a'..'z'|'A'..'Z');

protected
DIGIT : '0'..'9';

//protected
//INT :;

//protected
//FLOAT :;


protected INT : (DIGIT)+;

protected FLOAT : (DIGIT)+ DOT (DIGIT)+;


INT_FLOAT : ( ( DIGIT )+ ) => INT { $setType( INT ); } | ( ( DIGIT )+ DOT ) => FLOAT { $setType( FLOAT ); };

protected
Exponent
    : 'e' ( '+' | '-' )? (DIGIT)+
    ;


// Number
//     :
//       ( (DIGIT)+ (DOT | 'e') ) => (DIGIT)+ ( DOT (DIGIT)* (Exponent)? | Exponent) { _ttype = FLOAT; }
//     | DOT { _ttype = DOT; } ( (DIGIT)+ (Exponent)? { _ttype = FLOAT; } )?
//     | (DIGIT)+ { _ttype = INT; }
//     ;


QUOTED_STRING : '\''! (~('"' | '\''))* '\''! ;
//QUOTED_STRING : '\''! (LETTER | DIGIT | '_' | ' ' | '.' | '@')* '\''! ;


STRING : LETTER (LETTER | DIGIT | '_')*;


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
