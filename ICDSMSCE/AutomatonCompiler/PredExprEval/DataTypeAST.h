/**
 * @file DataTypeAST.h
 * @date November 30, 2005
 * @brief This file declares class DataTypeAST.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef __DATA_TYPE_AST__
#define __DATA_TYPE_AST__

#include "BasicAST.h"
using namespace antlr;

#include "BasicDataStructure/UnionDataType.h"
using namespace Cayuga::BasicDataStructure;

#include <string>
using namespace std;

namespace Cayuga {
namespace AutomatonCompiler {
namespace PredExprEval {
class DataTypeAST;
typedef ASTRefCount<DataTypeAST> RefDataTypeAST;
/**
 * @brief A DataTypeAST object represents a dataType type of 
		  AST node for a complex predicate in AIR.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 30, 2005
 * @version 1.0
 */
class DataTypeAST : public BasicAST {
 public:
	 void initialize( RefToken t );

	virtual RefAST clone( void );

	static RefAST factory( void );

	virtual void compile(Context& context,
		vector< pair<char*, size_t> >& programOut);

	virtual ByteCodeNS::ByteCode getType(Context& context);

	virtual void write(ostream& out = cout);

 private:
	UnionDataType _value;
};//class DataTypeAST
} //namespace PredExprEval
} //namespace AutomatonCompiler 
} //namespace Cayuga

#endif //__DATA_TYPE_AST__
