/* $ANTLR 2.7.6 (20060413): "CayugaPred.g" -> "P.cpp"$ */
#include "P.hpp"
#include <NoViableAltException.hpp>
#include <SemanticException.hpp>
#include <ASTFactory.hpp>
#line 1 "CayugaPred.g"
#line 8 "P.cpp"
P::P(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf, int k)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(tokenBuf,k)
{
}

P::P(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(tokenBuf,4)
{
}

P::P(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer, int k)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(lexer,k)
{
}

P::P(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(lexer,4)
{
}

P::P(const ANTLR_USE_NAMESPACE(antlr)ParserSharedInputState& state)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(state,4)
{
}

void P::startRule() {
	returnAST = RefBasicAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefBasicAST startRule_AST = RefBasicAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		arith_exp();
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		{
		switch ( LA(1)) {
		case EQ:
		{
			RefBinaryCompArithOpAST tmp1_AST = RefBinaryCompArithOpAST(RefBasicAST(ANTLR_USE_NAMESPACE(antlr)nullAST));
			tmp1_AST = astFactory->create(LT(1));
			astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp1_AST));
			match(EQ);
			break;
		}
		case NE:
		{
			RefBinaryCompArithOpAST tmp2_AST = RefBinaryCompArithOpAST(RefBasicAST(ANTLR_USE_NAMESPACE(antlr)nullAST));
			tmp2_AST = astFactory->create(LT(1));
			astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp2_AST));
			match(NE);
			break;
		}
		case LE:
		{
			RefBinaryCompArithOpAST tmp3_AST = RefBinaryCompArithOpAST(RefBasicAST(ANTLR_USE_NAMESPACE(antlr)nullAST));
			tmp3_AST = astFactory->create(LT(1));
			astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp3_AST));
			match(LE);
			break;
		}
		case OP_LT:
		{
			RefBinaryCompArithOpAST tmp4_AST = RefBinaryCompArithOpAST(RefBasicAST(ANTLR_USE_NAMESPACE(antlr)nullAST));
			tmp4_AST = astFactory->create(LT(1));
			astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp4_AST));
			match(OP_LT);
			break;
		}
		case GT:
		{
			RefBinaryCompArithOpAST tmp5_AST = RefBinaryCompArithOpAST(RefBasicAST(ANTLR_USE_NAMESPACE(antlr)nullAST));
			tmp5_AST = astFactory->create(LT(1));
			astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp5_AST));
			match(GT);
			break;
		}
		case GE:
		{
			RefBinaryCompArithOpAST tmp6_AST = RefBinaryCompArithOpAST(RefBasicAST(ANTLR_USE_NAMESPACE(antlr)nullAST));
			tmp6_AST = astFactory->create(LT(1));
			astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp6_AST));
			match(GE);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		arith_exp();
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		startRule_AST = RefBasicAST(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_0);
	}
	returnAST = startRule_AST;
}

void P::arith_exp() {
	returnAST = RefBasicAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefBasicAST arith_exp_AST = RefBasicAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		term();
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == PLUS || LA(1) == MINUS)) {
				{
				switch ( LA(1)) {
				case PLUS:
				{
					RefBinaryCompArithOpAST tmp7_AST = RefBinaryCompArithOpAST(RefBasicAST(ANTLR_USE_NAMESPACE(antlr)nullAST));
					tmp7_AST = astFactory->create(LT(1));
					astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp7_AST));
					match(PLUS);
					break;
				}
				case MINUS:
				{
					RefBinaryCompArithOpAST tmp8_AST = RefBinaryCompArithOpAST(RefBasicAST(ANTLR_USE_NAMESPACE(antlr)nullAST));
					tmp8_AST = astFactory->create(LT(1));
					astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp8_AST));
					match(MINUS);
					break;
				}
				default:
				{
					throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
				}
				}
				}
				term();
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			else {
				goto _loop6;
			}
			
		}
		_loop6:;
		} // ( ... )*
		arith_exp_AST = RefBasicAST(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_1);
	}
	returnAST = arith_exp_AST;
}

void P::term() {
	returnAST = RefBasicAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefBasicAST term_AST = RefBasicAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		factor();
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == MUL || LA(1) == DIV)) {
				{
				switch ( LA(1)) {
				case MUL:
				{
					RefBinaryCompArithOpAST tmp9_AST = RefBinaryCompArithOpAST(RefBasicAST(ANTLR_USE_NAMESPACE(antlr)nullAST));
					tmp9_AST = astFactory->create(LT(1));
					astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp9_AST));
					match(MUL);
					break;
				}
				case DIV:
				{
					RefBinaryCompArithOpAST tmp10_AST = RefBinaryCompArithOpAST(RefBasicAST(ANTLR_USE_NAMESPACE(antlr)nullAST));
					tmp10_AST = astFactory->create(LT(1));
					astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp10_AST));
					match(DIV);
					break;
				}
				default:
				{
					throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
				}
				}
				}
				factor();
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			else {
				goto _loop10;
			}
			
		}
		_loop10:;
		} // ( ... )*
		term_AST = RefBasicAST(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_2);
	}
	returnAST = term_AST;
}

