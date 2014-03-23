/**
* @file WindowString.h
* @date March 24, 2011
* @brief This file is used to generate a window expression in the query.
* @author Rainbow (rainbowsworld@163.com)
*/
#pragma once

#include "CayugaQLASTWalker.h"

using namespace std;

namespace Cayuga{
	namespace QLCompiler{
		/**
		* @brief A Window object walks the AST and does generates windowStrings
		of the AST nodes.
		* @author Rainbow
		* @date March 24, 2011
		* 
		*/
		class WindowGen : public CayugaQLASTWalker{
		public:
			/**
			* @brief Visit the AST.
			* @param rootAst The root of the input AST.
			* @invariant rootAst must contain at most one windowClause child.
			*/
			virtual void visit(RefCayugaQLAST& rootAst);

		};
	}
}