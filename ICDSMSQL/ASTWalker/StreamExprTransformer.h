/**
 * @file StreamExprTransformer.h
 * @date April 8, 2007
 * @brief This file declares StreamExprTransformer class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#pragma once

#include "CayugaQLASTWalker.h"

namespace Cayuga {
namespace QLCompiler {
/**
 * @brief A StreamExprTransformer object walks the AST and does tranformation
			on the StreamExpr type of nodes.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date April 8, 2007
 * @version 1.0
 */
class StreamExprTransformer : public CayugaQLASTWalker
{
public:
	//StreamExprTransformer(void);
	//~StreamExprTransformer(void);

	/**
	* @brief Visit the AST.

		Recursively "unflatten" streamExpr (sexp in short) nodes as follows.
			
		The antlr generated AST has the following structure for sexp nodes.
		sexp ::= uexp (binop uexp)+
			
		This class creates nested StreamExpr expressions, effectively
		transforming the grammar to sexp ::= sexp binop uexpr, 
			
	* @param rootAst The root of the input AST.
	* @invariant rootAst must contain at most one streamExpr child.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 8, 2007
	* @version 1.0
	*/
	virtual void visit(RefCayugaQLAST& rootAst);

private:
	//can return NULL if there is no such child
	RefCayugaQLAST _getStreamExprChildFromUnaryExpr(
		RefCayugaQLAST& unaryExprAST);
	
	//can return NULL if there is no such child
	RefCayugaQLAST _getStreamExprChildFromPrimaryExpr(
		RefCayugaQLAST& primaryExprAST);
};//class StreamExprTransformer
} //namespace QLCompiler
} //namespace Cayuga