void P::factor() {
	returnAST = RefBasicAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefBasicAST factor_AST = RefBasicAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		factor2();
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == MOD || LA(1) == EXP)) {
				{
				switch ( LA(1)) {
				case EXP:
				{
					RefBinaryCompArithOpAST tmp11_AST = RefBinaryCompArithOpAST(RefBasicAST(ANTLR_USE_NAMESPACE(antlr)nullAST));
					tmp11_AST = astFactory->create(LT(1));
					astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp11_AST));
					match(EXP);
					break;
				}
				case MOD:
				{
					RefBinaryCompArithOpAST tmp12_AST = RefBinaryCompArithOpAST(RefBasicAST(ANTLR_USE_NAMESPACE(antlr)nullAST));
					tmp12_AST = astFactory->create(LT(1));
					astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp12_AST));
					match(MOD);
					break;
				}
				default:
				{
					throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
				}
				}
				}
				factor2();
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			else {
				goto _loop14;
			}
			
		}
		_loop14:;
		} // ( ... )*
		factor_AST = RefBasicAST(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_3);
	}
	returnAST = factor_AST;
}

void P::factor2() {
	returnAST = RefBasicAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefBasicAST factor2_AST = RefBasicAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		switch ( LA(1)) {
		case OPEN_PAREN:
		{
			match(OPEN_PAREN);
			arith_exp();
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			match(CLOSE_PAREN);
			factor2_AST = RefBasicAST(currentAST.root);
			break;
		}
		case INT:
		case FLOAT:
		case STRING:
		{
			const_lit();
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			factor2_AST = RefBasicAST(currentAST.root);
			break;
		}
		default:
			if ((LA(1) == ATTR_OR_FUNC_NAME) && (_tokenSet_4.member(LA(2)))) {
				{
				if ((LA(1) == ATTR_OR_FUNC_NAME) && (LA(2) == DOT)) {
					RefAttrAST tmp15_AST = RefAttrAST(RefBasicAST(ANTLR_USE_NAMESPACE(antlr)nullAST));
					tmp15_AST = astFactory->create(LT(1));
					astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp15_AST));
					match(ATTR_OR_FUNC_NAME);
					RefBasicAST tmp16_AST = RefBasicAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
					tmp16_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp16_AST));
					match(DOT);
				}
				else if ((LA(1) == ATTR_OR_FUNC_NAME) && (_tokenSet_5.member(LA(2)))) {
				}
				else {
					throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
				}
				
				}
				RefAttrAST tmp17_AST = RefAttrAST(RefBasicAST(ANTLR_USE_NAMESPACE(antlr)nullAST));
				tmp17_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp17_AST));
				match(ATTR_OR_FUNC_NAME);
				factor2_AST = RefBasicAST(currentAST.root);
			}
			else if ((LA(1) == ATTR_OR_FUNC_NAME) && (LA(2) == OPEN_PAREN)) {
				RefAttrAST tmp18_AST = RefAttrAST(RefBasicAST(ANTLR_USE_NAMESPACE(antlr)nullAST));
				tmp18_AST = astFactory->create(LT(1));
				astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp18_AST));
				match(ATTR_OR_FUNC_NAME);
				RefBasicAST tmp19_AST = RefBasicAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
				tmp19_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp19_AST));
				match(OPEN_PAREN);
				{
				switch ( LA(1)) {
				case INT:
				case FLOAT:
				case STRING:
				case ATTR_OR_FUNC_NAME:
				case OPEN_PAREN:
				{
					arith_exp();
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
					{ // ( ... )*
					for (;;) {
						if ((LA(1) == COMMA)) {
							match(COMMA);
							arith_exp();
							astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
						}
						else {
							goto _loop19;
						}
						
					}
					_loop19:;
					} // ( ... )*
					break;
				}
				case CLOSE_PAREN:
				{
					break;
				}
				default:
				{
					throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
				}
				}
				}
				match(CLOSE_PAREN);
				factor2_AST = RefBasicAST(currentAST.root);
			}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_5);
	}
	returnAST = factor2_AST;
}

void P::const_lit() {
	returnAST = RefBasicAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefBasicAST const_lit_AST = RefBasicAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		switch ( LA(1)) {
		case INT:
		{
			RefDataTypeAST tmp22_AST = RefDataTypeAST(RefBasicAST(ANTLR_USE_NAMESPACE(antlr)nullAST));
			tmp22_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp22_AST));
			match(INT);
			const_lit_AST = RefBasicAST(currentAST.root);
			break;
		}
		case FLOAT:
		{
			RefDataTypeAST tmp23_AST = RefDataTypeAST(RefBasicAST(ANTLR_USE_NAMESPACE(antlr)nullAST));
			tmp23_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp23_AST));
			match(FLOAT);
			const_lit_AST = RefBasicAST(currentAST.root);
			break;
		}
		case STRING:
		{
			RefDataTypeAST tmp24_AST = RefDataTypeAST(RefBasicAST(ANTLR_USE_NAMESPACE(antlr)nullAST));
			tmp24_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp24_AST));
			match(STRING);
			const_lit_AST = RefBasicAST(currentAST.root);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_5);
	}
	returnAST = const_lit_AST;
}

