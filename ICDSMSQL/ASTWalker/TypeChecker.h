/**
 * @file TypeChecker.h
 * @date April 8, 2007
 * @brief This file declares TypeChecker class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#pragma once

#include "CayugaQLASTWalker.h"

#include <set>
using namespace std;

namespace Cayuga {
namespace QLCompiler {
/**
 * @brief A TypeChecker object walks the AST and does generates schemas
			of the AST nodes.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date April 8, 2007
 * @version 1.0
 */
class TypeChecker : public CayugaQLASTWalker
{
public:
	//TypeChecker(void);
	//virtual ~TypeChecker(void);

	/**
	* @brief Visit the AST.
	* @param rootAst The root of the input AST.
	* @invariant rootAst must contain at most one streamExpr child.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 8, 2007
	* @version 1.0
	*/
	virtual void visit(RefCayugaQLAST& rootAst);

private:
	//when creating NFA each binary operator, we have two
	//states corresponding to the intermediate and end states
	//of the operator. For example, for sequence, the intermdiate state is the
	//one with filter edge, and the end state is its next state.
	//For mu, the intermediate state is the one with both filter and rebind
	//edges, and the end state is its next state.
	enum NFAStateType {
		INTERMEDIATE, 
		END, 
	};

	//the input node must be QUERY node
	ASTSchema _processQuery(RefCayugaQLAST& queryAST);

	//from clause related functions
	ASTSchema _processStreamExpr(RefCayugaQLAST& streamExprAST);
	ASTSchema _processPrimaryExpr(
		RefCayugaQLAST& primaryExprAST);

	ASTSchema _processUnaryExpr(RefCayugaQLAST& unaryExprAST);

	///schema related functions
	
	/**
		Append the source schema to the destination schema in a way 
				according to the value of schematype and stateType.			

			The logic for creating destAlias (the "real" attribute name in
			NFA state schema) is as follows.

			If schematype is FOR_SEQ, just copy each attribute from source
			to destination verbatim.

			If schematype is FOR_SEQ_RENAME, copy and rename each attribute
			att from src to att_1

			If schematype is FOR_MU_FIRST and stateType is INTERMEDIATE,
			nextEventSchemaForMu will be read (it must take non-NULL value
			in this case).
				case 1: for those attributes att in source that also appear in 
				nextEventSchemaForMu, rename them to att_1
				case 2: for those attributes att in source that do not appear 
				in nextEventSchemaForMu, rename them to att_n

			If schematype is FOR_MU_LAST,the destAlias is set to att_n
			
			The logic for creating an attribute entry (including name, 
			data type, alias) in the destSchema is as follows.

			If stateType is INTERMEDIATE, name is srcAlias, and alias is 
			destAlias. If state is END, both name and alias are set to 
			destAlias.
	*/
	void _appendSchema(ASTSchema& destSchema,
		const ASTSchema& srcSchema,
		SchemaType schematype,
		NFAStateType stateType,
		const ASTSchema* nextEventSchemaForMu = NULL);

	//combine schema 1 and 2 to create the node schema of the 
	//output stream of SEQ operator.
	ASTSchema _combineSchemaForSEQEndState(const ASTSchema& schema1,
		const ASTSchema& schema2);

	//combine schema 1 and 2 to create the node schema of the 
	//intermediate NFA state for MU operator.
	//for those attributes foo in both schema 1 and 2, they will be duplicated
	//into foo_1 and foo_n. For those attributes only in 1, they will copied to
	//renamed to foo_n
	ASTSchema _combineSchemaForMUIntermediateState(
	const ASTSchema& schema1,
	const ASTSchema& schema2);

	//use the intermediate NFA state for MU operator to create the schema
	//for the end state of MU.
	ASTSchema _combineSchemaForMUEndState(
	const ASTSchema& muIntermediateNodeSchema);

	///result clause related functions
	//produce the schema for result clause from it sassociated outputSchema
	void _processResultClause(
		RefCayugaQLAST& resultAST,
		ASTSchema& outputSchema);

	///select clause related functions
	//produce the schema for select clause from it sassociated streamExprSchema
	ASTSchema _processSelectClause(
		RefCayugaQLAST& selectAST,
		ASTSchema& streamExprSchema);
	
	//To be invoked for generating schema for a select clause as follows.
	//Read termExprAST. If it is a regular attribute, 
	//set the first three parameters as follows.
	//Get the attrName used in its FROM clause and store
	//it in inAttrName. Also get the alias attribute name, the name to be used
	//outside of this SLECT clause scope, and store it in
	//outAttrName. If AS construct is used in select clause, alias is set by
	//AS. Otherwise outAttrName will be equal to inAttrName. Not that we do not
	//need to resolve any name conflict here, so if there is any, it will have
	//been resolved in generating schema for streamExpr. 
	//
	//If the termExpr is instead a complex term, we will create a UDF element
	//for AIR and store in under the input selectAST. in this case,
	//the inAttrName and outAttrName will both set to the same value, which is
	//either specified by AS, or freshly generated if AS clause is missing.
	void  _getAttrNameTypeOrUDFFromTermExprInSelectClause(
		string& inAttrName,
		string& outAttrName,
		string& outAttrType,
		const RefCayugaQLAST& termExprAST,
		const ASTSchema& streamExprSchema,
		const RefCayugaQLAST& selectAST
		);

	void _getUDFFromTermExprInMuParameter(
		const RefCayugaQLAST& termExprAST,
		const ASTSchema& nodeSchema,
		const ASTSchema& eventSchema,
		const RefCayugaQLAST& binOpAST);

	//get the first term parameter from MU. If it does not exist. Return
	//NULL
	///@invariant: input binaryOpAST must be MU
	RefCayugaQLAST _getFirstTermParameterFromMU(RefCayugaQLAST& binaryOpAST);

	//This variable stores an integer count used to generate unique IDs for
	//attributes
	static int _uniqueIDcnt;
};//class TypeChecker
} //namespace QLCompiler
} //namespace Cayuga
