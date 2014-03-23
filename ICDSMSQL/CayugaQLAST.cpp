/**
 * @file CayugaQLAST.cpp
 * @date April 4, 2007
 * @brief This file defines CayugaQLAST class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "CayugaQLAST.h"
#include "P2TokenTypes.hpp"

using namespace Cayuga::QLCompiler;

#include "SystemMsg.h"
using namespace Cayuga::SystemMsg;

#include "../ICDSMSCE/AutomatonCompiler/ByteCodeUtils.h"
#include "../ICDSMSCE/AutomatonCompiler/SchemaMapString.h"
using namespace Cayuga::AutomatonCompiler;

#include "../ICDSMSCE/BasicDataStructure/ByteCode.h"
using namespace Cayuga::BasicDataStructure;

#include "../ICDSMSCE/SystemMgr/FuncMgr.h"
using namespace Cayuga::SystemMgr;

#include "../ICDSMSCE/Utils/RAssert.h"
using namespace Cayuga::Utils;

CayugaQLAST::CayugaQLAST( void )  /*: lineNumber(0)*/ {
	initVars();
}

CayugaQLAST::CayugaQLAST (const CayugaQLAST& rhs) 
: CommonAST(rhs) /*, lineNumber(rhs.lineNumber)*/ {
	initVars();
}

CayugaQLAST::CayugaQLAST(RefToken t) : CommonAST(t) {
	initVars();
	//setLine(t->getLine() );
}

CayugaQLAST::~CayugaQLAST( void ) { 
	//delete _thisForwardEdgeSchemaMap;
	//delete _thisRebindEdgeSchemaMap;
}



//the node must have at least one child
RefCayugaQLAST CayugaQLAST::getLastChild() const
{
	RefCayugaQLAST childAST = (RefCayugaQLAST)getFirstChild();

	Assert (childAST != NULL);

	while (childAST->getNextSibling())
		childAST = (RefCayugaQLAST)childAST->getNextSibling();
	return childAST;
}

RefAST CayugaQLAST::clone( void ) {
	RefAST ret = new CayugaQLAST(*this);
	return ret;
}

RefAST CayugaQLAST::factory( void ) {
	RefAST ret = new CayugaQLAST();
	return ret;
}

void CayugaQLAST::initVars() {
	//_thisSchema = NULL;

	//_usedSchema = _demandSchema = NULL;

	_thisFilterDNF = NULL;
	_thisRebindDNF = NULL;
	//_thisCC = NULL;
	//_thisAtomPred = NULL;

	_thisQuery = NULL;
	_thisWindow= NULL;

	//luoluo 7-30
	_thisPredWindow = NULL;

	_thisForwardEdgeSchemaMap = new SchemaMapString();
	_thisRebindEdgeSchemaMap = new SchemaMapString();
}

void CayugaQLAST::addUDF(const string& outAttrName, 
						 const string& termExpression) {
	_udfNameExprMap.insert(make_pair(outAttrName, termExpression));
}

bool CayugaQLAST::findUDF(const string& udfName) {
	return _udfNameExprMap.find(udfName) != _udfNameExprMap.end();
}

bool CayugaQLAST::noUDF() {
	return _udfNameExprMap.empty();
}

string CayugaQLAST::getUDFExpr(const string& udfName) {
	map<string, string>::iterator find_it = _udfNameExprMap.find(udfName);
	if (find_it == _udfNameExprMap.end()) {
		//cerr<<"error: cannot find UDF name "<<udfName<<endl;
		throw "error: cannot find UDF name " + udfName;
	}
	return find_it->second;
}

