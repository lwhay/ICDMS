#include <exception>
#include <cassert>
using namespace std;

#include "P2TokenTypes.hpp"
#include "CayugaQLASTWalker.h"
using namespace Cayuga::QLCompiler;

#include "../ICDSMSCE/Utils/RAssert.h"
using namespace Cayuga::Utils;

RefCayugaQLAST CayugaQLASTWalker::termIsSimpleFactor(RefCayugaQLAST& termAST) {
	Assert (termAST->getType() == P2TokenTypes::PLUS_MINUS_EXPR);
	RefCayugaQLAST mTermExpr = (RefCayugaQLAST)termAST->getFirstChild();
	Assert (mTermExpr->getType() == P2TokenTypes::MUL_DIV_EXPR);
	//term should have only one child to be candidate for being simple.
	//Since otherwise it will look like mTerm1 +/- mTerm2, which is not an
	//*simple*
	if (mTermExpr->getNextSibling()) return NULL;

	RefCayugaQLAST factor = (RefCayugaQLAST)mTermExpr->getFirstChild();
	Assert (factor->getType() == P2TokenTypes::EXP_MOD_EXPR);
	//mTermExpr should have only one child to be candidate for being simple
	if (factor->getNextSibling()) return NULL;

	RefCayugaQLAST factor2 = (RefCayugaQLAST)factor->getFirstChild();
	//factor should have only one child to be candidate for being simple
	if (factor2->getNextSibling()) return NULL;

	return factor2;

}


RefCayugaQLAST CayugaQLASTWalker::termIsAttrExpr(RefCayugaQLAST& termAST) {
	RefCayugaQLAST factor2 = termIsSimpleFactor(termAST);

	if (!factor2 || factor2->getType() != P2TokenTypes::ATTR_EXPR)
		return NULL;

	return factor2;
}

RefCayugaQLAST CayugaQLASTWalker::termIsConst(RefCayugaQLAST& termAST) {
	RefCayugaQLAST factor2 = termIsSimpleFactor(termAST);

	if (!factor2 || factor2->getType() != P2TokenTypes::CONST_LIT_EXPR)
		return NULL;

	return factor2;
}

void CayugaQLASTWalker::getConstTypeAndValue(RefCayugaQLAST& constAST,
											 UnionDataType& outU) {
	RefCayugaQLAST constLitAST = (RefCayugaQLAST)constAST->getFirstChild();
	outU._type = constLitAST->getType();
	outU._val = constLitAST->getText();
}


RefCayugaQLAST CayugaQLASTWalker::getStreamExprChildFromQuery(
	RefCayugaQLAST& queryAST) {
	Assert (queryAST->getType() == P2TokenTypes::QUERY);
	RefCayugaQLAST childAST = (RefCayugaQLAST)queryAST->getFirstChild();
	if (childAST->getType() == P2TokenTypes::SELECT_CLAUSE)
		childAST = childAST->getNextSibling();

	//added by rainbow
	if (childAST->getType()==P2TokenTypes::WINDOW_CLAUSE)
	{
		childAST=childAST->getNextSibling();
	}

	Assert (childAST->getType() == P2TokenTypes::STREAM_EXPR);
	return childAST;
}

RefCayugaQLAST CayugaQLASTWalker::getSelectClauseChildFromQuery(
	RefCayugaQLAST& queryAST) {
	Assert (queryAST->getType() == P2TokenTypes::QUERY);
	RefCayugaQLAST childAST = (RefCayugaQLAST)queryAST->getFirstChild();
	if (childAST->getType() == P2TokenTypes::SELECT_CLAUSE)
		return childAST;
	else {
		RefAST newSelectClauseAST = CayugaQLAST::factory();
		newSelectClauseAST->setType(P2TokenTypes::SELECT_CLAUSE);

		queryAST->setFirstChild(newSelectClauseAST);
		newSelectClauseAST->setNextSibling((RefAST)childAST);

		RefAST newTermExprAST = CayugaQLAST::factory();
		newTermExprAST->setType(P2TokenTypes::MUL);
		newTermExprAST->setText("*");
		newSelectClauseAST->setFirstChild((RefAST)newTermExprAST);

		return (RefCayugaQLAST)newSelectClauseAST;
	}
}

RefCayugaQLAST CayugaQLASTWalker::getWindowClauseChildFromQuery(RefCayugaQLAST &queryAST){
	Assert(queryAST->getType()==P2TokenTypes::QUERY);
	RefCayugaQLAST childAST=(RefCayugaQLAST)queryAST->getFirstChild();
	if (childAST->getType()==P2TokenTypes::SELECT_CLAUSE)
	{
		childAST=childAST->getNextSibling();
	}
	if(childAST->getType()==P2TokenTypes::WINDOW_CLAUSE){
		return childAST;
	}
	return NULL;

}

