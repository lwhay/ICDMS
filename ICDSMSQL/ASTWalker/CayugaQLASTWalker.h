/**
 * @file CayugaQLASTWalker.h
 * @date April 4, 2007
 * @brief This file declares CayugaQLASTWalker class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#pragma once

#include "CayugaQLAST.h"


using namespace antlr;

namespace Cayuga {
namespace QLCompiler {
/**
 * @brief A UnionDataType is a data type object used in Cayuga CEL compiler.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date April 4, 2007
 * @version 1.0
 */	
struct UnionDataType {
	string _val;
	int _type;
};

/**
 * @brief CayugaQLASTWalker is an interface for each concrete AST walker.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date April 4, 2007
 * @version 1.0
 */
class CayugaQLASTWalker
{
public:
	CayugaQLASTWalker(void) {
	}
	virtual ~CayugaQLASTWalker(void) {
	}

	/**
	* @brief Visit the AST.
	* @param rootAst The root of the input AST.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 4, 2007
	* @version 1.0
	*/
	virtual void visit(RefCayugaQLAST& rootAst) = 0;
	
protected:
	//Query AST
	RefCayugaQLAST getStreamExprChildFromQuery(RefCayugaQLAST& queryAST);

	/**
	* @brief Get a SELECT_CLAUSE child from the input QUERY node.
			If there is no such node, will construct one at the appropriate
			position determined by the CEL language grammar, whose default 
			value is SELECT *.
	* @param queryAST The input AST rooted at a QUERY node.
	* @invariant The input node must be of type QUERY.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 8, 2007
	* @version 1.0
	*/
	RefCayugaQLAST getSelectClauseChildFromQuery(RefCayugaQLAST& queryAST);

	//return the windowClause of the query
    RefCayugaQLAST getWindowClauseChildFromQuery(RefCayugaQLAST& queryAST);

	//luoluo 7-22
	//return the predwindowclause of the query
	RefCayugaQLAST getPredWindowClauseChildFromQuery(RefCayugaQLAST& binOpAST);

	//Return the stream name from the input unary expression.
	//invariant: this input unary expression must be a simple expression
	string getStreamNameFromSimpleUnaryExpr(RefCayugaQLAST& unaryExprAST);

	//checks whether the input query expresssion is a simple one that can be
	//inlined in an automaton. Currently it is simple only if it is an identity
	//query that does not publish any stream. I.e., it only has a streamExpr 
	//child node that expands to stream name
	bool isSimpleQueryExpr(RefCayugaQLAST& queryAST);
	
	//Return the stream name from the input query expression.
	//invariant: this input query expression must be a simple expression
	string getStreamNameFromSimpleQueryExpr(RefCayugaQLAST& queryAST);

	//SelectClause AST

	//return true if the select clause is missing or is select *
	bool outputAllAttributes(const RefCayugaQLAST& selectClauseAST);
	
	//StreamExpr AST

	//return the binOp child of a streamExpr input AST node. Return NULL
	//if there is no such binOp child.
	//pre condition: the input must be a streamExpr AST node.
	RefCayugaQLAST getBinOpChildOfStreamExpr(
		const RefCayugaQLAST& StreamExprAST);

	//UnaryExpr AST

	//checks whether the input expression is a simple one that can be inlined
	//in an automaton. Will be used in generating NFA for binary operator.
	//Currently an expression is simple only when it is a stream name.
	//Will expand it to sigma operator applied to stream name in the future.
	bool isSimpleUnaryExpr(RefCayugaQLAST& unaryExprAST);

	
	//A term is simple if it is not essentially of type factor2 in the 
	//CEL grammar. If the input term is simple, return a reference to that
	//factor2 AST node. Otherwise return NULL.
	RefCayugaQLAST termIsSimpleFactor(RefCayugaQLAST& termAST);

	//If the term is of type attrVar in the grammar, return it.
	//otherwise return NULL
	RefCayugaQLAST termIsAttrExpr(RefCayugaQLAST& termAST);

	RefCayugaQLAST termIsConst(RefCayugaQLAST& termAST);

	//const AST
	void getConstTypeAndValue(RefCayugaQLAST& constAST, UnionDataType& outU);		
	RefCayugaQLAST unaryExpr2Query(RefCayugaQLAST& unaryExprAST);

protected:
	enum SchemaType {
		FOR_MU_FIRST, //attributes in first iteration of mu
		FOR_MU_LAST,  //attributes in last iteration of mu
		FOR_SEQ,	  //attributes for SEQ that do not require rename
		FOR_SEQ_RENAME, //attributes for SEQ that require adding _1 to names
	};
};
}
}
