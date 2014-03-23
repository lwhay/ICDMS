/**
 * @file DNFPredGen.cpp
 * @date April 11, 2007
 * @brief This file defines DNFPredGen class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include <set>
#include <cassert>
using namespace std;

#include "P2TokenTypes.hpp"
#include "DNFPredGen.h"

using namespace Cayuga::QLCompiler;

#include "../ICDSMSCE/AutomatonCompiler/ConjunctiveClauseString.h"
#include "../ICDSMSCE/AutomatonCompiler/Predicates_DNFString.h"
#include "../ICDSMSCE/AutomatonCompiler/StaAtomPredString.h"
#include "../ICDSMSCE/AutomatonCompiler/DynAtomPredString.h"
#include "../ICDSMSCE/AutomatonCompiler/DurationPredString.h"
#include "../ICDSMSCE/AutomatonCompiler/ComplexAtomPredString.h"

#include "../ICDSMSCE/BasicDataStructure/Predicate/AttributeID.h"

#include "../ICDSMSCE/SystemMgr/DataTypeMgr.h"
#include "../ICDSMSCE/SystemMgr/StreamSchemaMgr.h"

using namespace Cayuga::AutomatonCompiler;
using namespace Cayuga::BasicDataStructure::Predicate;
using namespace Cayuga::SystemMgr;

#include "Utils/RAssert.h"
using namespace Cayuga::Utils;

#include "SystemMsg.h"
using namespace Cayuga::SystemMsg;


//DNFPredGen::DNFPredGen(void) 
//{
//}
//
//DNFPredGen::~DNFPredGen(void)
//{
//}

void DNFPredGen::visit(RefCayugaQLAST& thisAST) {
	//logic to recursively process predicates.
	//This will be done in a bottom-up fashion.
	//visit all the children first
	for (RefCayugaQLAST childAST = (RefCayugaQLAST)thisAST->getFirstChild(); 
		 childAST; childAST = childAST->getNextSibling()) {
		visit(childAST);
	}

	//we start compilation in two types of nodes: UNARY_OPS and STREAM_EXPR
	//In the former case, the sibling of UNARY_OPS (a primaryExpr node)
	//has the schema, and compilation is done w.r.t. that schema. The generated
	//DNF is stored in the UNARY_OPS node.
	//In the latter case, if the sibling of STREAM_EXPR is binOp, then
	//compilation is done w.r.t. the schema of the STREAM_EXPR node and the 
	//sibling of the located binOp node, and the generated DNF is stored in
	//the binOp node.
	if (thisAST->getType() == P2TokenTypes::UNARY_OPS
		|| thisAST->getType() == P2TokenTypes::STREAM_EXPR) {
		//transform this node as well
		_compilePredicate(thisAST);

	}
}



void DNFPredGen::_compilePredicate(RefCayugaQLAST& uOpsOrStreamExprAST) {
	if (uOpsOrStreamExprAST->getType() == P2TokenTypes::UNARY_OPS) {
		_compileUnaryOpsPredicate(uOpsOrStreamExprAST);
	}
	else {
		Assert (uOpsOrStreamExprAST->getType() == P2TokenTypes::STREAM_EXPR);
		RefCayugaQLAST lhsUnaryExprAST 
			= (RefCayugaQLAST)uOpsOrStreamExprAST->getFirstChild();
		RefCayugaQLAST binOpAST 
			= (RefCayugaQLAST)lhsUnaryExprAST->getNextSibling();
		if (!binOpAST)
			return;

		Assert (!lhsUnaryExprAST->_thisSchema.empty());
		
		RefCayugaQLAST rhsUnaryExprAST 
			= (RefCayugaQLAST)binOpAST->getNextSibling();
		Assert (!rhsUnaryExprAST->_thisSchema.empty());

		if (binOpAST->getType() == P2TokenTypes::SEQ_OP) {
			RefCayugaQLAST predExprAST 
				= (RefCayugaQLAST)binOpAST->getFirstChild();

			//luoluo 8-1
			//the predicatewindow
			if (predExprAST->getType()==P2TokenTypes::PRED_WINDOW)
			{
				predExprAST = (RefCayugaQLAST)predExprAST->getNextSibling();
			}

			binOpAST->_thisFilterDNF = _compileDNF(predExprAST, 
				binOpAST->_thisSchema, 
				rhsUnaryExprAST->_thisSchema,
				SEQ_PARAM);

		}
		else {
			Assert (binOpAST->getType() == P2TokenTypes::MU_OP);
			RefCayugaQLAST childAST 
				= (RefCayugaQLAST)binOpAST->getFirstChild();
			
			//If mu parameter could be empty, in which case we just return
			if (!childAST) {
				return;
			}

			//luoluo 7-30
			//the predicatewindow
			if (childAST->getType()==P2TokenTypes::PRED_WINDOW)
			{
				childAST = (RefCayugaQLAST)childAST->getNextSibling();
			}

			//filter predicate
			if (childAST->getType() != P2TokenTypes::COMMA) {
				binOpAST->_thisFilterDNF = _compileDNF(childAST, 
					binOpAST->_thisSchema, 
					rhsUnaryExprAST->_thisSchema,
					MU_PARAM);

				childAST = (RefCayugaQLAST)childAST->getNextSibling();
				assert (childAST->getType() == P2TokenTypes::COMMA);
			}

			//get the next child of the first comma
			childAST = (RefCayugaQLAST)childAST->getNextSibling();
			if (!childAST) {
				return;
			}

			//rebind edge
			if (childAST->getType() != P2TokenTypes::COMMA) {
				//rebind predicate is stored in binOp
				binOpAST->_thisRebindDNF = _compileDNF(childAST, 
					binOpAST->_thisSchema, 
					rhsUnaryExprAST->_thisSchema,
					MU_PARAM);

				childAST = (RefCayugaQLAST)childAST->getNextSibling();
				assert (childAST->getType() == P2TokenTypes::COMMA);
			}

			//get the next child of the second comma
			childAST = (RefCayugaQLAST)childAST->getNextSibling();
			if (!childAST) {
				return;
			}
			
			//if there are UDF terms associated with this mu, they will have
			//been dealt with in TypeChecker::_processStreamExpr
		}
	}
}

void DNFPredGen::_compileUnaryOpsPredicate(
	RefCayugaQLAST& uOpsAST) {
		RefCayugaQLAST primaryExprAST 
			= (RefCayugaQLAST)uOpsAST->getNextSibling();
		//primaryExprAST is a primaryExpr node
		Assert (primaryExprAST->getType() == P2TokenTypes::STREAM_NAME
			|| primaryExprAST->getType() == P2TokenTypes::QUERY);
		//the schema should have been produced
		Assert (!primaryExprAST->_thisSchema.empty());

		uOpsAST->_thisFilterDNF = _compileSigmaPred(uOpsAST,
			primaryExprAST->_thisSchema);
}

Predicates_DNFString* DNFPredGen::_compileSigmaPred(RefCayugaQLAST& uOpsAST,
				 const ASTSchema& schema) {
	RefCayugaQLAST filterExprAST = (RefCayugaQLAST)uOpsAST->getFirstChild();
	//the normalized predicate should have only one filterExpr node as the child
	//of unaryOps node
	Assert (filterExprAST->getType() == P2TokenTypes::FILTER_EXPR
		&& filterExprAST->getNextSibling() == NULL);
	RefCayugaQLAST predExprAST = (RefCayugaQLAST)filterExprAST->getFirstChild();
	ASTSchema NullSchema;
	return _compileDNF(predExprAST, 
		schema, 
		NullSchema, 
		SIGMA_PARAM);
}

Predicates_DNFString* DNFPredGen::_compileDNF(
	RefCayugaQLAST& predExprAST,
	const ASTSchema& lhsSchema,
	const ASTSchema& rhsSchema,
	AttributeLocation attrLoc) {
	if (!predExprAST) return NULL;
	Assert (predExprAST->getType() == P2TokenTypes::PRED_EXPR);

	Predicates_DNFString* thisDNF = new Predicates_DNFString();

	//may have other silbings that are OR nodes
	for(RefCayugaQLAST andExprAst = (RefCayugaQLAST)predExprAST->getFirstChild(); 
		andExprAst; andExprAst = andExprAst->getNextSibling()) {
		//each child should be AND_EXPR
		Assert (andExprAst->getType() == P2TokenTypes::AND_EXPR);
		ConjunctiveClauseString* thisCC 
			= _compileCC(andExprAst, lhsSchema, rhsSchema, attrLoc);
		thisDNF->addClause(thisCC);
	}
	return thisDNF;
}

ConjunctiveClauseString* DNFPredGen::_compileCC(
	RefCayugaQLAST& andPredAST,
	const ASTSchema& lhsSchema,
	const ASTSchema& rhsSchema,
	AttributeLocation attrLoc) {
	ConjunctiveClauseString* thisCC = new ConjunctiveClauseString();

	//may have other silbings that are AND nodes
	int numDurAtomPreds = 0;
	for(RefCayugaQLAST atomPredAST 
		= (RefCayugaQLAST)andPredAST->getFirstChild(); atomPredAST; 
		atomPredAST = (RefCayugaQLAST)atomPredAST->getNextSibling()) {
		//each child should be ATOM_PRED or DUR_PRED
		AtomPredString* thisAtom 
			= _compileAtomPred(atomPredAST, lhsSchema, rhsSchema, attrLoc);
		thisCC->addAtomPred(thisAtom);
		if (atomPredAST->getType() == P2TokenTypes::DUR_PRED) {
			++numDurAtomPreds;
		}

		if (numDurAtomPreds > 1) {
			printError(NOT_IMPLEMENTED, 
				"multiple duration atomic predicates in the same predicate.");
		}
	}
	return thisCC;
}

void DNFPredGen::_getLHS_CompOp_RHS_FromAtomPred(
	const RefCayugaQLAST& atomPredAST,
	RefCayugaQLAST& lhsTermAST,
	RefCayugaQLAST& compOpAST,
	RefCayugaQLAST& rhsTermAST) {
	Assert (atomPredAST->getType() == P2TokenTypes::ATOM_PRED);

	lhsTermAST = (RefCayugaQLAST)atomPredAST->getFirstChild();
	Assert (lhsTermAST->getType() == P2TokenTypes::PLUS_MINUS_EXPR);

	compOpAST = (RefCayugaQLAST)lhsTermAST->getNextSibling();
	
	rhsTermAST = (RefCayugaQLAST)compOpAST->getNextSibling();
	Assert (rhsTermAST->getType() == P2TokenTypes::PLUS_MINUS_EXPR);

	Assert (rhsTermAST->getNextSibling() == NULL);
}

AtomPredString* DNFPredGen::_qualifyForStaticPredicateAndCompile(
	RefCayugaQLAST& atomPredAST,
	const ASTSchema& lhsSchema,
	const ASTSchema& rhsSchema,
	AttributeLocation attrLoc) {
	RefCayugaQLAST lhsTermAST, compOpAST, rhsTermAST;
	_getLHS_CompOp_RHS_FromAtomPred(atomPredAST, lhsTermAST, 
		compOpAST, rhsTermAST);

	//the logic below assumes stat pred is in normal form: 
	//LHS comes from evente schema, and RHS is a const. 
	//If it is the other way around, will swap LHS and RHS 
	string compOp = compOpAST->getText();
	if (termIsConst(lhsTermAST) && termIsAttrExpr(rhsTermAST)) {
		RefCayugaQLAST tmpTermExprAST = rhsTermAST;
		rhsTermAST = lhsTermAST;
		lhsTermAST = tmpTermExprAST;

		_reverse(compOp);
	}

	RefCayugaQLAST lhsAttrExprAST = termIsAttrExpr(lhsTermAST);
	RefCayugaQLAST rhsConstAST = termIsConst(rhsTermAST);

	///check LHS
	if (lhsAttrExprAST == NULL)
		return NULL;

	//check RHS
	if (!rhsConstAST)
		return NULL;

	if (!_attrIsFromEventSchema(lhsAttrExprAST, attrLoc))
		return NULL;
	
	UnionDataType u;
	getConstTypeAndValue(rhsConstAST, u);

	string lhsNfaAttrName = 
		lhsAttrExprAST->convertQLAttrNameTonNfaAttrName(
		lhsSchema, rhsSchema, attrLoc);
	return new StaAtomPredString(
		lhsNfaAttrName, 
		compOp, 
		u._val);
}

AtomPredString* DNFPredGen::_qualifyForDynamicPredicateAndCompile(
	RefCayugaQLAST& atomPredAST,
	const ASTSchema& lhsSchema,
	const ASTSchema& rhsSchema,
	AttributeLocation attrLoc) {
	RefCayugaQLAST lhsTermAST, compOpAST, rhsTermAST;
	_getLHS_CompOp_RHS_FromAtomPred(atomPredAST, lhsTermAST, 
		compOpAST, rhsTermAST);

	RefCayugaQLAST lhsAttrExprAST = termIsAttrExpr(lhsTermAST);
	RefCayugaQLAST rhsAttrExprAST = termIsAttrExpr(rhsTermAST);

	///check LHS
	if (lhsAttrExprAST == NULL)
		return NULL;

	//check RHS
	if (!rhsAttrExprAST)
		return NULL;

	//the logic below assumes dyn pred is in normal form: 
	//LHS comes from node schema, and RHS comes from event schema. 
	//If it is the other way around, will swap LHS and RHS 
	string compOp = compOpAST->getText();
	if (_attrIsFromEventSchema(lhsAttrExprAST, attrLoc) 
		&& _attrIsFromNodeSchema(rhsAttrExprAST, attrLoc)) {
			RefCayugaQLAST tmpAttrExprAST = rhsAttrExprAST;
			rhsAttrExprAST = lhsAttrExprAST;
			lhsAttrExprAST = tmpAttrExprAST;
	}
	else {
		//if not qualified for swapping, will check whether 
		if (!_attrIsFromNodeSchema(lhsAttrExprAST, attrLoc))
			return NULL;


		if (!_attrIsFromEventSchema(rhsAttrExprAST, attrLoc))
			return NULL;

		//if these are all passed, will need to reverse the compOp since
		//the CEL predicate is $1.foo > $2.bar.
		//the compiled dyn pred in AIR is will be bar < foo
		_reverse(compOp);

	}

	//the following segment is just to make sure the dynamic predicate
	//is well form: lhs is node attr, and rhs is event attr
	
	//get the nfa attribute names
	string lhsNfaAttrName = 
		lhsAttrExprAST->convertQLAttrNameTonNfaAttrName(
		lhsSchema, rhsSchema, attrLoc);

	string rhsNfaAttrName = 
		rhsAttrExprAST->convertQLAttrNameTonNfaAttrName(
		lhsSchema, rhsSchema, attrLoc);

	return new DynAtomPredString(
		rhsNfaAttrName,  //event attr
		compOp, 
		lhsNfaAttrName //node attr
		);
}

void DNFPredGen::_reverse (
	string& compOp) {
	if (compOp == "=") {
		compOp = "=";
	}
	else if (compOp == "!=") {
		compOp = "!=";
	}
	else if (compOp == "<=") {
		compOp = ">=";
	}
	else if (compOp == "<") {
		compOp = ">";
	}
	else if (compOp == ">=") {
		compOp = "<=";
	}
	else if (compOp == ">") {
		compOp = "<";
	}
	else {
		Assert (false);
	}

	//will not reach here
}


AtomPredString* DNFPredGen::_qualifyForDurationPredicateAndCompile(
	RefCayugaQLAST& atomPredAST,
	const ASTSchema& lhsSchema,
	const ASTSchema& rhsSchema,
	AttributeLocation attrLoc) {
	RefCayugaQLAST compOpAST = (RefCayugaQLAST)atomPredAST->getFirstChild();
	RefCayugaQLAST durationConstAST 
		= (RefCayugaQLAST)compOpAST->getNextSibling();

	//time unit is optional
	RefCayugaQLAST timeUnitAST 
		= (RefCayugaQLAST)durationConstAST->getNextSibling();

	//convert the duration predicate contant to a normalized constant
	//in Cayuga time unit 
	//according to the optionally input time unit
	string durationConst = durationConstAST->getText();
	if (timeUnitAST != NULL) {
		const string strTimeUnit = timeUnitAST->getText();
		durationConst = _convertDurationConst(durationConst, strTimeUnit);
	}

	string durPredType = (attrLoc == SIGMA_PARAM) ? "EVENT" : "COMPOSITE";

	return new DurationPredString(durPredType,
		compOpAST->getText(), 
		durationConst);
}

string DNFPredGen::_convertDurationConst(
	const string& strConst,
	const string& timeUnit) {
	printWarning(DURATION_PRED, TIME_UNIT_USED);

	//Will add the implementation later
	if (timeUnit == "year" || timeUnit == "YEAR") {
	}
	if (timeUnit == "month" || timeUnit == "MONTH") {
	}
	if (timeUnit == "day" || timeUnit == "DAY") {
	}
	if (timeUnit == "hour" || timeUnit == "HOUR") {
	}
	if (timeUnit == "min" || timeUnit == "MIN") {
	}
	if (timeUnit == "sec" || timeUnit == "SEC") {
	}
	
	return strConst;
}

AtomPredString* DNFPredGen::_qualifyForComplexPredicateAndCompile(
	RefCayugaQLAST& atomPredAST,
	const ASTSchema& lhsSchema,
	const ASTSchema& rhsSchema,
	AttributeLocation attrLoc) {
	RefCayugaQLAST lhsTermAST, compOpAST, rhsTermAST;
	_getLHS_CompOp_RHS_FromAtomPred(atomPredAST, lhsTermAST, 
		compOpAST, rhsTermAST);

	return  _compileComplexPredicate(
	lhsTermAST, rhsTermAST,compOpAST->getText(),
	lhsSchema, rhsSchema, attrLoc);
}

bool DNFPredGen::_attrIsFromEventSchema(
	const RefCayugaQLAST& attrAST,
	AttributeLocation attrLoc) {
	AttributeDecorator attrDec;
	string attrName;
	attrAST->getAttrVarNumAndName(attrDec, attrName);

	switch (attrLoc) {
			case SIGMA_PARAM: 
				return (attrDec == DOLLAR_1 || attrDec == DOLLAR_FREE);
			case SEQ_PARAM:
			case MU_PARAM:
				return (attrDec == DOLLAR_2);
			case SELECT_CLAUSE:
				//cannot happen for a predicate
				Assert (false);
		}

		//will not reach here
		return false;
}

bool DNFPredGen::_attrIsFromNodeSchema(
	const RefCayugaQLAST& attrAST,
	AttributeLocation attrLoc) {
	AttributeDecorator attrDec;
	string attrName;
	attrAST->getAttrVarNumAndName(attrDec, attrName);

	switch (attrLoc) {
			case SEQ_PARAM:
				return (attrDec == DOLLAR_FREE || attrDec == DOLLAR_1);
			case MU_PARAM:
				return (attrDec == DOLLAR_FREE || attrDec == DOLLAR_1 
					|| attrDec == DOLLAR);
			case SIGMA_PARAM: 
				Assert (attrDec == DOLLAR_FREE || attrDec == DOLLAR_1);
				return false;
			case SELECT_CLAUSE:
		//cannot happen for a predicate
		Assert (false);
		}

	//will not reach here
	return false;
}

AtomPredString* DNFPredGen::_compileAtomPred(
	RefCayugaQLAST& atomPredAST,
	const ASTSchema& lhsSchema,
	const ASTSchema& rhsSchema,
	AttributeLocation attrLoc) {
	AtomPredString* thisAtom = NULL;

	if (atomPredAST->getType() == P2TokenTypes::ATOM_PRED) {
		if (thisAtom = _qualifyForStaticPredicateAndCompile(
			atomPredAST, lhsSchema, rhsSchema, attrLoc)) {
				return thisAtom;
		}
		else if (thisAtom = _qualifyForDynamicPredicateAndCompile(
			atomPredAST, lhsSchema, rhsSchema, attrLoc)) {
				return thisAtom;
		}
		else {
			thisAtom = _qualifyForComplexPredicateAndCompile(
				atomPredAST, lhsSchema, rhsSchema, attrLoc);
			return thisAtom;
		}
	}
	else {
		Assert (atomPredAST->getType() == P2TokenTypes::DUR_PRED);
		return _qualifyForDurationPredicateAndCompile(
			atomPredAST, lhsSchema, rhsSchema, attrLoc);
		}
}

ComplexAtomPredString* DNFPredGen::_compileComplexPredicate(
	RefCayugaQLAST& lhsTermAST, 
	RefCayugaQLAST& rhsTermAST,
	const string& compOp,
	const ASTSchema& lhsSchema, 
	const ASTSchema& rhsSchema,
	AttributeLocation attrLoc) {
	//return NULL;

	string predExpression;
	lhsTermAST->write(predExpression, 
			lhsSchema, rhsSchema, 
			attrLoc);

	predExpression += compOp;

	rhsTermAST->write(predExpression, 
			lhsSchema, rhsSchema, 
			attrLoc);

	return new ComplexAtomPredString(predExpression);
}