//writes a complex UDF expression to termExpression, using attribute names
//in schema to resolve names
void CayugaQLAST::write(string& termExpression, 
						const ASTSchema& lhsSchema,
						const ASTSchema& rhsSchema,
						AttributeLocation attrLoc) {
	//termExpression = "1+2";
	RefCayugaQLAST childAST;
	//int attrVarNum = -1;
	string attrName;
	switch (getType()) {
		case P2TokenTypes::PLUS_MINUS_EXPR:
		case P2TokenTypes::MUL_DIV_EXPR:
		case P2TokenTypes::EXP_MOD_EXPR:
			childAST = (RefCayugaQLAST)getFirstChild();

			if (childAST->getNextSibling()) {
				termExpression += "(";
			}
			childAST->write(termExpression, lhsSchema, rhsSchema, attrLoc);
			
			if (childAST->getNextSibling()) {
				termExpression += ")";
			}
			
			childAST = (RefCayugaQLAST)childAST->getNextSibling();
			if (childAST) {
				termExpression += childAST->getText();
				childAST = (RefCayugaQLAST)childAST->getNextSibling();
				Assert (childAST != NULL);

				termExpression += "(";
				childAST->write(termExpression, lhsSchema, rhsSchema, attrLoc);
				termExpression += ")";
			}
			break;
		case P2TokenTypes::MINUS:
			printError(NOT_IMPLEMENTED, "");
			break;
		case P2TokenTypes::CONST_LIT_EXPR:
			childAST = (RefCayugaQLAST)getFirstChild();
			Assert (childAST != NULL);
			//child AST is either number of quoted string
			//for quoted string, need to add ' and '
			if (childAST->getType() == P2TokenTypes::QUOTED_STRING) 
				termExpression += "'";
			termExpression += childAST->getText();
			if (childAST->getType() == P2TokenTypes::QUOTED_STRING) 
				termExpression += "'";
			break;
		case P2TokenTypes::ATTR_EXPR:
			attrName = convertQLAttrNameTonNfaAttrName(
				lhsSchema, rhsSchema, attrLoc);
			if (_bAttrFromEvent) {
				termExpression += "EV.";
			}
			else {
				termExpression += "NODE.";
			}
			termExpression += attrName;
			break;
		case P2TokenTypes::FUNC_EXPR:
			childAST = (RefCayugaQLAST)getFirstChild();

			//write the function name
			termExpression += childAST->getText();

			termExpression += "(";
			for (childAST = (RefCayugaQLAST)childAST->getNextSibling(); 
				childAST;
				childAST = (RefCayugaQLAST)childAST->getNextSibling()) {
				//write each term as the input argument of the function
				childAST->write(termExpression, lhsSchema, rhsSchema, attrLoc);

				//skip COMMA
				childAST = (RefCayugaQLAST)childAST->getNextSibling();
				if (childAST == NULL)
					break;
				Assert (childAST->getType() == P2TokenTypes::COMMA);

				termExpression += ", ";

			}
			termExpression += ")";
			break;
		default:
			cerr<<"AST node type "<<getType()<<" should not occur here."<<endl;
			Assert (false);
			break;
	}
}

//return the data type of the expression rooted here
ByteCodeNS::ByteCode CayugaQLAST::getDataType(const ASTSchema& lhsSchema,
											  const ASTSchema& rhsSchema,
											  AttributeLocation attrLoc) {
	//return "int";
	RefCayugaQLAST childAST;
	AttributeDecorator attrDec;
	string inAttrName;
	ByteCodeNS::ByteCode lhsDataType, rhsDataType;
	switch (getType()) {
		case P2TokenTypes::PLUS_MINUS_EXPR:
		case P2TokenTypes::MUL_DIV_EXPR:
		case P2TokenTypes::EXP_MOD_EXPR:
			childAST = (RefCayugaQLAST)getFirstChild();
			lhsDataType = childAST->getDataType(lhsSchema, rhsSchema, attrLoc);

			childAST = (RefCayugaQLAST)childAST->getNextSibling();
			if (childAST) {
				childAST = (RefCayugaQLAST)childAST->getNextSibling();

				rhsDataType = childAST->getDataType(lhsSchema, 
					rhsSchema, 
					attrLoc);

				return combineDataType(lhsDataType, rhsDataType);
			}
			else {
				return lhsDataType;
			}
			break;
		case P2TokenTypes::MINUS:
			childAST = (RefCayugaQLAST)getFirstChild();
			return childAST->getDataType(lhsSchema, rhsSchema, attrLoc);
			break;
		case P2TokenTypes::CONST_LIT_EXPR:
			childAST = (RefCayugaQLAST)getFirstChild();
			Assert (childAST != NULL);
			//child AST is either number of quoted string
			switch (childAST->getType()) {
		case P2TokenTypes::INT:
			return ByteCodeNS::INT;
		case P2TokenTypes::FLOAT:
			return ByteCodeNS::FLOAT;
		case P2TokenTypes::QUOTED_STRING:
			return ByteCodeNS::STRING;
		default:
			Assert(false);
			}
			break;
		case P2TokenTypes::ATTR_EXPR:
			//resolve attr name with the input schema
			getAttrVarNumAndName(attrDec, inAttrName);

			//the logic is similar to DNFPredGen::compileAtomPred
			//will refactor
			if (attrDec == DOLLAR_FREE ||attrDec == DOLLAR_1) {
				//life is simple: find the name from the schema alias
				Assert (lhsSchema.findName(inAttrName) == true);
				return convertStringToByteCodeType(
					lhsSchema.getDataType(inAttrName));
			}
			else if (attrDec == DOLLAR_2) {
				return convertStringToByteCodeType(
					rhsSchema.getDataType(inAttrName));
			}
			else if (attrDec == DOLLAR) {
				//$ is specified in mu predicate
				Assert (lhsSchema.findAliasName(inAttrName + "_n") == true);
				Assert (lhsSchema.findName(inAttrName) == true);
				return convertStringToByteCodeType(
					lhsSchema.getDataType(inAttrName));
			}
			break;
		case P2TokenTypes::FUNC_EXPR:
			childAST = (RefCayugaQLAST)getFirstChild();

			//write the function name
			return FuncMgr::getInstance()->getReturnType(childAST->getText());

			//TODO: type check the function arguments
			//for (childAST = (RefCayugaQLAST)childAST->getNextSibling(); 
			//	childAST;
			//	childAST = (RefCayugaQLAST)childAST->getNextSibling()) {
			//	//write each term as the input argument of the function
			//	childAST->write(termExpression, schema);

			//}
			break;
		default:
			break;
	}

	cerr<<"AST node type "<<getType()<<" should not occur here."<<endl;
	Assert (false);
	//just to suppress compiler warning
	return ByteCodeNS::INVALID;
}


