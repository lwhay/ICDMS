/**
 * @file TypeChecker.cpp
 * @date April 8, 2007
 * @brief This file defines TypeChecker class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

//#include <cassert>
#include <set>
using namespace std;

#include "P2TokenTypes.hpp"
#include "TypeChecker.h"

using namespace Cayuga::QLCompiler;

#include "../ICDSMSCE/AutomatonCompiler/ByteCodeUtils.h"
using namespace Cayuga::AutomatonCompiler;

#include "../ICDSMSCE/SystemMgr/DataTypeMgr.h"
#include "../ICDSMSCE/SystemMgr/StreamSchemaMgr.h"
using namespace Cayuga::SystemMgr;

#include "../ICDSMSCE/Utils/AttrMap.h"
#include "../ICDSMSCE/Utils/RAssert.h"
using namespace Cayuga::Utils;

#include "../ICDSMSCE/SystemMsg.h"
using namespace Cayuga::SystemMsg;

int TypeChecker::_uniqueIDcnt = 0;

//TypeChecker::TypeChecker(void) : _uniqueIDcnt(0)
//{
//}
//
//TypeChecker::~TypeChecker(void)
//{
//}

void TypeChecker::visit(RefCayugaQLAST& rootAST) {
	_processQuery(rootAST);
	//rootAST->_thisSchema->write();
}

ASTSchema TypeChecker::_processQuery(RefCayugaQLAST& queryAST) {
	Assert (queryAST->getType() == P2TokenTypes::QUERY);

	RefCayugaQLAST streamExprAST = getStreamExprChildFromQuery(queryAST);
	ASTSchema streamExprSchema = _processStreamExpr(streamExprAST);

	//produce the schema for select clause from streamExprSchema
	RefCayugaQLAST selectClauseAST = getSelectClauseChildFromQuery(queryAST);
	Assert (selectClauseAST != NULL);
	ASTSchema outputSchema 
		= _processSelectClause(selectClauseAST, streamExprSchema);

	//now we modify outputSchema to set name to its alias, since outsider
	//of the query shouldn't be able to refer to the names any more.
	outputSchema.setNameToAlias();

	RefCayugaQLAST resultAST = (RefCayugaQLAST)streamExprAST->getNextSibling();
	_processResultClause(resultAST, outputSchema);

	queryAST->_thisSchema = outputSchema;

	return outputSchema;
}

ASTSchema TypeChecker::_processSelectClause(
	RefCayugaQLAST& selectAST,
	ASTSchema& streamExprSchema) {
	bool bSelectStar = false;
	for (RefCayugaQLAST termExprAST 
		= (RefCayugaQLAST)selectAST->getFirstChild(); termExprAST; 
		termExprAST = termExprAST->getNextSibling()) {
		if (termExprAST->getType() == P2TokenTypes::TERM_EXPR) {
			string inAttrName, outAttrName, attrType;
			_getAttrNameTypeOrUDFFromTermExprInSelectClause(
				inAttrName, outAttrName, attrType, 
				termExprAST, streamExprSchema, selectAST);
			selectAST->_thisSchema.insert(
				NameType(inAttrName, attrType, outAttrName));
		}
		else {
			//this is "select *"
			Assert (termExprAST->getType() == P2TokenTypes::MUL);
			selectAST->_thisSchema.append(streamExprSchema);

			//select * can only occur once in the query
			Assert (!bSelectStar);
			bSelectStar = true;
		}
	}

	return selectAST->_thisSchema;
}


void TypeChecker::_processResultClause(
	RefCayugaQLAST& resultAST,
	ASTSchema& outputSchema) {
	Assert (!resultAST || resultAST->getType() == P2TokenTypes::RESULT_CLAUSE); 

	if (resultAST) {
		resultAST->_thisSchema = outputSchema;
	}
}

void  TypeChecker::_getAttrNameTypeOrUDFFromTermExprInSelectClause(
	string& inAttrName,
	string& outAttrName,
	string& outAttrType,
	const RefCayugaQLAST& termExprAST,
	const ASTSchema& streamExprSchema,
	const RefCayugaQLAST& selectAST) {
	//this variable will be true iff the output attr name is not specified,
	//but the term is an attribute name itself
	bool bOutAttrNameIsInAttrName = false;
	RefCayugaQLAST termAST = (RefCayugaQLAST)termExprAST->getFirstChild();

	//set outAttrName
	//if output attribute name specified by AS clause, this is easy
	RefCayugaQLAST outAttrNameAST = (RefCayugaQLAST)termAST->getNextSibling();
	if (outAttrNameAST) {
		outAttrName = outAttrNameAST->getText();
	}
	else {
		//output attribute name is not specified
		//but the term is not an
		//attribute expression, then a fresh attr name will be assigned.
		RefCayugaQLAST attrExprAST = termIsAttrExpr(termAST);
		if (!attrExprAST) {
			printWarning(SELECT, FUNC_NOT_NAMED);
			printSolution(CREATE_NEW_ATTR_NAME_SELECT);

			///@bug: this ID may not be absolutely new, but is easy to
			//assign.
			outAttrName = "NewAttr" + toString(_uniqueIDcnt++);
		}
		else {
			bOutAttrNameIsInAttrName = true;
			//in this case outAttrName is not set yet, but will be set by
			//the case that reads bOutAttrNameIsInAttrName below
		}
	}

	//set outAttrType and inAttrName
	//handle the easy special case that term is ATTR_EXPR
	RefCayugaQLAST attrExprAST = termIsAttrExpr(termAST);
	if (attrExprAST) {
		//just need to ensure either the termAST
		//does not use $, or $ or $1 is used. Other cases such as $2
		//must be an error
		AttributeDecorator attrDec;
		attrExprAST->getAttrVarNumAndName(attrDec, inAttrName);
		
		if (attrDec != DOLLAR_FREE) {
			printWarning(SELECT, DOLLAR_USED);
			//$ or $1 must be used here
			Assert (attrDec == DOLLAR || attrDec == DOLLAR_1);
		}

		if (bOutAttrNameIsInAttrName) {
			outAttrName = inAttrName;
		}

		outAttrType = streamExprSchema.getDataType(inAttrName);
		return;
	}
	else {
		//the term is a complex function.
		//In this case outAttrName must have been set.
		//We only need to deduce the data type from the complex function
		Assert (!bOutAttrNameIsInAttrName);
		inAttrName = outAttrName;
		string termExpression;
		ASTSchema nullSchema;
		ByteCodeNS::ByteCode dataType = termAST->getDataType(
			streamExprSchema, nullSchema, SELECT_CLAUSE);
		outAttrType = convertByteCodeTypeToString(dataType);
		termAST->write(termExpression, 
			streamExprSchema, nullSchema, 
			SELECT_CLAUSE);
		selectAST->addUDF(outAttrName, termExpression);
	}
}

void  TypeChecker::_getUDFFromTermExprInMuParameter(
	const RefCayugaQLAST& termExprAST,
	const ASTSchema& nodeSchema,
	const ASTSchema& eventSchema,
	const RefCayugaQLAST& binOpAST) {
	string outAttrName;
	
	RefCayugaQLAST termAST = (RefCayugaQLAST)termExprAST->getFirstChild();

	//set outAttrName
	//assert in MU parameter, the output attribute name must be specified by 
	//AS clause
	RefCayugaQLAST outAttrNameAST = (RefCayugaQLAST)termAST->getNextSibling();
	Assert (outAttrNameAST != NULL);
	outAttrName = outAttrNameAST->getText();
	//this attribute name must be postfixed _n, since it 
	//is in the schema of mu state
	outAttrName += "_n"; 

	string termExpression;
	termAST->write(termExpression, 
		nodeSchema, eventSchema, 
		MU_PARAM);
	binOpAST->addUDF(outAttrName, termExpression);
}

//here we assume the transformed streamExpr has grammar
//streamExpr -> streamExpr binOp unaryExpr | unaryExpr
ASTSchema TypeChecker::_processStreamExpr(
	RefCayugaQLAST& streamExprAST) {
	int astType = streamExprAST->getType();
	Assert (astType == P2TokenTypes::STREAM_EXPR);
	RefCayugaQLAST unaryOrStreamExprAST 
		= (RefCayugaQLAST)streamExprAST->getFirstChild();
	if (unaryOrStreamExprAST->getType() == P2TokenTypes::UNARY_EXPR) {
		streamExprAST->_thisSchema = _processUnaryExpr(unaryOrStreamExprAST);
	}
	else {
		//this is a binary expression
		Assert (unaryOrStreamExprAST->getType() == P2TokenTypes::STREAM_EXPR);
		ASTSchema lhsSchema = _processStreamExpr(unaryOrStreamExprAST);

		RefCayugaQLAST binaryOpAST 
			= (RefCayugaQLAST)unaryOrStreamExprAST->getNextSibling();

		Assert (binaryOpAST != NULL);
		RefCayugaQLAST rhsAST 
			= (RefCayugaQLAST)binaryOpAST->getNextSibling();
			
		Assert (rhsAST != NULL);
		ASTSchema rhsSchema = _processUnaryExpr(rhsAST);
		
		//Now we need to combine the lhsSchema and rhsSchema with the 
		//binary operator to produce streamExprAST->_thisSchema, which 
		//corresponds to the end automaton state of this compiled binary 
		//operator,	and binaryExprAST->_thisSchema, which corresponds to 
		//the intermediate automaton state of this compiled binary operator
		if (binaryOpAST->getType() == P2TokenTypes::SEQ_OP) {
			//streamExpr stores the end result schema of sequence.
			streamExprAST->_thisSchema 
				= _combineSchemaForSEQEndState(lhsSchema, rhsSchema);
			//binOp stores the node schema of the NFA node with filter edge,
			//which implements this sequence operator.
			//without attribute elimination, we set its node schema to 
			//that of the LHS argument. However, still need to be be careful
			//with attribute name alias. 
			//If LHS and RHS of SEQ have overlapping attribute 
			//names, The alias will be att_1, and name will be att.
			SchemaType schematype 
				= (lhsSchema.intersection(rhsSchema))? FOR_SEQ_RENAME : FOR_SEQ;
			_appendSchema(binaryOpAST->_thisSchema, lhsSchema, 
				schematype, INTERMEDIATE);
		}
		else {
			Assert (binaryOpAST->getType() == P2TokenTypes::MU_OP);
			//streamExpr stores the end result schema of mu.
			//binOp stores the node schema of the NFA node with filter and 
			//rebind edge, which implements this mu operator.
			//without attribute elimination, we set its node schema 
			//as well as that of streamExpr to be the same: cross product 
			//between LHS and RHS of the mu operator
			//LHS argument
			binaryOpAST->_thisSchema 
				= _combineSchemaForMUIntermediateState(lhsSchema, rhsSchema);
			streamExprAST->_thisSchema = 
				_combineSchemaForMUEndState(binaryOpAST->_thisSchema);

			//handle the optional third parameter, UDF term here
			//the logic is similar to term handling in _processSelectClause
			//above

			//get the third term here, or if it does not exist, return NULL
			RefCayugaQLAST childAST 
				= _getFirstTermParameterFromMU(binaryOpAST);

			//if childAST is non NULL, then there is some UDF specs
			while (childAST) {
				Assert (childAST->getType() == P2TokenTypes::TERM_EXPR);
				_getUDFFromTermExprInMuParameter(
					childAST, //the term expr
					binaryOpAST->_thisSchema, //mu state schema 
					rhsSchema,					//event schema
					binaryOpAST);	

				//will not need to insert the UDF output attribute into the 
				//node schema since that attribute should already be there
				//from the LHS schema of mu


				childAST = (RefCayugaQLAST)childAST->getNextSibling();
				//go on for possibly multiple terms in MU
				if (childAST) {
					Assert (childAST->getType() == P2TokenTypes::COMMA);
					childAST = (RefCayugaQLAST)childAST->getNextSibling();
					Assert (childAST != NULL);
				}

			}
		}
	}
	return streamExprAST->_thisSchema;
}

RefCayugaQLAST TypeChecker::_getFirstTermParameterFromMU(
	RefCayugaQLAST& binaryOpAST) {
	Assert (binaryOpAST->getType() == P2TokenTypes::MU_OP);
	RefCayugaQLAST nullAST;
	RefCayugaQLAST childAST 
		= (RefCayugaQLAST)binaryOpAST->getFirstChild();
	//If mu parameter could be empty, in which case we just return
	//otherwise this is a predExpr for filter predicate
	if (!childAST) {
		return childAST;
	}

	//luoluo 7-30
	//the predicatewindow
	if (childAST->getType()==P2TokenTypes::PRED_WINDOW)
	{
		childAST = (RefCayugaQLAST)childAST->getNextSibling();
	}

	//could be filter predicate or comma
	if (childAST->getType() == P2TokenTypes::PRED_EXPR) {
		childAST = (RefCayugaQLAST)childAST->getNextSibling();
	}

	Assert (childAST->getType() == P2TokenTypes::COMMA);

	//get the next child of the first comma.
	//it could be a predExpr for rebind predicate
	childAST = (RefCayugaQLAST)childAST->getNextSibling();
	if (!childAST) {
		return childAST;
	}

	//could be rebind predicate or comma
	if (childAST->getType() == P2TokenTypes::PRED_EXPR) {
		childAST = (RefCayugaQLAST)childAST->getNextSibling();
	}
	Assert (childAST->getType() == P2TokenTypes::COMMA);

	//get the next child of the second comma
	childAST = (RefCayugaQLAST)childAST->getNextSibling();
	if (!childAST) {
		return childAST;
	}

	return childAST;
}

ASTSchema TypeChecker::_processPrimaryExpr(
	RefCayugaQLAST& primaryExprAST) {
	int astType = primaryExprAST->getType();
	Assert (astType == P2TokenTypes::STREAM_NAME
		|| astType == P2TokenTypes::QUERY);

	switch(astType) {
		case P2TokenTypes::STREAM_NAME:
			primaryExprAST->_inputStreamName = primaryExprAST->getText();
			primaryExprAST->_thisSchema.fromStreamSchema(
				StreamSchemaMgr::getInstance()->getStreamSchema(
				primaryExprAST->_inputStreamName));
			break;
		case P2TokenTypes::QUERY:
			_processQuery(primaryExprAST);
			break;
		default:
			Assert (false);
	}

	return primaryExprAST->_thisSchema;
}

ASTSchema TypeChecker::_processUnaryExpr(
	RefCayugaQLAST& unaryExprAST) {
		RefCayugaQLAST unaryOpsOrPrimaryExprAST 
			= (RefCayugaQLAST)unaryExprAST->getFirstChild();
		if (unaryOpsOrPrimaryExprAST->getType() == P2TokenTypes::UNARY_OPS) {
			//this next sibling must be a primaryExpr
			unaryOpsOrPrimaryExprAST 
				= (RefCayugaQLAST)unaryOpsOrPrimaryExprAST->getNextSibling();
		}

		//since for now unary ops only involve sigma, we set the output 
		//schema to be the same as streamExprSchema
		unaryExprAST->_thisSchema 
			= this->_processPrimaryExpr(unaryOpsOrPrimaryExprAST);

		return unaryExprAST->_thisSchema;
}

void TypeChecker::_appendSchema(ASTSchema& destSchema,
							   const ASTSchema& srcSchema,
							   SchemaType schematype,
							   NFAStateType stateType,
							   const ASTSchema* nextEventSchemaForMu) {
	for (ConstASTSchemaIterator it = srcSchema.begin(); it != srcSchema.end(); 
		++it) {
		string attrName = it.getName();
		string srcStateAlias = it.getAliasName();
		string attrType = srcSchema.getDataType(attrName);
		string destStateAlias = srcStateAlias;

		if (schematype == FOR_SEQ_RENAME
			|| (schematype == FOR_MU_FIRST && stateType == INTERMEDIATE
			&& nextEventSchemaForMu->findName(attrName))) {
			destStateAlias += "_1";
		}
		else if (schematype == FOR_MU_LAST
			|| (schematype == FOR_MU_FIRST  && stateType == INTERMEDIATE
			&& !nextEventSchemaForMu->findName(attrName))) {
				destStateAlias += "_n";
		}

		if (stateType == INTERMEDIATE) {
			destSchema.insert(
				NameType(srcStateAlias, attrType, destStateAlias));	
		}
		else {
			//for END state, aliasName will be used as attrName
			destSchema.insert(
				NameType(destStateAlias, attrType, destStateAlias));	
		}
	}
}

ASTSchema TypeChecker::_combineSchemaForSEQEndState(const ASTSchema& schema1,
				 const ASTSchema& schema2) {
	ASTSchema schema;
	SchemaType schematype 
		= (schema1.intersection(schema2))? FOR_SEQ_RENAME : FOR_SEQ;
	_appendSchema(schema, schema1, schematype, END);
	_appendSchema(schema, schema2, FOR_SEQ, END);
	return schema;
}

ASTSchema TypeChecker::_combineSchemaForMUEndState(
	const ASTSchema& muIntermediateNodeSchema) {
	ASTSchema schema;
	//algorithm for schema map on the forward edge?
	//first map attribute aliases in node schema1 with suffix_1 to dest schema
	//for those attribute aliases in node schema1 with suffix_n, only when they
	//do not occur in schema2 (event schema), will we map them too

	//algorithm for construction the schema for end node of mu
	//Simple: just map everything from schema1: if they have suffix _1,
	//keep that name, otherwise erase _n.
	for (ConstASTSchemaIterator it = muIntermediateNodeSchema.begin(); 
		it != muIntermediateNodeSchema.end(); ++it) {
		string attrName = it.getName();
		string aliasName = it.getAliasName();
		string attrType = muIntermediateNodeSchema.getDataType(attrName);
			
		//each attribute in schema1 must have suffix _1 or _n
		Assert (aliasName.size() > 2 && aliasName[aliasName.size()-2] == '_');
		if (aliasName[aliasName.size()-1] == '1') {
			schema.insert(NameType(aliasName, attrType, aliasName));
		}
		else {
			Assert (aliasName[aliasName.size()-1] == 'n');
			//just erase the suffix _n 
			string aliasWithoutSuffix = aliasName.substr(0, aliasName.size()-2);
			schema.insert(
				NameType(aliasWithoutSuffix, attrType, aliasWithoutSuffix));
		}
	}
	return schema;
}

ASTSchema TypeChecker::_combineSchemaForMUIntermediateState(
	const ASTSchema& schema1,
	const ASTSchema& schema2) {
	ASTSchema schema;
	Assert (schema1.contain(schema2));
	_appendSchema(schema, schema1, FOR_MU_FIRST, INTERMEDIATE, &schema2);
	_appendSchema(schema, schema2, FOR_MU_LAST, INTERMEDIATE);
	return schema;
}
