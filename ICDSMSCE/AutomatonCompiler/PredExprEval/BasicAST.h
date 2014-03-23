/**
 * @file BasicAST.h
 * @date November 30, 2005
 * @brief This file declares class BasicAST.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef __BASIC_AST__
#define __BASIC_AST__


#include "CommonAST.hpp"

#include <set>
#include <string>
#include <vector>
using namespace std;

using namespace antlr;


#include "BasicDataStructure/ByteCode.h"
using namespace Cayuga::BasicDataStructure;

#include "AutomatonCompiler/ByteCodeUtils.h"
using namespace Cayuga::AutomatonCompiler;

namespace Cayuga {

namespace AutomatonCompiler {
class Context;
class SchemaMapString;

namespace PredExprEval {
class BasicAST;
typedef ASTRefCount<BasicAST> RefBasicAST;
/**
 * @brief A BasicAST object represents a base 
		  type of AST node for a complex predicate in AIR.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 30, 2005
 * @version 1.0
 */
class BasicAST : public CommonAST {

public:
	//post order traversal code, useful for efficient stack based evaluation
	//in left-child, right-sibling tree, post order traversal should be
	//1.first child
	//2.self
	//3.next sibling
	virtual void compile(Context& context,
		vector<pair<char*, size_t> >& programOut) = 0;
	
	virtual ByteCodeNS::ByteCode getType(Context& context) = 0;

	virtual void renameAttrRef(const SchemaMapString* schemaMap) {
	}

	virtual void getReferredAttributes(set<string>& referredAttrs) {
	}

	virtual void write(ostream& out = cout) = 0;

};//class BasicAST
} //namespace PredExprEval
} //namespace AutomatonCompiler 
} //namespace Cayuga


#endif //__BASIC_AST__
