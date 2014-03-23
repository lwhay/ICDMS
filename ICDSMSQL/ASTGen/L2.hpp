#ifndef INC_L2_hpp_
#define INC_L2_hpp_

#include <antlr/config.hpp>
/* $ANTLR 2.7.7 (2006-11-01): "CayugaQL.g" -> "L2.hpp"$ */
#include <antlr/CommonToken.hpp>
#include <antlr/InputBuffer.hpp>
#include <antlr/BitSet.hpp>
#include "P2TokenTypes.hpp"
#include <antlr/CharScanner.hpp>
#line 1 "CayugaQL.g"

    #include "CayugaQLAST.h"
    using namespace Cayuga::QLCompiler;

    #include <iostream>
    using namespace std;

#line 20 "L2.hpp"
class CUSTOM_API L2 : public ANTLR_USE_NAMESPACE(antlr)CharScanner, public P2TokenTypes
{
#line 1 "CayugaQL.g"
#line 24 "L2.hpp"
private:
	void initLiterals();
public:
	bool getCaseSensitiveLiterals() const
	{
		return true;
	}
public:
	L2(ANTLR_USE_NAMESPACE(std)istream& in);
	L2(ANTLR_USE_NAMESPACE(antlr)InputBuffer& ib);
	L2(const ANTLR_USE_NAMESPACE(antlr)LexerSharedInputState& state);
	ANTLR_USE_NAMESPACE(antlr)RefToken nextToken();
	public: void mCOMP_OP(bool _createToken);
	protected: void mEQ(bool _createToken);
	protected: void mNE(bool _createToken);
	protected: void mLE(bool _createToken);
	protected: void mOP_LT(bool _createToken);
	protected: void mGT(bool _createToken);
	protected: void mGE(bool _createToken);
	public: void mATTR_VAR_MARKER(bool _createToken);
	public: void mCOMMA(bool _createToken);
	public: void mPLUS(bool _createToken);
	public: void mMINUS(bool _createToken);
	public: void mMUL(bool _createToken);
	public: void mDIV(bool _createToken);
	public: void mMOD(bool _createToken);
	public: void mEXP(bool _createToken);
	public: void mOPEN_PAREN(bool _createToken);
	public: void mCLOSE_PAREN(bool _createToken);
	public: void mOPEN_CUR_PAREN(bool _createToken);
	public: void mCLOSE_CUR_PAREN(bool _createToken);
	public: void mDOT(bool _createToken);
	protected: void mLETTER(bool _createToken);
	protected: void mDIGIT(bool _createToken);
	protected: void mINT(bool _createToken);
	protected: void mFLOAT(bool _createToken);
	public: void mINT_FLOAT(bool _createToken);
	protected: void mExponent(bool _createToken);
	public: void mQUOTED_STRING(bool _createToken);
	public: void mSTRING(bool _createToken);
	protected: void mNEWLINE(bool _createToken);
	protected: void mWS(bool _createToken);
private:
	
	static const unsigned long _tokenSet_0_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_0;
	static const unsigned long _tokenSet_1_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_1;
};

#endif /*INC_L2_hpp_*/
