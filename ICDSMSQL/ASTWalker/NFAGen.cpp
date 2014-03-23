/**
 * @file NFAGen.cpp
 * @date April 11, 2007
 * @brief This file defines NFAGen class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include <fstream>
#include <set>
using namespace std;

#include "P2TokenTypes.hpp"
#include "NFAGen.h"

using namespace Cayuga::QLCompiler;

#include "../ICDSMSCE/AutomatonCompiler/ForwardEdgeString.h"
#include "../ICDSMSCE/AutomatonCompiler/IntermediateNodeString.h"
#include "../ICDSMSCE/AutomatonCompiler/Predicates_DNFString.h"
#include "../ICDSMSCE/AutomatonCompiler/QueryString.h"
using namespace Cayuga::AutomatonCompiler;

#include "../ICDSMSCE/BasicDataStructure/Automaton/Query.h"

#include "../ICDSMSCE/SAXHandler/ConfigMgr.h"
using namespace Cayuga::SAXHandler;

#include "../ICDSMSCE/SystemMgr/DataTypeMgr.h"
#include "../ICDSMSCE/SystemMgr/Options.h"
#include "../ICDSMSCE/SystemMgr/StreamSchemaMgr.h"
using namespace Cayuga::SystemMgr;


#include "../ICDSMSCE/Utils/AttrMap.h"
#include "../ICDSMSCE/Utils/RAssert.h"
using namespace Cayuga::Utils;

#include "../ICDSMSCE/SystemMsg.h"
using namespace Cayuga::SystemMsg;

#include "predWindowGen.h"

int NFAGen::_internalStreamIDCnt;
int NFAGen::_uniqueAttrIDCnt;

NFAGen::NFAGen(vector<Query*>& newQueries)  : _nodeIDCnt(0), 
_queries(newQueries) {
}

//NFAGen::~NFAGen(void)
//{
//}

void NFAGen::visit(RefCayugaQLAST& thisAST) {
	Assert (thisAST->getType() == P2TokenTypes::QUERY);

	string outStreamName;
	_genNFAForQuery(thisAST, outStreamName);

	_internalizeAndMergeAIRQueries();
}

QueryString* NFAGen::_genNFAForQuery(RefCayugaQLAST& queryAST,
									 string& outStreamName) {
	//get select clause
	RefCayugaQLAST selectClauseAST = getSelectClauseChildFromQuery(queryAST);

	//process streamExpr clause
	RefCayugaQLAST streamExprAST = getStreamExprChildFromQuery(queryAST);

	RefCayugaQLAST outputClauseAST 
		= (RefCayugaQLAST)streamExprAST->getNextSibling();

	string inputStreamName;
	QueryString* thisQuery
		= _genNFAForStreamExpr(streamExprAST,
		inputStreamName); //set inputStreamName in this function call
	
	//process optional output/publish/result clause
	//if it exists, set the outStreamName to that published name
	//other create an internal stream name for outStreamName
	bool isPublishedStream;
	if (outputClauseAST) {
		assert (outputClauseAST->getType() == P2TokenTypes::RESULT_CLAUSE);

		//The AIR query compiler will push the output stream schema into the 
		//system table. So we do not need to worry here.
		outStreamName = outputClauseAST->getText();
		isPublishedStream = true;
		//cout<<"compiling query "<<_outputStreamName<<endl;
	}
	else {
		//create a random internal stream name, not published
		outStreamName = _getNewInternalStreamName();
		isPublishedStream = false;
	}

	//generate final NFA node for select clause based on the query generated
	//so far
	if (thisQuery && ConfigMgr::getInstance()->getOptions()->_bInlineQuery) {
		//can inline the top level one step NFA corresponding to select clause
		//with thisQuery
		thisQuery = _inlineNFAForSelectClause(
			thisQuery, 
			selectClauseAST->_thisForwardEdgeSchemaMap, //schema map
			selectClauseAST->_thisSchema,
			outStreamName,
			isPublishedStream
			);
		Assert (thisQuery != NULL);

		thisQuery=_addWindow(thisQuery,queryAST->_thisWindow);

		//luoluo 7-30
		//thisQuery = _addPredWindow(thisQuery,queryAST->_thisPredWindow);
// 		thisQuery->_thisPredWindow = queryAST->_thisPredWindow;


		//if inline is successful
		if (thisQuery)
			return thisQuery;
	}

	//cannot inline
	thisQuery = _genNFAForSelectClause(selectClauseAST, 
		inputStreamName, //input stream name the NFA generated in the 
		//function call
		outStreamName,	 //output stream name the NFA generated in the 
		//function call
		isPublishedStream,
		queryAST->_thisWindow
		);
	return thisQuery;
}

QueryString* NFAGen::_inlineNFAForFilter(QueryString* thisQuery, 
		   Predicates_DNFString* predToInline, 
		   const ASTSchema& schemaToInline,
		   const string& outStreamName,
		   const bool bIsPublished) {
	//get last forward edge of thisQuery
	ForwardEdgeString* pEdge = thisQuery->getLastForwardEdge();

	///sanity checks
	Assert (predToInline != NULL);
	if (pEdge->getDNF() == NULL) {
		pEdge->setDNF(new Predicates_DNFString());
	}

	bool thisPredIsEmpty = pEdge->getDNF()->isEmpty();
	bool predToInlineIsEmpty = predToInline->isEmpty();

	//if neither predicate is empty,
	//and at least one of them is not conjunctive, cannot merge
	if (!thisPredIsEmpty && !predToInlineIsEmpty) {
		if (!predToInline->isConjunctive() || !pEdge->getDNF()->isConjunctive())
			return NULL;
	}

	//if predicate has a complex pred, and the schema map on pEdge is not
	//identity, then cannot proceed
	//if (predToInline->hasComplexPredicate() 
	//	&& !pEdge->_schemaMap.IsIdentityMap())
	//	return NULL;

	//if predToInline refers to attributes created by the schema map on pEdge, 
	//cannot inline
	//Note that here we have to have a conservative estimate: 
	//since getReferredAttributes only returns attributes mentioned by 
	//non-complex predicates, if predToInline contains any complex pred, we 
	//cannot merge in general
	set<string> referredAttrs
		= predToInline->getReferredAttributes();
	if (pEdge->_schemaMap.attributeCreatedByUDF(referredAttrs))
		return NULL;

	//start doing real work
	//1. rename each attribute reference x in predToInline to f^{-1}(x), where
	//f is pEdge->_schemaMap
	//2. merge predToInline with pEdge->_pDNF
	//5. clear the output stream name
	//6. add new output stream name
	//7. modify _queryStrings

	predToInline->renameAttrRefInPredsToAttrIn(&pEdge->_schemaMap);

	pEdge->getDNF()->mergePredicate(predToInline);

	IntermediateNodeString* pEndNode = thisQuery->getEndNode();
	
	pEndNode->clearOutputStreams();

	pEndNode->addOutputStream(outStreamName, bIsPublished);

	//modify the output stream name of this query
	//invariant: thisQuery must be at the bottom of _queryStrings
	Assert (_queryStrings.back().first == thisQuery);
	_queryStrings.back().second = outStreamName;

	return thisQuery;
}

QueryString* NFAGen::_inlineNFAForSelectClause(QueryString* thisQuery, 
		   SchemaMapString* schemaMapToInline, 
		   const ASTSchema& schemaToInline,
		   const string& outStreamName,
		   const bool bIsPublished) {
	//get last forward edge of thisQuery
	ForwardEdgeString* pEdge = thisQuery->getLastForwardEdge();

	//no sanity check required. this optimization can always go through.

	//start doing real work
	//1. compose schemaMapToInline into pEdge->_schemaMap
	//2. change the end state schema of thisQuery into schemaToInline
	//3. clear the output stream name
	//4. add new output stream name
	//5. modify _queryStrings

	pEdge->_schemaMap.composeSchemaMapToSchemaOfInputMap(schemaMapToInline);

	IntermediateNodeString* pEndNode = thisQuery->getEndNode();
	pEndNode->setNodeSchema(schemaToInline.toStreamSchema());
	
	pEndNode->clearOutputStreams();

	pEndNode->addOutputStream(outStreamName, bIsPublished);

	//modify the output stream name of this query
	//invariant: thisQuery must be at the bottom of _queryStrings
	Assert (_queryStrings.back().first == thisQuery);
	_queryStrings.back().second = outStreamName;

	return thisQuery;
}

QueryString* NFAGen::_inlineNFAForLHSBinary(QueryString* thisQuery, 
		   QueryString* topQuery) {
	//get last forward edge of thisQuery
	ForwardEdgeString* pEdge = thisQuery->getLastForwardEdge();

	//no sanity check required. this optimization can always go through.

	//start doing real work
	//1. Let the schema map of the first forward edge in topQ be
	//	 schemaMapToInline. Compose schemaMapToInline into pEdge->_schemaMap
	//2. Let the end state of thisQuery be Q. Let the middle state schema 
	//	 of topQ be schemaToInline. Change the schema of Q to schemaToInline. 
	//3. Remove the output stream of Q. 
	//	 set the input stream name of Q to intputStreamName.
	//4. Let the second forward edge of topQ be FE. 
	//   Move FE into Q.
	//5. Move the filter edge and optional rebind edge of the middle state
	//	 of topQ into Q.
	//6. create a new end state ES of thisQuery, and link FE to it. 
	//	 Let the output stream name of topQ be OS. Set the output
	//	 stream name of ES to OS, and do not publish.
	//	 set the schema of ES to that of end state in topQ.
	//7. modify _queryStrings, by removing topQ from its back, 
	//	 moving thisQ to the back, and deleting topQ.

	ForwardEdgeString* pFirstEdgeInTopQ = topQuery->getFirstForwardEdge();
	SchemaMapString* schemaMapToInline = &pFirstEdgeInTopQ->_schemaMap;
	pEdge->_schemaMap.composeSchemaMapToSchemaOfInputMap(schemaMapToInline);

	IntermediateNodeString* pMiddleStateInTopQ = topQuery->getSecondNode();
	IntermediateNodeString* pEndNode = thisQuery->getEndNode();
	pEndNode->setNodeSchema(new StreamSchema(
		*pMiddleStateInTopQ->getNodeSchema()));
	//pMiddleStateInTopQ->setNodeSchema(NULL);
	
	pEndNode->clearOutputStreams();
	pEndNode->setInputStreamName(pMiddleStateInTopQ->getInputStreamName());

	//move forward edge of middle state in topQ
	ForwardEdgeString* pSecondEdgeInTopQ = topQuery->getLastForwardEdge();
	vector<ForwardEdgeString*>::iterator edge_find_it;
	for (edge_find_it = pMiddleStateInTopQ->_pForwards.begin(); 
		edge_find_it != pMiddleStateInTopQ->_pForwards.end(); ++edge_find_it) {
			if (*edge_find_it == pSecondEdgeInTopQ) break;;
	}
	Assert (edge_find_it != pMiddleStateInTopQ->_pForwards.end());
	pMiddleStateInTopQ->_pForwards.erase(edge_find_it);
	pEndNode->addEdge(pSecondEdgeInTopQ);

	//move filter edge of middle state in topQ
	FilterEdgeString* pFilterEdgeInMiddleStateOfTopQ 
		= pMiddleStateInTopQ->_pFilter;
	pEndNode->addEdge(pFilterEdgeInMiddleStateOfTopQ);
	pMiddleStateInTopQ->_pFilter = NULL;

	//move optional rebind edge of middle state in topQ
	RebindEdgeString* pRebindEdgeInMiddleStateOfTopQ 
		= pMiddleStateInTopQ->_pRebind;
	if (pRebindEdgeInMiddleStateOfTopQ) {
		pEndNode->addEdge(pRebindEdgeInMiddleStateOfTopQ);
		pMiddleStateInTopQ->_pRebind = NULL;
	}

	//luoluo 8-2
	if (pMiddleStateInTopQ->returnPredWindowString())
			 pEndNode->setRationalPredWindow(pMiddleStateInTopQ->returnPredWindowString());
	else	 pEndNode->setRationalPredWindow(NULL);
	


	NodeID newEndStateID;
	IntermediateNodeString* newEndState = 
		_createNodeString<IntermediateNodeString>("", newEndStateID);
	thisQuery->addNode(newEndState);
	pSecondEdgeInTopQ->setDestNodeID(newEndStateID);
	IntermediateNodeString* pEndNodeInTopQ = topQuery->getEndNode();
	pair<string, bool> outStreamEntry = pEndNodeInTopQ->getOutputStream();
	newEndState->addOutputStream(outStreamEntry.first, outStreamEntry.second);
	newEndState->setNodeSchema(new StreamSchema(
		*pEndNodeInTopQ->getNodeSchema()));
	

	//modify the output stream name of this query
	//invariant: topQuery must be at the bottom of _queryStrings
	Assert (_queryStrings.back().first == topQuery);
	
	//find the entry for thisQuery in _queryStrings
	//_queryStrings.
	vector<pair<QueryString*, string> >::iterator find_it;
	for (find_it = _queryStrings.begin(); find_it != _queryStrings.end(); 
		++find_it) {
		if (find_it->first == thisQuery) break;
	}
	Assert (find_it != _queryStrings.end());

	//copy it to the end, but overwriting the entry for topQ, but kept the out
	//put stream name
	_queryStrings.back().first = find_it->first;
	
	//erase the original entry for thisQuery in _queryStrings
	_queryStrings.erase(find_it);
	delete topQuery;

	//thisQuery->write();
	return thisQuery;
}

QueryString* NFAGen::_inlineNFAForRHSBinary(QueryString* thisQuery, 
		   QueryString* rhsQueryToInline) {
	//get last forward edge of thisQuery
	ForwardEdgeString* pEdge = thisQuery->getLastForwardEdge();

	ForwardEdgeString* pEdgeInRhs = rhsQueryToInline->getLastForwardEdge();

	Predicates_DNFString* predToInline = pEdgeInRhs->getDNF(); 
	Predicates_DNFString nullPred;
	if (predToInline == NULL)
		predToInline = &nullPred;

	///sanity checks
	Assert (predToInline != NULL);
	if (pEdge->getDNF() == NULL) {
		pEdge->setDNF(new Predicates_DNFString());
	}

	SchemaMapString* schemaMapToInline = &pEdgeInRhs->_schemaMap; 
	IntermediateNodeString* pSecondToEndNode = thisQuery->getSecondToEndNode();

	Predicates_DNFString* thisPred = pEdge->getDNF();
	assert (thisPred == NULL || thisPred->isEmpty());
	//if (thisPred->hasComplexPredicate() && !schemaMapToInline->IsIdentityMap()) 
	//	return NULL;
	//
	////if pEdge->getDNF() refers to attributes created by the schema map 
	////schemaMapToInline, cannot inline. Also check this for filter edge.
	////Note that here we have to have a conservative estimate.
	//set<string>& referredAttrs
	//	= thisPred->getReferredAttributes();
	//if (schemaMapToInline->attributeCreatedByUDF(referredAttrs))
	//	return NULL;

	//copy and pasted the fragment below...
	if (pSecondToEndNode->_pFilter && pSecondToEndNode->_pFilter->getDNF()) {
		thisPred = pSecondToEndNode->_pFilter->getDNF();
		//if (thisPred->hasComplexPredicate() 
		//	&& !schemaMapToInline->IsIdentityMap())
		//	return NULL;
		set<string> referredAttrs
			= thisPred->getReferredAttributes();
		if (schemaMapToInline->attributeCreatedByUDF(referredAttrs))
			return NULL;

		//if neither predicate is empty,
		//and at least one of them is not conjunctive, cannot merge
		if (!predToInline->isEmpty()) {
			if (!predToInline->isConjunctive() || !thisPred->isConjunctive())
				return NULL;
		}
	}

	//copy and pasted the fragment below...
	if (pSecondToEndNode->_pRebind && pSecondToEndNode->_pRebind->getDNF()) {
		thisPred = pSecondToEndNode->_pRebind->getDNF();
		//if (thisPred->hasComplexPredicate() 
		//	&& !schemaMapToInline->IsIdentityMap())
		//	return NULL;
		set<string> referredAttrs
			= thisPred->getReferredAttributes();
		if (schemaMapToInline->attributeCreatedByUDF(referredAttrs))
			return NULL;
	}

	//start doing real work
	//1. rename each attribute reference x in the 
	//   filter predicate of the second to last state in thisQ:
	//	 set it to f^{-1}(x), where f is schemaMapToInline.
	//	 Do so for the optional rebind predicate.
	//2. merge predToInline with the filter predicate of the second to 
	//   last state in thisQ.
	//3. compose pEdge->_schemaMap into schemaMapToInline
	//4. Change the input stream of the second to last node in thisQ to that
	//	 of the start node in rhsQ
	//5. modify _queryStrings: remove rhsQ from _queryStrings. It must be 
	//	 second to last one

	//pEdge->getDNF()->
	//renameAttrRefInPredsToAttrIn(schemaMapToInline);

	if (pSecondToEndNode->_pFilter && pSecondToEndNode->_pFilter->getDNF()) {
		pSecondToEndNode->_pFilter->getDNF()->
			renameAttrRefInPredsToAttrIn(schemaMapToInline);
	}

	//merge predToInline into filter pred of thisQ
	if (!predToInline->isEmpty()) {
		if (pSecondToEndNode->_pFilter) {
			if (pSecondToEndNode->_pFilter->getDNF())
				pSecondToEndNode->_pFilter->getDNF()->
				mergePredicate(predToInline);
			else {
				pSecondToEndNode->_pFilter->setDNF(predToInline);
			}
		}
		else {
			//create a filter edge
			pSecondToEndNode->_pFilter = new FilterEdgeString();
			pSecondToEndNode->_pFilter->setDNF(
				new Predicates_DNFString(*predToInline));
		}
	}

	if (pSecondToEndNode->_pRebind && pSecondToEndNode->_pRebind->getDNF()) {
		pSecondToEndNode->_pRebind->getDNF()->
			renameAttrRefInPredsToAttrIn(schemaMapToInline);
	}

	pEdge->_schemaMap.composeSchemaMapToSchemaOfCurrentMap(schemaMapToInline);
	//schemaMapToInline->composeSchemaMapToSchemaOfInputMap(&pEdge->_schemaMap);
	//pEdge->_schemaMap = *schemaMapToInline;

	pSecondToEndNode->setInputStreamName(
		rhsQueryToInline->getStartNode()->getInputStreamName());

	//modify the output stream name of this query
	//invariant: thisQuery must be at the bottom of _queryStrings
	Assert (_queryStrings.size() >= 2);
	Assert (_queryStrings[_queryStrings.size()-2].first == rhsQueryToInline);
	vector<pair<QueryString*, string> >::iterator erase_it 
		= _queryStrings.end()-2;
	_queryStrings.erase(erase_it);
	delete rhsQueryToInline;

	//thisQuery->write();
	return thisQuery;
}

void NFAGen::_internalizeAndMergeAIRQueries() {
	for (vector<pair<QueryString*, string> >::iterator it 
		= _queryStrings.begin(); it != _queryStrings.end(); ++it) {
		QueryString* thisQuery = it->first;
		string outputStreamName = it->second;
		Query* thisQ;
		try {
			if (ConfigMgr::getInstance()->getOptions()->_bVerbose) {
				thisQuery->write(cout);
			}
			
			const string& _compiledAIRLogDir 
				= ConfigMgr::getInstance()->getOptions()->_compiledAIRLogDir;
			if (!_compiledAIRLogDir.empty()) {
				//log the AIR query
				string outFileName = outputStreamName + ".xml";
				//store files in directory compiledAIR
				outFileName = _compiledAIRLogDir + outFileName ;
				ofstream out(outFileName.c_str());
				if (!out) {
					cerr<<"error opening "<<outFileName<<endl;
					exit(-1);
				}
				thisQuery->write(out);
				out.close();
			}

			thisQ = thisQuery->compile();
			//delete thisQString;
		}
		catch (string &s){
			cout << "Query compilation failed: " <<s<<endl;
			//delete thisQString;  // not sure these two lines are necessary...
			throw (s);
		}

		_queries.push_back(thisQ);
	}
}

QueryString* NFAGen::_genNFAForStreamExpr(RefCayugaQLAST& streamExprAST,
										  string& outStreamName) {
	//The compilation logic for a streamExpr is as follows:
	//We first construct NFAs for the LHS streamExpr, 
	//and store it in thisQuery. If this is unary operator, we return 
	//thisQuery immeidately.
	//
	//If this is a binary operator, we will construct NFAs for the RHS 
	//unaryExpr. Then we will create a top-level 2-step automaton for this 
	//binary stream expression. Then we perform optional inlining optimization.
	//We finally return the top-level automaton.
						
	string lhsInputStreamName;
	RefCayugaQLAST unaryOrStreamExprAST 
		= (RefCayugaQLAST)streamExprAST->getFirstChild();
	QueryString* thisQuery;
	if (unaryOrStreamExprAST->getType() == P2TokenTypes::UNARY_EXPR) {
		thisQuery = _genNFAForUnaryExpr(unaryOrStreamExprAST, 
			lhsInputStreamName);
	}
	else {
		thisQuery = _genNFAForStreamExpr(unaryOrStreamExprAST, 
			lhsInputStreamName);
	}

	RefCayugaQLAST binOpAST 
		= (RefCayugaQLAST)unaryOrStreamExprAST->getNextSibling();
	if (!binOpAST)  {
		//the input sexp is unary
		outStreamName = lhsInputStreamName;
		return thisQuery;
	}

	QueryString* lhsQuery = thisQuery;

	//generate NFA for the rhs argument
	//of the binary op. 
	RefCayugaQLAST rhsUnaryExprAST 
		= (RefCayugaQLAST)binOpAST->getNextSibling();
	Assert (rhsUnaryExprAST 
		&& rhsUnaryExprAST->getType() == P2TokenTypes::UNARY_EXPR);
	
	string rhsInputStreamName;
	QueryString* rhsQuery = _genNFAForUnaryExpr(rhsUnaryExprAST, 
		rhsInputStreamName);

	//TODO: new generate a binary query
	//push the new queries into _queryStrings
	bool bHasRebindEdge = (binOpAST->getType() == P2TokenTypes::MU_OP);
	thisQuery = _createTwoStepNFA(
		lhsInputStreamName,
		NULL,//forward edge 1 predicate
		binOpAST->_thisForwardEdgeSchemaMap,
		binOpAST->_thisSchema.toStreamSchema(),
		binOpAST->_thisFilterDNF,
		bHasRebindEdge,
		binOpAST->_thisRebindDNF,
		binOpAST->_thisRebindEdgeSchemaMap,
		rhsInputStreamName,
		NULL, //forward edge 2 predicate
		streamExprAST->_thisForwardEdgeSchemaMap,
		streamExprAST->_thisSchema.toStreamSchema(),	
		outStreamName,
		binOpAST
		);
// 	//luoluo 8-1
// 	IntermediateNodeString * pIntermediateNode = thisQuery->getSecondNode();
// 	predWindowGen * expr = new predWindowGen();
// 	expr->visit(binOpAST);
// 	Assert(binOpAST->_thisPredWindow);
// 	pIntermediateNode->setRationalPredWindow(binOpAST->_thisPredWindow);
	

	//may involve optimization
	Assert (thisQuery != NULL);
	if (lhsQuery && ConfigMgr::getInstance()->getOptions()->_bInlineQuery) {
		thisQuery = _inlineNFAForLHSBinary(lhsQuery, 
		   thisQuery);
		Assert (thisQuery != NULL);
	}

	if (rhsQuery && ConfigMgr::getInstance()->getOptions()->_bInlineQuery) {
		QueryString* inlinedQuery = _inlineNFAForRHSBinary(thisQuery, 
		   rhsQuery);
		 
		if (inlinedQuery != NULL) {
			thisQuery = inlinedQuery;
		}
	}

	return thisQuery;
}

QueryString* NFAGen::_genNFAForUnaryExpr(RefCayugaQLAST& unaryExprAST,
									  string& outStreamName) {
	RefCayugaQLAST childAST 
		= (RefCayugaQLAST)unaryExprAST->getFirstChild();
	RefCayugaQLAST primaryExprAST;
	if (childAST->getType() ==  P2TokenTypes::UNARY_OPS)
		primaryExprAST = childAST->getNextSibling();
	else
		primaryExprAST = childAST;

	string inputStreamName; 
	QueryString* thisQuery = _genNFAForPrimaryExpr(primaryExprAST, 
		inputStreamName); //set inputStreamName in the function call 

	if (childAST->getType() ==  P2TokenTypes::UNARY_OPS) {
		//preserve pointer to that last level query so that it can be 
		//optionally inlined with the newly created top-level query here
		//QueryString* lowLevelQuery = thisQuery;
		//create a top level 1-edge NFA here that reads inputStreamName
		//as input, and has predicate childAST->_thisFilterDNF
		Assert (childAST->_thisFilterDNF != NULL);

		outStreamName = _getNewInternalStreamName();

		//if need inline and thisQuery is not NULL, merge thisQuery and the
		//top level one step NFA correponding to the filter expression here
		//into thisQuery
		if (thisQuery 
			&& ConfigMgr::getInstance()->getOptions()->_bInlineQuery) {
			thisQuery = _inlineNFAForFilter(
				thisQuery, 
				childAST->_thisFilterDNF, 
				//childAST->_thisForwardEdgeSchemaMap, 
				primaryExprAST->_thisSchema,
				outStreamName,
				false
				);

			//if inline is successful
			if (thisQuery) {
				return thisQuery;
			}
		}

		//if cannot inline
		//forward edge schema map needs to be obtained from the child
		//primaryExpr
		thisQuery = _createOneStepNFA(
			inputStreamName, 
			outStreamName,
			false,
			primaryExprAST->_thisSchema.toStreamSchema(),
			childAST->_thisFilterDNF,
			childAST->_thisForwardEdgeSchemaMap,
			NULL
			);
	}
	else {
		//will not create a top-level automaton in this case. Just "forward"
		//the output stream name obtained from the call to 
		//_genNFAForPrimaryExpr() to the caller of this function
		outStreamName = inputStreamName;
	}

	return thisQuery;
}

QueryString* NFAGen::_genNFAForPrimaryExpr(RefCayugaQLAST& primaryExprAST,
										   string& outStreamName) {
	QueryString* thisQuery;
	if (primaryExprAST->getType() == P2TokenTypes::STREAM_NAME) {
		//return immediately the stream name.
		//Also, since no new NFA is created, return NULL for the QueryString
		outStreamName = primaryExprAST->getText();
		return NULL;
	}
	else {
		Assert (primaryExprAST->getType() == P2TokenTypes::QUERY);
		thisQuery = _genNFAForQuery(primaryExprAST, outStreamName);
	}

	return thisQuery;
}

QueryString* NFAGen::_createOneStepNFA(
	const string& inStreamName,
	const string& outputStreamName,
	const bool isPublishedStream,
	StreamSchema* pSchema,
	Predicates_DNFString* pPredicate,
	SchemaMapString* pSchemaMap,
	WindowString* pWindowString) {
	//create start node
	NodeID startNodeID;
	StartNodeString* pStartNode 
		= _createNodeString<StartNodeString>(inStreamName, startNodeID);

	//create end node
	NodeID endNodeID;
	IntermediateNodeString* pEndNode 
		= _createNodeString<IntermediateNodeString>("", endNodeID);

	//set schema
	pEndNode->setNodeSchema(pSchema);

	//set output stream
	pEndNode->addOutputStream(outputStreamName, 
		isPublishedStream); 

	//add forward edge from start node to end node
	_createForwardEdgeString(
		pStartNode,
		endNodeID,
		pPredicate,
		pSchemaMap);

	QueryString* thisQuery = new QueryString();
	thisQuery->addNode(pStartNode);
	thisQuery->addNode(pEndNode);

	//added by Rainbow
	//@date  March,26,2011
	if (pWindowString)
	{
		thisQuery->_thisWindow=pWindowString;
	}


	_queryStrings.push_back(make_pair(thisQuery, outputStreamName));


	if (isPublishedStream)
		return NULL;
	else
		return thisQuery;
}

template <typename T>
T* NFAGen::_createNodeString(const string& inStreamName,
										  NodeID& outNodeID) {
	outNodeID = _nodeIDCnt++;
	StringMap attrMap;
	string strNodeID = toString(outNodeID);
	attrMap.insert(make_pair("NodeID", strNodeID));
	if (inStreamName != "")
		attrMap.insert(make_pair("InputStreamName", inStreamName));
	T* pNode = new T(attrMap);
	return pNode;
}

void NFAGen::_createForwardEdgeString(
	NodeString* pSourceNode,
	NodeID destNodeID,
	Predicates_DNFString* pPredicate,
	SchemaMapString* pSchemaMap) {
	//add forward edge from start node to end node
	ForwardEdgeString* pForwardEdge = new ForwardEdgeString();
	pForwardEdge->setDestNodeID(destNodeID);
	pForwardEdge->setDNF(pPredicate);
	pForwardEdge->setSchemaMap(pSchemaMap);

	pSourceNode->addEdge(pForwardEdge);
}


QueryString* NFAGen::_createTwoStepNFA(
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
	) {
	//create start node
	NodeID startNodeID;
	StartNodeString* pStartNode 
		= _createNodeString<StartNodeString>(inStreamName1, startNodeID);

	//create intermediate node
	NodeID intermediateNodeID;
	IntermediateNodeString* pIntermediateNode 
		= _createNodeString<IntermediateNodeString>(inStreamName2, 
		intermediateNodeID);

	//create end node
	NodeID endNodeID;
	IntermediateNodeString* pEndNode 
		= _createNodeString<IntermediateNodeString>("", endNodeID);

	//set schema 
	pIntermediateNode->setNodeSchema(pSchema1);
	pEndNode->setNodeSchema(pSchema2);

	//set output stream
	outputStreamName = _getNewInternalStreamName();
	pEndNode->addOutputStream(outputStreamName, 
		false); //this output stream is only internal, and not published

	//add forward edge from start node to intermediate node
	_createForwardEdgeString(
		pStartNode,
		intermediateNodeID,
		pForwardPredicate1,
		pForwardSchemaMap1);


	//add forward edge from intermediate node to end node
	_createForwardEdgeString(
		pIntermediateNode,
		endNodeID,
		pForwardPredicate2,
		pForwardSchemaMap2);

	//add filter edge to intermediate node
	//Assert(pFilterPredicate != NULL);
	FilterEdgeString* pFilterEdge = new FilterEdgeString();
	pIntermediateNode->addEdge(pFilterEdge);
	pFilterEdge->setDNF(pFilterPredicate);

	//add rebind edge to intermediate node, if there is one
	if (bHasRebindEdge) {
		RebindEdgeString* pRebindEdge = new RebindEdgeString();
		pIntermediateNode->addEdge(pRebindEdge);
		//pForwardEdge2->setDestNodeID(endNodeID);
		pRebindEdge->setDNF(pRebindPredicate);
		pRebindEdge->setSchemaMap(pRebindSchemaMap);
	}

	QueryString* thisQuery = new QueryString();
	thisQuery->addNode(pStartNode);
	thisQuery->addNode(pIntermediateNode);
	thisQuery->addNode(pEndNode);

	//luoluo 8-2
//  	IntermediateNodeString * pIntermediateNode = thisQuery->getSecondNode();
 	predWindowGen * expr = new predWindowGen();
 	expr->visit(streamxprAST);
	//inorder to let the nopredwindow to run ,the following assert was cancled.
/* 	Assert(streamxprAST->_thisPredWindow);*/
 	pIntermediateNode->setRationalPredWindow(streamxprAST->_thisPredWindow);
	thisQuery= _addPredWindow(thisQuery,streamxprAST->_thisPredWindow);

	_queryStrings.push_back(make_pair(thisQuery, outputStreamName));

	return thisQuery;
}

