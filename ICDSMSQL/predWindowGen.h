#pragma once

#include "CayugaQLASTWalker.h"

using namespace std;

namespace Cayuga{
	namespace QLCompiler{
		/**
		* @brief	A Window object walks the AST and does generates windowStrings
						of the AST nodes.
		*/
		class predWindowGen : public CayugaQLASTWalker{
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