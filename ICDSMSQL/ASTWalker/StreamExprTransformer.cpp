/**
 * @file StreamExprTransformer.cpp
 * @date April 8, 2007
 * @brief This file defines StreamExprTransformer class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "StreamExprTransformer.h"

using namespace Cayuga::QLCompiler;

#include "P2TokenTypes.hpp"

#include "../ICDSMSCE/Utils/RAssert.h"
using namespace Cayuga::Utils;


//StreamExprTransformer::StreamExprTransformer(void)
//{
//}
//
//StreamExprTransformer::~StreamExprTransformer(void)
//{
//}

void StreamExprTransformer::visit(RefCayugaQLAST& rootAST)
{
	//logic to get the first streamExpr from root
	RefCayugaQLAST streamExprAST;
	if (rootAST->getType() == P2TokenTypes::QUERY) {
		streamExprAST = getStreamExprChildFromQuery(rootAST);
	}
	else {
		Assert (rootAST->getType() == P2TokenTypes::UNARY_EXPR);
		streamExprAST = _getStreamExprChildFromUnaryExpr(rootAST);
		if (!streamExprAST)
			return;
	}

	//Since streamExpr could be recursive, will process the tree bottom up -- 
	//descendants are transformed before ancestors.
	//This is guaranteed by making sure that rootAST is either a QUERY node,
	//or a unaryExpr node.

	//process its descendants first
	//remember the children AST of streamExprAST, so that we can scan them
	//backwards
	vector<RefCayugaQLAST> childrenAST;
	for (RefCayugaQLAST childAST = (RefCayugaQLAST)streamExprAST->getFirstChild();
		childAST; childAST = childAST->getNextSibling()) {
		childrenAST.push_back(childAST);
		
		//childAST will be a unaryExpr, recursively call this function
		Assert (childAST->getType() == P2TokenTypes::UNARY_EXPR);
		visit(childAST);

		//the next node must be a binary operator or NULL
		childAST = childAST->getNextSibling();
		if (!childAST) {
			//we have transformed all descendant children of streamExprAST
			break;
		}

		Assert (childAST->getType() == P2TokenTypes::SEQ_OP
			|| childAST->getType() == P2TokenTypes::MU_OP);
		//luoluo 7-29
// 		if (childAST->getType() == P2TokenTypes::BIN_OP)
// 		{
// 			childAST = childAST->getFirstChild();
// 			 Assert (childAST->getType() == P2TokenTypes::SEQ_OP
// 				|| childAST->getType() == P2TokenTypes::MU_OP);
// 		}
		childrenAST.push_back(childAST);
	}

	//now we process streamExprAST
	//if streamExprAST is the following sequence of children:
	//uexpr1 binop1 uexpr2 binop2 uexpr3,
	//then the transformed result should be:
	//sexpr1 binop2 uexpr3, where sexpr1 has three children
	//sexpr2 binop1 uexpr2, where sexpr2 has one child uexpr1.
	//In other words, the new grammar for streamExpr becomes
	//streamExpr -> streamExpr binOp unaryExpr | unaryExpr
	Assert (childrenAST.size() % 2 == 1);
	if (childrenAST.size() <= 1) {
		//no transformation needed
	}
	else {
		//get the third to last child, which is a unaryExpr
		//will pull all the children before it (including it) into a sub-node
		RefCayugaQLAST curStreamExpr = streamExprAST;
		for (int child_idx = (int)childrenAST.size() - 3; child_idx >= 0; 
		child_idx -= 2) {
			RefCayugaQLAST childStreamExpr = (RefCayugaQLAST)CayugaQLAST::factory();
			childStreamExpr->setType(P2TokenTypes::STREAM_EXPR);

			//set child link
			curStreamExpr->setFirstChild((RefAST)childStreamExpr);
			childStreamExpr->setFirstChild((RefAST)childrenAST[0]);

			//set sibling link
			childrenAST[child_idx]->setNextSibling(NULL);
			childStreamExpr->setNextSibling((RefAST)childrenAST[child_idx+1]);

			curStreamExpr = childStreamExpr;
		}
	}
}

RefCayugaQLAST StreamExprTransformer::_getStreamExprChildFromUnaryExpr
(RefCayugaQLAST& unaryExprAST) {
	//a unaryExpr node has children of type streamExpr, query, streamName
	//or unaryOps
	RefCayugaQLAST childAST = (RefCayugaQLAST)unaryExprAST->getFirstChild();
	if (childAST->getType() == P2TokenTypes::UNARY_OPS) {
		childAST = childAST->getNextSibling();
	}

	//childAST must be a primaryExpr
	return _getStreamExprChildFromPrimaryExpr(childAST);
}

RefCayugaQLAST StreamExprTransformer::_getStreamExprChildFromPrimaryExpr
(RefCayugaQLAST& primaryExprAST) {
	if (primaryExprAST->getType() == P2TokenTypes::QUERY) {
		return getStreamExprChildFromQuery(primaryExprAST);
	}
	else {
		Assert (primaryExprAST->getType() == P2TokenTypes::STREAM_NAME);
		//rootAST does not have any streamExpr to transform.
		//Mission accomplished!
		return NULL;
	}
}
