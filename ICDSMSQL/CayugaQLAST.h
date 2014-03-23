/**
 * @file CayugaQLAST.h
 * @date April 4, 2007
 * @brief This file declares CayugaQLAST class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#pragma once

#include "antlr/CommonAST.hpp"

#include <string>
#include <vector>
#include <map>
using namespace std;

using namespace antlr;

#include "ASTSchema.h"

#include "../ICDSMSCE/BasicDataStructure/ByteCode.h"
//#include "AutomatonCompiler/WindowString.h"
using namespace Cayuga::BasicDataStructure;

namespace Cayuga {
namespace AutomatonCompiler {
class EdgeString;
class ForwardEdgeString;
class SchemaMapString;

class QueryString;
class WindowString;
class predWindowString;
class NodeString;
class StartNodeString;
class IntermediateNodeString;

class Predicates_DNFString;
class ConjunctiveClauseString;

class AtomPredString;
}

namespace SystemMgr {
class DataTypeMgr;
class StreamSchema;
class StreamSchemaMgr;
}

namespace QLCompiler {
enum AttributeLocation {
	SELECT_CLAUSE, 
	SIGMA_PARAM,
	SEQ_PARAM,
	MU_PARAM
};

enum AttributeDecorator {
	DOLLAR_FREE, 
	DOLLAR,
	DOLLAR_1,
	DOLLAR_2,
};

using namespace Cayuga::AutomatonCompiler;
using namespace Cayuga::SystemMgr;

class CayugaQLAST;
typedef ASTRefCount<CayugaQLAST> RefCayugaQLAST;

/**
 * @brief A CayugaQLAST object represents a CEL query AST node.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date April 4, 2007
 * @version 1.0
 */
class CayugaQLAST : public CommonAST {
public:

    CayugaQLAST( void );

	CayugaQLAST (const CayugaQLAST& rhs);

	CayugaQLAST(RefToken t);

    virtual ~CayugaQLAST( void );


	//the node must have at least one child
	RefCayugaQLAST getLastChild() const;

	virtual RefAST clone( void );

	static RefAST factory( void );

	void initVars();

	///UDF related
	void addUDF(const string& outAttrName, const string& termExpression);

	bool findUDF(const string& udfName);

	string getUDFExpr(const string& udfName);

	bool noUDF();

	//TODO: 
	//writes a complex UDF expression to termExpression, using attribute names
	//in schema to resolve names
	void write(string& termExpression, 
		const ASTSchema& lhsSchema,
		const ASTSchema& rhsSchema,
		AttributeLocation attrLoc);

	//return the data type of the expression rooted here
	//The input schema will be used to resolve data type of attribute names
	ByteCodeNS::ByteCode getDataType(const ASTSchema& lhsSchema,
		const ASTSchema& rhsSchema,
		AttributeLocation attrLoc);

	//obtain the attribute variable number and name from the attrExpr node.
	//if $ is not specified, outAttrVarNum should be set to -1.
	//if $ is specified without a number, outAttrVarNum should be 0.
	//Otherwise only $1 or $2 is allowed
	void getAttrVarNumAndName(AttributeDecorator& outAttrDecorator, 
		string& outAttrName);

	//depending on whether the attribute has $ sign, and whether it occurs
	//in sigma predicate, sequence predicate, mu predicate or UDF in 
	//or select clause or mu operator, will convert it to the appropriate
	//attribute name used in NFA node schema. Suppose the attr name is foo.
	//The convention is as follows:
	//1. Specifying $1 is always the same as not specifying $ (called $-free), 
	//and they can be used in all places. We will return the alias name of foo
	//in lhsSchema, exception in the case of mu, where we always return foo_1, 
	//and make sure it is an alias in lhsSchema.
	//2. $2 can be used in sequence or mu predicate/UDF. In both cases, 
	//we return the alias name of foo in rhsSchema.
	//3. $ can only be used in mu predicate/UDF. We always return
	//foo_n, and make sure it is an alias in lhsSchema.
	//invariant: this AST node must be of type AttrExpr
	string convertQLAttrNameTonNfaAttrName(const ASTSchema& lhsSchema,
		const ASTSchema& rhsSchema, AttributeLocation attrLoc);

//protected:
	// append your own extensions to CayugaQLAST here !

	//This variable stores schema of a StreamExpr, UnaryExpr or SelectCLause 
	//ast node. It will be reduced through attribute elimination pass.
	//const StreamSchema* _thisSchema;
	ASTSchema _thisSchema;

	//This variable stores the set of attributes required for predicate 
	//evaluation for a StreamExpr, UnaryExpr or SelectCLause 
	//ast node. It will be calculated attribute elimination pass.
	//const StreamSchema* _usedSchema;
	ASTSchema _usedSchema;
	
	//This variable stores the set of attributes required for output
	//for a StreamExpr, UnaryExpr or SelectCLause 
	//ast node. It will be calculated attribute elimination pass.
	//const StreamSchema* _demandSchema;
	ASTSchema _demandSchema;

	//This variable stores the stream name of a STREAM_NAME ast node
	string _inputStreamName;

	//This variable stores the DNF predicate of a predicate expr node.
	//it could be the filter predicate on a binary operator, or a normal 
	//predicate for a sigma operator
	Predicates_DNFString* _thisFilterDNF;

	//This variable stores the DNF rebind predicate of a mu node
	Predicates_DNFString* _thisRebindDNF;

	//This variable stores the conjunctive clause of a andExpr node
	//ConjunctiveClauseString* _thisCC;

	//This variable stores the atomic predicate of a literal node
	//AtomPredString* _thisAtomPred;

	//This variable stores the query under a QUERY node
	QueryString* _thisQuery;

	//This variable maps UDF names to their expressions
	map<string, string> _udfNameExprMap;

	//This variable stores the schema map of a forward edge associated with
	//select clause, mu or sequence operator
	SchemaMapString* _thisForwardEdgeSchemaMap;

	//This variable stores the schema map of a rebind edge associated with
	//mu operator
	SchemaMapString* _thisRebindEdgeSchemaMap;

	//This variable stores the Window under a QUERY node.
	// @added by Rainbow
	// @date March 23, 2011
	WindowString* _thisWindow;

	//luoluo 7-29
	predWindowString* _thisPredWindow;

	//true iff attribute is from event; false iff it is from node.
	//It is to be set by convertQLAttrNameTonNfaAttrName
	bool _bAttrFromEvent;
};//CayugaQLAST
} //QLCompiler
} //Cayuga

