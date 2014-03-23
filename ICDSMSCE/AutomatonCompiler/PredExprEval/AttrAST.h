/**
 * @file AttrAST.h
 * @date November 30, 2005
 * @brief This file declares class AttrAST.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */


#ifndef __ATTR_AST__
#define __ATTR_AST__


#include "BasicAST.h"
using namespace Cayuga::AutomatonCompiler::PredExprEval;

#include <string>
using namespace std;

#include "BasicDataStructure/ByteCode.h"

#include "BasicDataStructure/Predicate/AttributeID.h"
using Cayuga::BasicDataStructure::Predicate::AttributeID;

using namespace antlr;

namespace Cayuga {
namespace AutomatonCompiler {
namespace PredExprEval {
class AttrAST;
typedef ASTRefCount<AttrAST> RefAttrAST;
/**
 * @brief An AttrAST object represents an attribute type of 
		  AST node for a complex predicate in AIR.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 30, 2005
 * @version 1.0
 */
class AttrAST : public BasicAST {
 public:
	 void initialize( RefToken t );
	
	virtual RefAST clone( void );

	static RefAST factory( void );

	virtual void getAttrTypeFromSchema(Context& context);

	virtual void compile(Context& context,
		vector<pair<char*, size_t> >& programOut);

	void compileFunction(Context& context,
		vector<pair<char*, size_t> >& programOut);

	void compileAttribute(Context& context,
		vector<pair<char*, size_t> >& programOut);

	virtual ByteCodeNS::ByteCode getType(Context& context);

	virtual void renameAttrRef(const SchemaMapString* schemaMap);

	virtual void getReferredAttributes(set<string>& referredAttrs);

	virtual void write(ostream& out = cout);

 private:
	 bool _isFuncNode();

	 void _getAttrSource(Context& context);

	 string _getAttrName();

	 void _setAttrName(const string& attrName);

	//string _attrName;
	ByteCodeNS::ByteCode _AttrSource;

	AttributeID _attrID;
	size_t _offset;
	string _attrTypeName;
};//class AttrAST
} //namespace PredExprEval
} //namespace AutomatonCompiler 
} //namespace Cayuga

#endif //__ATTR_AST__
