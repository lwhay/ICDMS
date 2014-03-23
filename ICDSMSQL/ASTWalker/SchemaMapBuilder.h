/**
 * @file SchemaMapBuilder.h
 * @date April 8, 2007
 * @brief This file declares SchemaMapBuilder class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#pragma once

#include "CayugaQLASTWalker.h"

namespace Cayuga {
namespace QLCompiler {
/**
 * @brief A SchemaMapBuilder object build schema maps for foward/rebind edges, 
		 and put them under binOp (for rebind edge ) or select_clause 
		 (for forward) AST nodes.

		 SchemaMaps are stored in the following AST nodes: selectClause,
		 sexp, binOp. 
		 
		 The schemaMap of uexp can be found by visiting its primaryExpr child.
		 The schemaMap of primaryExpr is either trivially obtained from SNAME
		 if primaryExpr is of type SNAME, or can be obtained recursively from
		 QUERY, if it is of type QUERY.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date April 8, 2007
 * @version 1.0
 */
class SchemaMapBuilder : public CayugaQLASTWalker
{
public:
	//SchemaMapBuilder(void);
	//~SchemaMapBuilder(void);

	/**
	* @brief Visit the AST.
	* @param thisAST The root of the input AST.
	* @invariant rootAst must be a QUERY node.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 8, 2007
	* @version 1.0
	*/
	virtual void visit(RefCayugaQLAST& thisAST);

private:
	//(except for a start state) The state into which a forward edge goes 
	//has one of the following two types.
	enum StateType {MU, SEQ};

	///query and streamExpr related functions
	void _processQuery(RefCayugaQLAST& queryAST);

	//process the input streamExpr. If it is unary, of form sigma(E), first
	//construct the schema map for E, and then construct the schema map
	//for the forward edge corresponding to sigma in the top-level one-step 
	//automaton.
	//If it is binary, of form E1 binOp E2, first construct the schema maps
	//for E1 and E2. Then construct the schema maps of the two forward edegs
	//in the top-level two-step automaton. 
	//Also if the binOp is mu, will construct schema map for the rebind edge.
	void _constructSchemaMapForStreamExpr(RefCayugaQLAST& streamExprAST);

	void _processUnaryExpr(RefCayugaQLAST& unaryExprAST);

	void _constructIdentitySchemaMapForUnaryExpr(
		const ASTSchema& eventAndDestSchema, 
		SchemaMapString& outSchemaMap);

	void _processPrimaryExpr(RefCayugaQLAST& primaryExprAST);

	///select clause related functions
	//
	//construct the schema map of the forward edge leading to end state,
	//and store it in select_clause node
	void _constructSchemaMapForSelectClause(
		RefCayugaQLAST& selectClauseAST,
		const RefCayugaQLAST& streamExprAST);

	///schema related functions

	//Create schema map for a forwarde edge one a one-step automaton.
	//The source state of this forward edge is a start state, so we know its
	//schema is empty. So no entry in the schema map will come from source 
	//state schema. Each entry in the schema map will come either from the 
	//input event stream schema of this edge, or the pontential
	//UDF specification from selectOrMuOpAST
	//eventSchema is the input stream schema of this forward edge.
	//destStateSchema is the schema of the destination state of 
	//this forward edge.
	//selectOrMuOpAST is an AST node that could contain UDF specs.
	//The constructed schema will be stored in outSchemaMap
	void _constructSchemaMapForForwardEdgeInOneStepAutomaton(
		const ASTSchema& eventSchema, 
		const ASTSchema& destStateSchema, 
		const RefCayugaQLAST& selectAST,
		SchemaMapString& outSchemaMap);

	void _constructSchemaMapForForwardEdgeInToSEQ(
		const ASTSchema& eventSchema, 
		const ASTSchema& destStateSchema, 
		const ASTSchema& nextEventSchema, 
		SchemaMapString& outSchemaMap);

	//Create schema map for a forwarde edge leading to a non-mu node: SEQ
	//or END.
	//srcNodeSchema is the schema of the source state of this forward edge.
	//eventSchema is the input stream schema of this forward edge.
	//destSchema is the schema of the destination state of this forward edge.
	void _constructSchemaMapForForwardEdgeOutOfSEQIntoEND(
		const ASTSchema& srcNodeSchema, 
		const ASTSchema& eventSchema, 
		const ASTSchema& destSchema, 
		SchemaMapString& outSchemaMap);

	//Create schema map for a forward edge leading to a mu node. 
	//srcNodeSchema is the schema of the source state of this forward edge.
	//eventSchema is the input stream schema of this forward edge.
	//nextEventSchema is the input stream schema of dest state of 
	//this forward edge.
	//destSchema is the schema of the destination state of this forward edge.
	void _constructSchemaMapForForwardEdgeIntoMU(
		const ASTSchema& eventSchema,
		const ASTSchema& destSchema,
		SchemaMapString& outSchemaMap);

	void _constructSchemaMapForForwardEdgeOutOfMUIntoEND(
		const ASTSchema& prevEventSchema, 
		const ASTSchema& srcNodeSchema, 
		const ASTSchema& eventSchema, 
		const ASTSchema& destSchema, 
		const RefCayugaQLAST& muOpAST,
		SchemaMapString& outSchemaMap);

	///for rebind edge
	//construct the schema map for a rebind edge as follows.
	//pNextEventSchema is the stream schema of the RHS 
	//expression of the mu expression. 
	//pDestSchema is for the schema of the mu state with rebind edge. 
	//Store the constructed schemaMap in outSchemaMap
	void _constructSchemaMapForRebindEdge(
		const ASTSchema& pNextEventSchema,
		const ASTSchema& pDestSchema, 
		const RefCayugaQLAST& muOpAST,
		SchemaMapString& outSchemaMap);

};//class SchemaMapBuilder
} //namespace QLCompiler
} //namespace Cayuga
