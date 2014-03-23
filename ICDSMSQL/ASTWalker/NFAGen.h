/**
 * @file NFAGen.h
 * @date April 11, 2007
 * @brief This file declares NFAGen class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#pragma once

#include "CayugaQLASTWalker.h"

#include "../ICDSMSCE/BasicDataStructure/Automaton/NodeID.h"
using Cayuga::BasicDataStructure::Automaton::NodeID;

namespace Cayuga {

namespace AutomatonCompiler {
class EdgeString;
class RebindEdgeString;
class ForwardEdgeString;

class QueryString;
class NodeString;
class StartNodeString;
class IntermediateNodeString;

class Predicates_DNFString;
class ConjunctiveClauseString;

class AtomPredString;
}

namespace BasicDataStructure {
namespace Automaton {
class Query;
}
}


namespace QLCompiler {
using namespace Cayuga::AutomatonCompiler;
using Cayuga::BasicDataStructure::Automaton::Query;
/**
 * @brief An NFAGen object generates NFAs.

 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date April 11, 2007
 * @version 1.0
 */
class NFAGen : public CayugaQLASTWalker
{
public:
	NFAGen(vector<Query*>& newQueries);
	//virtual ~NFAGen(void);

	virtual void visit(RefCayugaQLAST& thisAST);

private:
	enum StateType {MU, SEQ, END};

	//functions to create NFAs for various AST nodes.
	//They all have the common behavior described below.
	//Given an input AST node, they create query strings for 
	//all its children and descendants, and append them into _queryStrings.
	//Then it creates a top-level automaton that reads the output streams from
	//the automata just created, and produces an output stream for 
	//the input AST node. 
	//If this ASST node publishes (as in the case for a QUERY, say), 
	//return NULL.
	//Otherwise, return this top level automaton.
	//The rational is that the returned NFA can be inline with some
	//other NFAs in an optional optimization step. However, if it publishes,
	//this optimization could be invalid in general.
	//Also, each function call will set the outStreamName to be the output
	//stream name of the NFA created. This will be used as the input stream
	//name for the edges that the function caller is constructing
	QueryString* _genNFAForQuery(RefCayugaQLAST& queryAST,
		string& outStreamName);

	QueryString* _genNFAForStreamExpr(RefCayugaQLAST& streamExprAST,
		string& outStreamName);

	QueryString* _genNFAForUnaryExpr(RefCayugaQLAST& unaryExprAST,
		string& outStreamName);

	QueryString* _genNFAForPrimaryExpr(RefCayugaQLAST& primaryExprAST,
		string& outStreamName);

	QueryString* _genNFAForSelectClause(RefCayugaQLAST& selectClauseAST,
		const string& inStreamName,
		const string& outStreamName,
		bool isPublishedStream,
		WindowString* pwindowString); //added by rainbow

	QueryString* _createOneStepNFA(
		const string& inStreamName,
		const string& outputStreamName,
		const bool isPublishedStream,
		StreamSchema* pSchema,
		Predicates_DNFString* pPredicate,
		SchemaMapString* pSchemaMap,
		WindowString* pWindowString);////add the window into the NFA ;
	                                //added by Rainbow

	QueryString* _createTwoStepNFA(
		const string& inStreamName1,
		Predicates_DNFString* pForwardPredicate1,
		SchemaMapString* pForwardSchemaMap1,
		StreamSchema* pSchema1,
		Predicates_DNFString* pFilterPredicate,
		bool bHasRebindEdge,
		Predicates_DNFString* pRebindPredicate,
		SchemaMapString* pRebindSchemaMap,
		const string& inStreamName2,
		Predicates_DNFString* pForwardPredicate2,
		SchemaMapString* pForwardSchemaMap2,
		StreamSchema* pSchema2,	
		string& outputStreamName,
		RefCayugaQLAST &streamxprAST
		);

	string _getNewInternalStreamName();

	//add a window to the query
	//@author  Rainbow
	//@data    March,25,2011
	QueryString * _addWindow(QueryString* queryString,WindowString* windowString);

	//luoluo 7-29
	QueryString * _addPredWindow(QueryString* queryString,predWindowString* pwinString);

	
	void _internalizeAndMergeAIRQueries();

	//create a new node on the heap and return it. T must be StartNodeString
	//or IntermediateNodeString.
	//if inStreamName is "", do not insert an entry into attrMap to be passed
	//into the constructor of T
	//Set outNodeID to the newly assigned ID for this node
	template <typename T>
	T* _createNodeString(const string& inStreamName,
		NodeID& outNodeID);

	void _createForwardEdgeString(
		NodeString* pSourceNode,
		NodeID destNodeID,
		Predicates_DNFString* pPredicate,
		SchemaMapString* pSchemaMap);

	//inline the input predicate into the input query.
	//If succeed, return the modified query. Otherwise return NULL.
	QueryString* _inlineNFAForFilter(QueryString* thisQuery, 
		   Predicates_DNFString* predToInline, 
		   const ASTSchema& schemaToInline,
		   const string& outStreamName,
		   const bool bIsPublished);

	//inline the input schema map into the input query, so that
	//the end state schema of this query is the same as schemaToInline
	//Always succeed and return the modified query
	QueryString* _inlineNFAForSelectClause(QueryString* thisQuery, 
		   SchemaMapString* schemaMapToInline, 
		   const ASTSchema& schemaToInline,
		   const string& outStreamName,
		   const bool bIsPublished);

	//inline the input thisQuery into the first edge of the input topQuery 
	//Always succeed and return the modified query
	QueryString* _inlineNFAForLHSBinary(QueryString* thisQuery, 
		   QueryString* topQuery);

	//inline the input rhsQueryToInline into the input thisQuery.
	//If succeed, return the modified query. Otherwise return NULL.
	QueryString* _inlineNFAForRHSBinary(QueryString* thisQuery, 
		   QueryString* rhsQueryToInline);

	//This variable stores an integer counter for node IDs
	//It can be reset for each CEL query compilation
	NodeID _nodeIDCnt;

	//This variable stores an integer counter for internal stream IDs (names)
	static int _internalStreamIDCnt;

	//This variable stores an integer counter for unique attribute name suffixes
	static int _uniqueAttrIDCnt;
	
	//This variable stores the set of newly created automaton query strings,
	//along with their output stream names. 
	//The output stream names will be used as file names to dump the compiled
	//queries. We assume each query is associated with one and only one 
	//unique output stream name. 
	//If two queries share the same output stream name, when the latter one 
	//is dumped into disk, it will overwrite the earlier one. (That can be 
	//fixed though). 
	//When we compile a CEL query into multiple NFA queries, it is guaranteed 
	//that each automaton query has exactly one output stream.
	//Queries earlier in the vector are lower level ones.
	vector<pair<QueryString*, string> > _queryStrings;

	//This variable stores the set of newly created automaton queries, 
	//compiled from the query strings
	vector<Query*>& _queries;
};//class NFAGen
} //namespace QLCompiler
} //namespace Cayuga
