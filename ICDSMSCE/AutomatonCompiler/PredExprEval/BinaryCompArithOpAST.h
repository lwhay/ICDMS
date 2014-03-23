/**
 * @file BinaryCompArithOpAST.h
 * @date November 30, 2005
 * @brief This file declares class BinaryCompArithOpAST.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef __BINARY_COMP_ARITH_OP_AST__
#define __BINARY_COMP_ARITH_OP_AST__


#include "BasicAST.h"

#include <string>
using namespace std;

using namespace antlr;

namespace Cayuga {
namespace AutomatonCompiler {
namespace PredExprEval {
class BinaryCompArithOpAST;
typedef ASTRefCount<BinaryCompArithOpAST> RefBinaryCompArithOpAST;

/**
 * @brief A BinaryCompArithOpAST object represents a binary comparison or 
		  arithmetic operator type of AST node for a complex predicate in AIR.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 30, 2005
 * @version 1.0
 */
class BinaryCompArithOpAST : public BasicAST {
 public:
	 void initialize( RefToken t );

	 virtual RefAST clone( void );

	 static RefAST factory( void );

	 virtual void compile(Context& context,
		 vector<pair<char*, size_t> >& programOut);

	virtual ByteCodeNS::ByteCode getType(Context& context);

	virtual void renameAttrRef(const SchemaMapString* schemaMap);

	virtual void getReferredAttributes(set<string>& referredAttrs);

	virtual void write(ostream& out = cout);

 private:
	ByteCodeNS::ByteCode _opType;
};//class BinaryCompArithOpAST
} //namespace PredExprEval
} //namespace AutomatonCompiler 
} //namespace Cayuga

#endif //__BINARY_COMP_ARITH_OP_AST__
