#ifndef INC_P_hpp_
#define INC_P_hpp_

#include <config.hpp>
/* $ANTLR 2.7.6 (20060413): "CayugaPred.g" -> "P.hpp"$ */
#include <TokenStream.hpp>
#include <TokenBuffer.hpp>
#include "PTokenTypes.hpp"
#include <LLkParser.hpp>

#line 3 "CayugaPred.g"

#include <iostream>
using namespace std;

#include "AutomatonCompiler/PredExprEval/BasicAST.h"
#include "AutomatonCompiler/PredExprEval/BinaryCompArithOpAST.h"
#include "AutomatonCompiler/PredExprEval/DataTypeAST.h"
#include "AutomatonCompiler/PredExprEval/AttrAST.h"

using namespace Cayuga::AutomatonCompiler::PredExprEval;

#line 24 "P.hpp"
class CUSTOM_API P : public ANTLR_USE_NAMESPACE(antlr)LLkParser, public PTokenTypes
{
#line 1 "CayugaPred.g"
#line 28 "P.hpp"
public:
	void initializeASTFactory( ANTLR_USE_NAMESPACE(antlr)ASTFactory& factory );
protected:
	P(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf, int k);
public:
	P(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf);
protected:
	P(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer, int k);
public:
	P(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer);
	P(const ANTLR_USE_NAMESPACE(antlr)ParserSharedInputState& state);
	int getNumTokens() const
	{
		return P::NUM_TOKENS;
	}
	const char* getTokenName( int type ) const
	{
		if( type > getNumTokens() ) return 0;
		return P::tokenNames[type];
	}
	const char* const* getTokenNames() const
	{
		return P::tokenNames;
	}
	public: void startRule();
	public: void arith_exp();
	public: void term();
	public: void factor();
	public: void factor2();
	public: void const_lit();
public:
	ANTLR_USE_NAMESPACE(antlr)RefAST getAST()
	{
		return ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST);
	}
	
protected:
	RefBasicAST returnAST;
private:
	static const char* tokenNames[];
#ifndef NO_STATIC_CONSTS
	static const int NUM_TOKENS = 30;
#else
	enum {
		NUM_TOKENS = 30
	};
#endif
	
	static const unsigned long _tokenSet_0_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_0;
	static const unsigned long _tokenSet_1_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_1;
	static const unsigned long _tokenSet_2_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_2;
	static const unsigned long _tokenSet_3_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_3;
	static const unsigned long _tokenSet_4_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_4;
	static const unsigned long _tokenSet_5_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_5;
};

#endif /*INC_P_hpp_*/
