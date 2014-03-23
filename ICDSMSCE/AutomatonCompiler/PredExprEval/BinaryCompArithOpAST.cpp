/**
* @file BinaryCompArithOpAST.cpp
* @date November 30, 2005
* @brief This file defines class BinaryCompArithOpAST.
* @author Mingsheng Hong (mshong@cs.cornell.edu)
*/

#include "AutomatonCompiler/PredExprEval/BinaryCompArithOpAST.h"
using namespace Cayuga::AutomatonCompiler::PredExprEval;

#include "PTokenTypes.hpp"
using namespace antlr;

#include <cassert>
#include <string>
using namespace std;

using namespace antlr;

void BinaryCompArithOpAST::initialize( RefToken t ) {
	BasicAST::initialize(t);
	switch (t->getType()) {
			 case PTokenTypes::EQ: 
				 _opType = ByteCodeNS::EQ;
				 break;
			 case PTokenTypes::NE: 
				 _opType = ByteCodeNS::NE;
				 break;
			 case PTokenTypes::OP_LT: 
				 _opType = ByteCodeNS::LT;
				 break;
			 case PTokenTypes::LE: 
				 _opType = ByteCodeNS::LE;
				 break;
			 case PTokenTypes::GT: 
				 _opType = ByteCodeNS::GT;
				 break;
			 case PTokenTypes::GE: 
				 _opType = ByteCodeNS::GE;
				 break;

			 case PTokenTypes::PLUS: 
				 _opType = ByteCodeNS::PLUS;
				 break;
			 case PTokenTypes::MINUS: 
				 _opType = ByteCodeNS::MINUS;
				 break;
			 case PTokenTypes::MUL: 
				 _opType = ByteCodeNS::MUL;
				 break;
			 case PTokenTypes::DIV: 
				 _opType = ByteCodeNS::DIV;
				 break;
			 case PTokenTypes::MOD: 
				 _opType = ByteCodeNS::MOD;
				 break;
			 case PTokenTypes::EXP: 
				 _opType = ByteCodeNS::EXP;
				 break;
			 default:
				 assert(false);
	}
}

RefAST BinaryCompArithOpAST::clone( void ) {
	RefAST ret = new BinaryCompArithOpAST(*this);
	return ret;
}

RefAST BinaryCompArithOpAST::factory( void ) {
	RefAST ret = new BinaryCompArithOpAST();
	return ret;
}

void BinaryCompArithOpAST::compile(Context& context,
	vector<pair<char*, size_t> >& programOut) {
	ByteCodeNS::ByteCode dataType = getType(context);

	(RefBasicAST(getFirstChild()))->compile(context, programOut);

	/*
	#ifdef _DEBUG
	if (dataType == ByteCodeNS::INT) {
	cout<<"INT ";
	}
	else if(dataType == ByteCodeNS::FLOAT) {
	cout<<"FLOAT ";
	}
	else {
	cout<<"STRING ";
	}

	if (_opType <= ByteCodeNS::GT) {
	cout<<"COMPARISON"<<endl;
	}
	else {
	cout<<"ARITH"<<endl;
	}
	#endif //_DEBUG
	*/

	size_t szInstr = sizeof(ByteCodeNS::ByteCode)*2;
	char* pInstruction = new char[szInstr];
	size_t ip = 0;
	setValToProgram<ByteCodeNS::ByteCode>(pInstruction, ip, _opType);
	setValToProgram<ByteCodeNS::ByteCode>(pInstruction, ip, dataType);
	programOut.push_back(make_pair(pInstruction, szInstr));

	if (getNextSibling()) {
	   (RefBasicAST(getNextSibling()))->compile(context, programOut);
	}

}

ByteCodeNS::ByteCode BinaryCompArithOpAST::getType(Context& context) {
	assert (getFirstChild());
	ByteCodeNS::ByteCode left = (RefBasicAST(getFirstChild()))->getType(context);

	if (!getNextSibling()) {
		return left;
	}

	ByteCodeNS::ByteCode right = (RefBasicAST(getNextSibling()))->getType(context);
	if (left == ByteCodeNS::STRING) {
		assert (right == ByteCodeNS::STRING);
		return ByteCodeNS::STRING;
	}

	if (left == ByteCodeNS::INT) {
		return right;
	}
	else {
		return ByteCodeNS::FLOAT;
	}
}

void BinaryCompArithOpAST::renameAttrRef(const SchemaMapString* schemaMap) {
	RefAST childAST = getFirstChild();
	assert (childAST);
	(RefBasicAST(childAST))->renameAttrRef(schemaMap);

	if (childAST->getNextSibling()) {
		(RefBasicAST(childAST->getNextSibling()))->renameAttrRef(schemaMap);
	}
}

void BinaryCompArithOpAST::getReferredAttributes(set<string>& referredAttrs) {
	RefAST childAST = getFirstChild();
	assert (childAST);
	(RefBasicAST(childAST))->getReferredAttributes(referredAttrs);

	if (childAST->getNextSibling()) {
		(RefBasicAST(childAST->getNextSibling()))->
			getReferredAttributes(referredAttrs);
	}
}

void BinaryCompArithOpAST::write(ostream& out) {
	RefAST childAST = getFirstChild();
	assert (childAST);
	(RefBasicAST(childAST))->write(out);

	out<<getText();

	if (childAST->getNextSibling()) {
		(RefBasicAST(childAST->getNextSibling()))->write(out);
	}
}