void CayugaQLAST::getAttrVarNumAndName(AttributeDecorator& outAttrDecorator, 
									   string& outAttrName) {
	RefCayugaQLAST childAST = (RefCayugaQLAST)getFirstChild();
	if (childAST->getType() == P2TokenTypes::ATTR_VAR_MARKER) {
		RefCayugaQLAST nextChildAST 
			= (RefCayugaQLAST)childAST->getNextSibling();
		if (nextChildAST->getType() == P2TokenTypes::INT) {
			if (nextChildAST->getText() == "1") {
				outAttrDecorator = DOLLAR_1;
			}
			else {
				Assert (nextChildAST->getText() == "2");
				outAttrDecorator = DOLLAR_2;
			}
			//outAttrVarNum = atoi(nextChildAST->getText().c_str());

			//the next sibling must store the input attribute name
			nextChildAST = (RefCayugaQLAST)nextChildAST->getNextSibling();
		}
		else {
			//$ is specified but without a number
			//outAttrVarNum = 0;
			outAttrDecorator = DOLLAR;
		}

		Assert (nextChildAST->getType() == P2TokenTypes::STRING);
		outAttrName = nextChildAST->getText();
	}
	else {
		//$ is not specified
		//outAttrVarNum = -1;
		outAttrDecorator = DOLLAR_FREE;
		Assert (childAST->getType() == P2TokenTypes::STRING);
		outAttrName = childAST->getText();
	}
}

//1. Specifying $1 is always the same as not specifying $ (called $-free), 
	//and they can be used in all places. We will return the alias name of foo
	//in lhsSchema, exception in the case of mu, where we always return foo_1, 
	//and make sure it is an alias in lhsSchema.
	//2. $2 can be used in sequence or mu predicate/UDF. In both cases, 
	//we return the alias name of foo in rhsSchema.
	//3. $ can only be used in mu predicate/UDF. We always return
	//foo_n, and make sure it is an alias in lhsSchema.
	//invariant: this AST node must be of type AttrExpr
string CayugaQLAST::convertQLAttrNameTonNfaAttrName(const ASTSchema& lhsSchema,
													const ASTSchema& rhsSchema,
													AttributeLocation attrLoc) {
	string inAttrName;
	AttributeDecorator attrDec;
	//resolve attr name with the input schema
	getAttrVarNumAndName(attrDec, inAttrName);

	//the logic is similar to DNFPredGen::compileAtomPred
	//will refactor
	if (attrDec == DOLLAR_FREE ||attrDec == DOLLAR_1) {
		switch (attrLoc) {
			case SELECT_CLAUSE:
			case SIGMA_PARAM:
				_bAttrFromEvent = true;
				return  lhsSchema.getAliasName(inAttrName);
			case SEQ_PARAM:
				_bAttrFromEvent = false;
				return  lhsSchema.getAliasName(inAttrName);
			case MU_PARAM:
				inAttrName += "_1";
				Assert (lhsSchema.findAliasName(inAttrName) == true);
				_bAttrFromEvent = false;
				return inAttrName;
			default:
				Assert (false);
		}
	} 
	else if (attrDec == DOLLAR_2) {
		switch (attrLoc) {
			case SEQ_PARAM:
			case MU_PARAM:
				Assert (rhsSchema.findAliasName(inAttrName) == true);
				_bAttrFromEvent = true;
				return  rhsSchema.getAliasName(inAttrName);
			case SELECT_CLAUSE:
			case SIGMA_PARAM:
			default:
				Assert (false);
		}
	}
	else {
		Assert (attrDec == DOLLAR);
		Assert (attrLoc == MU_PARAM);
		//$ is specified in mu predicate
		inAttrName += "_n";
		Assert (lhsSchema.findAliasName(inAttrName) == true);
		_bAttrFromEvent = false;
		return inAttrName;
	}

	//just to suppress compiler warning
	return "";
}
