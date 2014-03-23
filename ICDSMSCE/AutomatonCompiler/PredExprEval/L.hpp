#ifndef INC_L_hpp_
#define INC_L_hpp_

#include <config.hpp>
/* $ANTLR 2.7.6 (20060413): "CayugaPred.g" -> "L.hpp"$ */
#include <CommonToken.hpp>
#include <InputBuffer.hpp>
#include <BitSet.hpp>
#include "PTokenTypes.hpp"
#include <CharScanner.hpp>
#line 3 "CayugaPred.g"

#include <iostream>
using namespace std;

#include "AutomatonCompiler/PredExprEval/BasicAST.h"
#include "AutomatonCompiler/PredExprEval/BinaryCompArithOpAST.h"
#include "AutomatonCompiler/PredExprEval/DataTypeAST.h"
#include "AutomatonCompiler/PredExprEval/AttrAST.h"

using namespace Cayuga::AutomatonCompiler::PredExprEval;

#line 24 "L.hpp"
class CUSTOM_API L : public ANTLR_USE_NAMESPACE(antlr)CharScanner, public PTokenTypes
{
#line 1 "CayugaPred.g"
#line 28 "L.hpp"
private:
	void initLiterals();
public:
	bool getCaseSensitiveLiterals() const
	{
		return true;
	}
public:
	L(ANTLR_USE_NAMESPACE(std)istream& in);
	L(ANTLR_USE_NAMESPACE(antlr)InputBuffer& ib);
	L(const ANTLR_USE_NAMESPACE(antlr)LexerSharedInputState& state);
	ANTLR_USE_NAMESPACE(antlr)RefToken nextToken();
	public: void mLE(bool _createToken);
	public: void mOP_LT(bool _createToken);
	public: void mGE(bool _createToken);
	public: void mGT(bool _createToken);
	public: void mEQ(bool _createToken);
	public: void mNE(bool _createToken);
	public: void mPLUS(bool _createToken);
	public: void mMINUS(bool _createToken);
	public: void mMUL(bool _createToken);
	public: void mDIV(bool _createToken);
	public: void mMOD(bool _createToken);
	public: void mEXP(bool _createToken);
	public: void mOPEN_PAREN(bool _createToken);
	public: void mCLOSE_PAREN(bool _createToken);
	public: void mDOT(bool _createToken);
	public: void mCOMMA(bool _createToken);
	protected: void mLETTER(bool _createToken);
	protected: void mDIGIT(bool _createToken);
	protected: void mINT(bool _createToken);
	protected: void mFLOAT(bool _createToken);
	protected: void mExponent(bool _createToken);
	public: void mNumber(bool _createToken);
	public: void mSTRING(bool _createToken);
	public: void mATTR_OR_FUNC_NAME(bool _createToken);
	protected: void mNEWLINE(bool _createToken);
	protected: void mWS(bool _createToken);
private:
	
	static const unsigned long _tokenSet_0_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_0;
	static const unsigned long _tokenSet_1_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_1;
};

#endif /*INC_L_hpp_*/