QueryString* NFAGen::_genNFAForSelectClause(RefCayugaQLAST& selectClauseAST,
											const string& inStreamName,
											const string& outStreamName,
											bool isPublishedStream,
											WindowString* pwindowString) {
	//create a top-level 1-step NFA, which reads inStreamName as input,
	//and produces output stream named outStreamName. The output schema is
	//stored in selectClauseAST.
	//There is no predicate on the forward edge, and its schema Map is stored 
	//in selectClauseAST.
	//possible optimization: of select spec is SELECT *, can avoid creating
	//a new NFA. However do not do this, since this optimization will be 
	//included in our general inling optimizations.
												
	QueryString* thisQuery = _createOneStepNFA(
		inStreamName, 
		outStreamName,
		isPublishedStream,
		selectClauseAST->_thisSchema.toStreamSchema(),
		NULL,
		selectClauseAST->_thisForwardEdgeSchemaMap,
		pwindowString
		);

	return thisQuery;
}

string NFAGen::_getNewInternalStreamName() {
	return toString(_internalStreamIDCnt++);
}
QueryString* NFAGen::_addWindow(QueryString* queryString,WindowString* windowString){


	if (queryString&&windowString)
	{
		queryString->_thisWindow=windowString;
	}
	return queryString;
}

QueryString * NFAGen::_addPredWindow(QueryString* queryString,predWindowString* pwinString)
{
	if (queryString && pwinString)
	{
		queryString->_thisPredWindow = pwinString;
	}
	return queryString;
}
