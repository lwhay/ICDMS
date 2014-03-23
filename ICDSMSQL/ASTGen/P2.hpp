#ifndef INC_P2_hpp_
#define INC_P2_hpp_

#include <antlr/config.hpp>
/* $ANTLR 2.7.7 (2006-11-01): "CayugaQL.g" -> "P2.hpp"$ */
#include <antlr/TokenStream.hpp>
#include <antlr/TokenBuffer.hpp>
#include "P2TokenTypes.hpp"
#include <antlr/LLkParser.hpp>

#line 1 "CayugaQL.g"

    #include "CayugaQLAST.h"
    using namespace Cayuga::QLCompiler;

    #include <iostream>
    using namespace std;

#line 20 "P2.hpp"
class CUSTOM_API P2 : public ANTLR_USE_NAMESPACE(antlr)LLkParser, public P2TokenTypes
{
#line 1 "CayugaQL.g"
#line 24 "P2.hpp"
public:
	void initializeASTFactory( ANTLR_USE_NAMESPACE(antlr)ASTFactory& factory );
protected:
	P2(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf, int k);
public:
	P2(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf);
protected:
	P2(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer, int k);
public:
	P2(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer);
	P2(const ANTLR_USE_NAMESPACE(antlr)ParserSharedInputState& state);
	int getNumTokens() const
	{
		return P2::NUM_TOKENS;
	}
	const char* getTokenName( int type ) const
	{
		if( type > getNumTokens() ) return 0;
		return P2::tokenNames[type];
	}
	const char* const* getTokenNames() const
	{
		return P2::tokenNames;
	}
	public: void query();
	public: void selectClause();
	public: void windowClause();
	public: void streamExpr();
	public: void resultClause();
	public: void publish();
	public: void streamName();
	public: void select();
	public: void selectList();
	public: void from();
	public: void termOrAny();
	public: void any();
	public: void termExpr();
	public: void term();
	public: void as();
	public: void attrName();
	public: void unaryExpr();
	public: void binaryOp();
	public: void unaryOps();
	public: void primaryExpr();
	public: void window();
	public: void type();
	public: void length();
	public: void step();
	public: void timeStart();
	public: void number();
	public: void selectionOp();
	public: void filter();
	public: void predExpr();
	public: void sequenceOp();
	public: void muOp();
	public: void next();
	public: void predicateWindow();
	public: void fold();
	public: void muArgs();
	public: void predwindow();
	public: void temporalClause();
	public: void countClause();
	public: void temporal();
	public: void time_unit();
	public: void count();
	public: void muTerms();
	public: void andExpr();
	public: void or_kw();
	public: void literal();
	public: void and_kw();
	public: void dur();
	public: void year();
	public: void month();
	public: void day();
	public: void hour();
	public: void min();
	public: void sec();
	public: void m_term();
	public: void factor();
	public: void factor2();
	public: void const_lit();
	public: void attrVar();
	public: void func();
	public: void union_kw();
	public: void not_kw();
public:
	ANTLR_USE_NAMESPACE(antlr)RefAST getAST()
	{
		return ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST);
	}
	
protected:
	RefCayugaQLAST returnAST;
private:
	static const char* tokenNames[];
#ifndef NO_STATIC_CONSTS
	static const int NUM_TOKENS = 105;
#else
	enum {
		NUM_TOKENS = 105
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
	static const unsigned long _tokenSet_6_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_6;
	static const unsigned long _tokenSet_7_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_7;
	static const unsigned long _tokenSet_8_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_8;
	static const unsigned long _tokenSet_9_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_9;
	static const unsigned long _tokenSet_10_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_10;
	static const unsigned long _tokenSet_11_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_11;
	static const unsigned long _tokenSet_12_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_12;
	static const unsigned long _tokenSet_13_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_13;
	static const unsigned long _tokenSet_14_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_14;
	static const unsigned long _tokenSet_15_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_15;
	static const unsigned long _tokenSet_16_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_16;
	static const unsigned long _tokenSet_17_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_17;
	static const unsigned long _tokenSet_18_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_18;
	static const unsigned long _tokenSet_19_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_19;
	static const unsigned long _tokenSet_20_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_20;
	static const unsigned long _tokenSet_21_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_21;
	static const unsigned long _tokenSet_22_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_22;
	static const unsigned long _tokenSet_23_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_23;
	static const unsigned long _tokenSet_24_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_24;
	static const unsigned long _tokenSet_25_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_25;
	static const unsigned long _tokenSet_26_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_26;
	static const unsigned long _tokenSet_27_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_27;
	static const unsigned long _tokenSet_28_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_28;
	static const unsigned long _tokenSet_29_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_29;
	static const unsigned long _tokenSet_30_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_30;
};

#endif /*INC_P2_hpp_*/