//luoluo 7-29
RefCayugaQLAST CayugaQLASTWalker::getPredWindowClauseChildFromQuery(RefCayugaQLAST& binOpAST)
{
     Assert(binOpAST->getType()==P2TokenTypes::SEQ_OP || binOpAST->getType() == P2TokenTypes::MU_OP);
	RefCayugaQLAST childAST = (RefCayugaQLAST)binOpAST->getFirstChild();
	if(childAST->getType()==P2TokenTypes::PRED_WINDOW)
	return childAST;
	else return NULL;
}



bool CayugaQLASTWalker::isSimpleUnaryExpr(RefCayugaQLAST& unaryExprAST) {
	Assert (unaryExprAST->getType() == P2TokenTypes::UNARY_EXPR);

	RefCayugaQLAST childAST 
		= (RefCayugaQLAST)unaryExprAST->getFirstChild();
	if (childAST->getType() == P2TokenTypes::STREAM_NAME) {
		return true;
	}

	return false;
}

string CayugaQLASTWalker::getStreamNameFromSimpleUnaryExpr(
	RefCayugaQLAST& unaryExprAST) {
	Assert(isSimpleUnaryExpr(unaryExprAST));

	RefCayugaQLAST childAST 
		= (RefCayugaQLAST)unaryExprAST->getFirstChild();
	return childAST->getText();
}

bool CayugaQLASTWalker::isSimpleQueryExpr(RefCayugaQLAST& queryAST) {
	Assert (queryAST->getType() == P2TokenTypes::QUERY);
	RefCayugaQLAST selectClauseAST = getSelectClauseChildFromQuery(queryAST);

	RefCayugaQLAST streamExprAST = getStreamExprChildFromQuery(queryAST);

	RefCayugaQLAST outputClauseAST 
		= (RefCayugaQLAST)streamExprAST->getNextSibling();

	if (!outputAllAttributes(selectClauseAST)) return false;

	if (outputClauseAST) return false;

	RefCayugaQLAST unaryExprAST 
		= (RefCayugaQLAST)streamExprAST->getFirstChild();

	if (unaryExprAST->getNextSibling()) return false;

	return isSimpleUnaryExpr(unaryExprAST);
}

string CayugaQLASTWalker::getStreamNameFromSimpleQueryExpr(
	RefCayugaQLAST& queryAST) {
	Assert(isSimpleQueryExpr(queryAST));

	RefCayugaQLAST streamExprAST = getStreamExprChildFromQuery(queryAST);

	RefCayugaQLAST unaryExprAST 
		= (RefCayugaQLAST)streamExprAST->getFirstChild();

	return getStreamNameFromSimpleUnaryExpr(unaryExprAST);
}

bool CayugaQLASTWalker::outputAllAttributes(
	const RefCayugaQLAST& selectClauseAST) {
	if (!selectClauseAST) return true;
	
	const RefCayugaQLAST childAST 
		= (const RefCayugaQLAST)selectClauseAST->getFirstChild();

	if (!childAST->getNextSibling() && childAST->getText() == "*")
		return true;

	return false;
}

RefCayugaQLAST CayugaQLASTWalker::getBinOpChildOfStreamExpr(
	const RefCayugaQLAST& StreamExprAST) {
	Assert (StreamExprAST->getType() == P2TokenTypes::STREAM_EXPR);
	RefCayugaQLAST childAST = (RefCayugaQLAST)StreamExprAST->getFirstChild();

	Assert (childAST != NULL);
	return (RefCayugaQLAST)childAST->getNextSibling();
}


//if the unaryExpr has a query child, return it directly.
//otherwise make a new query node and put the unary expr as descendant
//of the query node.
RefCayugaQLAST CayugaQLASTWalker::unaryExpr2Query(
	RefCayugaQLAST& unaryExprAST) {
	RefCayugaQLAST childAST 
		= (RefCayugaQLAST)unaryExprAST->getFirstChild();
	if (childAST->getType() == P2TokenTypes::UNARY_OPS 
		|| childAST->getType() == P2TokenTypes::STREAM_NAME) {
			//make a new node
			RefAST newQueryAST = CayugaQLAST::factory();
			newQueryAST->setType(P2TokenTypes::QUERY);

			RefAST newStreamExprAST	= CayugaQLAST::factory();
			newStreamExprAST->setType(P2TokenTypes::STREAM_EXPR);

			newQueryAST->setFirstChild(newStreamExprAST);
			newStreamExprAST->setFirstChild((RefAST)unaryExprAST);

			//set schema for the RHS query
			Assert (!unaryExprAST->_thisSchema.empty());
			((RefCayugaQLAST)newQueryAST)->_thisSchema 
				= unaryExprAST->_thisSchema;
			
			//copy schema map into the select clause now since 
			//processQuery will need it
			RefCayugaQLAST selectClauseAST 
				= getSelectClauseChildFromQuery((RefCayugaQLAST&)newQueryAST);
			Assert (!((RefCayugaQLAST)newQueryAST)->_thisSchema.empty());
			selectClauseAST->_thisSchema 
				= ((RefCayugaQLAST)newQueryAST)->_thisSchema;

			return (RefCayugaQLAST)newQueryAST;
	}
	else {
		//return the query child directly
		Assert (childAST->getType() == P2TokenTypes::QUERY);
		return childAST;
	}
}
