/**
* @file DataTypeAST.cpp
* @date November 30, 2005
* @brief This file defines class DataTypeAST.
* @author Mingsheng Hong (mshong@cs.cornell.edu)
*/

#include "AutomatonCompiler/PredExprEval/DataTypeAST.h"
using namespace Cayuga::AutomatonCompiler::PredExprEval;

#include "AutomatonCompiler/PredExprEval/PTokenTypes.hpp"
using namespace antlr;

#include <string>
using namespace std;

#include "MemoryMgr/CY_GCFace.h"
using namespace CY_Heap;

void DataTypeAST::initialize( RefToken t ) {
	BasicAST::initialize(t);

	string str, strWithoutQuotes;
	switch (t->getType()) {
			 case PTokenTypes::INT: 
				 _value._type = ByteCodeNS::INT;
				 _value._i = atoi(t->getText().c_str());
				 break;
			 case PTokenTypes::FLOAT:
				 _value._type = ByteCodeNS::FLOAT;
				 _value._f = (float)atof(t->getText().c_str());
				 break;
			 case PTokenTypes::STRING:
				 _value._type = ByteCodeNS::STRING;
				 str = t->getText();
				 strWithoutQuotes = str.substr(1, str.size()-2);
				 //_value._s = lookup(strWithoutQuotes.c_str(), 1);
				 _value._s = lookup(strWithoutQuotes, 
					 true,				//need to install
					 true				//for external space	
					 );

				 //need to ping predicate constants
				 registerRootObject(_value._s);
				 break;
			 default:
				 assert (false);
	}
}

RefAST DataTypeAST::clone( void ) {
	RefAST ret = new DataTypeAST(*this);
	return ret;
}

RefAST DataTypeAST::factory( void ) {
	RefAST ret = new DataTypeAST();
	return ret;
}

void DataTypeAST::compile(Context& context,
						  vector< pair<char*, size_t> >& programOut) {
	assert (getFirstChild() == NULL);

	/*
	#ifdef _DEBUG
	if (_value._type == ByteCodeNS::INT) {
	cout<<"INT ";
	}
	else if(_value._type == ByteCodeNS::FLOAT) {
	cout<<"FLOAT ";
	}
	else {
	cout<<"STRING ";
	}
	cout<<"DATA TYPE"<<endl;
	#endif //_DEBUG
	*/

	size_t len;
	switch (_value._type) {
	case ByteCodeNS::INT:
		len = sizeof(int);
		break;
	case ByteCodeNS::FLOAT:
		len = sizeof(float);
		break;
	case ByteCodeNS::STRING:
		len = sizeof(IntStringHandle); 
		break;
	default:
		assert(false);
	}

	//type + value in the array
	size_t szInstr = sizeof(ByteCodeNS::ByteCode)*2 + len; 
	char* pInstruction = new char[szInstr];

	size_t ip = 0;
	setValToProgram<ByteCodeNS::ByteCode>(
		pInstruction, ip, ByteCodeNS::CONSTANT);
	setValToProgram<ByteCodeNS::ByteCode>(pInstruction, ip, _value._type);

	switch (_value._type) {
	case ByteCodeNS::INT:
		setValToProgram<int>(pInstruction, ip, _value._i);
		break;
	case ByteCodeNS::FLOAT:
		setValToProgram<float>(pInstruction, ip, _value._f);
		break;
	case ByteCodeNS::STRING:
		setValToProgram<IntStringHandle>(pInstruction, ip, _value._s);
		break;
	default:
		assert(false);
	}

	programOut.push_back(make_pair(pInstruction, szInstr));

	if (getNextSibling()) {
	  (RefBasicAST(getNextSibling()))->compile(context, programOut);
	}
}

ByteCodeNS::ByteCode DataTypeAST::getType(Context& context) {
	ByteCodeNS::ByteCode attrType =  _value._type;

	assert (!getFirstChild());

	if (!getNextSibling()) {
		return attrType;
	}

	ByteCodeNS::ByteCode right = (
		RefBasicAST(getNextSibling()))->getType(context);
	if (attrType == ByteCodeNS::STRING) {
		assert (right == ByteCodeNS::STRING);
		return ByteCodeNS::STRING;
	}
	if (attrType == ByteCodeNS::INT) {
		return right;
	}
	else {
		return ByteCodeNS::FLOAT;
	}
}

void DataTypeAST::write(ostream& out) {
	out<<getText();
}