void P::initializeASTFactory( ANTLR_USE_NAMESPACE(antlr)ASTFactory& factory )
{
	factory.registerFactory(4, "BinaryCompArithOpAST", BinaryCompArithOpAST::factory);
	factory.registerFactory(5, "BinaryCompArithOpAST", BinaryCompArithOpAST::factory);
	factory.registerFactory(6, "BinaryCompArithOpAST", BinaryCompArithOpAST::factory);
	factory.registerFactory(7, "BinaryCompArithOpAST", BinaryCompArithOpAST::factory);
	factory.registerFactory(8, "BinaryCompArithOpAST", BinaryCompArithOpAST::factory);
	factory.registerFactory(9, "BinaryCompArithOpAST", BinaryCompArithOpAST::factory);
	factory.registerFactory(10, "BinaryCompArithOpAST", BinaryCompArithOpAST::factory);
	factory.registerFactory(11, "BinaryCompArithOpAST", BinaryCompArithOpAST::factory);
	factory.registerFactory(12, "BinaryCompArithOpAST", BinaryCompArithOpAST::factory);
	factory.registerFactory(13, "BinaryCompArithOpAST", BinaryCompArithOpAST::factory);
	factory.registerFactory(14, "BinaryCompArithOpAST", BinaryCompArithOpAST::factory);
	factory.registerFactory(15, "BinaryCompArithOpAST", BinaryCompArithOpAST::factory);
	factory.registerFactory(16, "DataTypeAST", DataTypeAST::factory);
	factory.registerFactory(17, "DataTypeAST", DataTypeAST::factory);
	factory.registerFactory(18, "DataTypeAST", DataTypeAST::factory);
	factory.registerFactory(19, "AttrAST", AttrAST::factory);
	factory.setMaxNodeType(29);
}
const char* P::tokenNames[] = {
	"<0>",
	"EOF",
	"<2>",
	"NULL_TREE_LOOKAHEAD",
	"EQ",
	"NE",
	"LE",
	"OP_LT",
	"GE",
	"GT",
	"PLUS",
	"MINUS",
	"MUL",
	"DIV",
	"MOD",
	"EXP",
	"INT",
	"FLOAT",
	"STRING",
	"ATTR_OR_FUNC_NAME",
	"OPEN_PAREN",
	"CLOSE_PAREN",
	"DOT",
	"COMMA",
	"LETTER",
	"DIGIT",
	"Exponent",
	"Number",
	"NEWLINE",
	"WS",
	0
};

const unsigned long P::_tokenSet_0_data_[] = { 2UL, 0UL, 0UL, 0UL };
// EOF 
const ANTLR_USE_NAMESPACE(antlr)BitSet P::_tokenSet_0(_tokenSet_0_data_,4);
const unsigned long P::_tokenSet_1_data_[] = { 10486770UL, 0UL, 0UL, 0UL };
// EOF EQ NE LE OP_LT GE GT CLOSE_PAREN COMMA 
const ANTLR_USE_NAMESPACE(antlr)BitSet P::_tokenSet_1(_tokenSet_1_data_,4);
const unsigned long P::_tokenSet_2_data_[] = { 10489842UL, 0UL, 0UL, 0UL };
// EOF EQ NE LE OP_LT GE GT PLUS MINUS CLOSE_PAREN COMMA 
const ANTLR_USE_NAMESPACE(antlr)BitSet P::_tokenSet_2(_tokenSet_2_data_,4);
const unsigned long P::_tokenSet_3_data_[] = { 10502130UL, 0UL, 0UL, 0UL };
// EOF EQ NE LE OP_LT GE GT PLUS MINUS MUL DIV CLOSE_PAREN COMMA 
const ANTLR_USE_NAMESPACE(antlr)BitSet P::_tokenSet_3(_tokenSet_3_data_,4);
const unsigned long P::_tokenSet_4_data_[] = { 14745586UL, 0UL, 0UL, 0UL };
// EOF EQ NE LE OP_LT GE GT PLUS MINUS MUL DIV MOD EXP CLOSE_PAREN DOT 
// COMMA 
const ANTLR_USE_NAMESPACE(antlr)BitSet P::_tokenSet_4(_tokenSet_4_data_,4);
const unsigned long P::_tokenSet_5_data_[] = { 10551282UL, 0UL, 0UL, 0UL };
// EOF EQ NE LE OP_LT GE GT PLUS MINUS MUL DIV MOD EXP CLOSE_PAREN COMMA 
const ANTLR_USE_NAMESPACE(antlr)BitSet P::_tokenSet_5(_tokenSet_5_data_,4);


