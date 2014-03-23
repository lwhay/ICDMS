/**
 * @file FREvaluator.h
 * @date November 26, 2005
 * @brief FR Evaluator evaluates FR edge predicates.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _FR_EVALUATOR_H
#define _FR_EVALUATOR_H

#include "arch-dep.h"

#include "BasicDataStructure/Predicate/AttributeID.h"
#include "BasicDataStructure/Automaton/EdgeID.h"
using Cayuga::BasicDataStructure::Automaton::EdgeID;

#include "BasicDataStructure/Automaton/NodeID.h"
using Cayuga::BasicDataStructure::Automaton::NodeID;

#include "Utils/Singleton.h"

#include "IndexMap.h"


#include <iostream>
#include <fstream>
#include <map>
#include <vector>
using namespace std;

namespace Cayuga {

namespace AutomatonCompiler {
class PredIndexSpecString;
class Context;
class NormalAtomPredString;
}

namespace BasicDataStructure {
namespace Automaton {
class Node;
class StartNode;
class IntermediateNode;
class Edge;
class ForwardEdge;
class RebindEdge;
class Instance;
}

namespace EventNS {
class Event;
}
}

namespace Index {
template <typename ID_T, typename ID_SET_ENTRY, typename ID_SET> class HashIndex;
}

namespace QueryProcessor {
using Cayuga::AutomatonCompiler::PredIndexSpecString;
using Cayuga::AutomatonCompiler::Context;
using Cayuga::AutomatonCompiler::NormalAtomPredString;

using Cayuga::BasicDataStructure::Automaton::Node;
using Cayuga::BasicDataStructure::Automaton::StartNode;
using Cayuga::BasicDataStructure::Automaton::IntermediateNode;
using Cayuga::BasicDataStructure::Automaton::Edge;
using Cayuga::BasicDataStructure::Automaton::RebindEdge;
using Cayuga::BasicDataStructure::Automaton::ForwardEdge;
using Cayuga::BasicDataStructure::Automaton::Instance;
using Cayuga::BasicDataStructure::EventNS::Event;
using Cayuga::BasicDataStructure::Predicate::AttributeID;

using Cayuga::Index::HashIndex;

using Cayuga::Utils::Singleton;

using Cayuga::QueryProcessor::IndexMap;

class QueryProcessorImpl;

class FREvaluator;
/**
 * @brief An FREvaluator object represents an FR evaluator.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 26, 2005
 * @version 1.0
 */
class FREvaluator  : public Singleton<FREvaluator> {
	typedef vector<AttributeID> _HashPredIndexSpec;

	/** 
	* A pair of node pointer and a non-genative integer, denoting which
	* Conjunctive clause of the FR edge predicate this is on 
	*/
	typedef pair<EdgeID, size_t> HashIndexIDType;

	typedef HashIndex<HashIndexIDType*,HashIndexIDType*,
		hash_set<HashIndexIDType*> > HashIndexType;


	struct PredKey {
		PredKey(NodeID nodeID, EdgeID edgeID, size_t clauseID) 
			: _nodeID(nodeID), _edgeID(edgeID), _clauseID(clauseID) {
		}

		bool operator < (const PredKey& rhs) const {
			if (_nodeID < rhs._nodeID) return true;

			if (_nodeID == rhs._nodeID && _edgeID < rhs._edgeID) return true; 
			
			if (_nodeID == rhs._nodeID && _edgeID == rhs._edgeID
				&& _clauseID < rhs._clauseID) return true; 
			
			return false;
		}

		bool operator == (const PredKey& rhs) const {
			if (_nodeID == rhs._nodeID && _edgeID == rhs._edgeID
				&& _clauseID == rhs._clauseID) return true; 
			
			return false;
		}


		NodeID _nodeID;
		EdgeID _edgeID;
		size_t _clauseID;
	};

	typedef map<PredKey, HashIndexIDType*> HashIndexMap;
public:
	/**
	* @brief Default constructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 26, 2005
	* @version 1.0
	*/
	FREvaluator();

	/**
	* @brief Virtual destructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 26, 2005
	* @version 1.0
	*/
	~FREvaluator();

	/**
	* @brief Evaluate the input event and create new instances.
	* @param ev The input event to evaluate
	* @param pNode The current node being processed
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 26, 2005
	* @version 1.0
	*/
	void eval(/*const*/ Event* ev, Node* pNode);

	/**
	* @brief Set the query processor associated with this FR evaluator.
	* @param qpImpl The query processor to set
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 26, 2005
	* @version 1.0
	*/
	void setQP(QueryProcessorImpl* qpImpl);

	/**
	* @brief Add a predicate index to this FR evaluator.
	* @param indexSpec The spec of the predicate index to add
	* @param context The compilation context of this index
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 26, 2005
	* @version 1.0
	*/
	void addIndexSpec(const PredIndexSpecString& indexSpec, 
		const Context& context);

	/**
	* @brief Insert a predicate to the predicate index of this FR evaluator.
	* @param context The compilation context of this index
	* @param pHashIndexSpec Pointer to the spec of the index into which the 
							predicate should be inserted. If NULL, the default
							"index", a sequential list, is used.
	* @param indexedPreds The predicate to insert
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 26, 2005
	* @version 1.0
	*/
	void insertPred(const Context& context, 
		const vector<AttributeID>* pHashIndexSpec,
		const vector<NormalAtomPredString*>& indexedPreds);

	/** 
	* @brief Change the key of the predicates associated with pOldNode to 
	         pNewNode 
	* @param pOldNode The old node where the old edge is associated
	* @param pEdge The forward edge to be moved
	* @param oldEdgeID The ID of the old forward edge with which the 
			 indexed predicates are currently associated
	* @param pNewNode The new node with which the predicates are to be 
			 associated
	* @param newEdgeID The ID of the new forward edge
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 26, 2005
	* @version 1.0
	*/
	void changeForwardPredKey(StartNode* pOldNode,
		ForwardEdge* pEdge,
		EdgeID oldEdgeID, 
		StartNode* pNewNode,
		EdgeID newEdgeID);

	/**
	* @brief Delete a forward edge on a particular node from this FR evaluator.
	* @param pOldNode The automaton node associated with which the forward 
			 edge is to be deleted.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 26, 2005
	* @version 1.0
	*/
	void deleteForwardEdgeOnNode(StartNode* pOldNode);
	
	/**
	* @brief Delete a rebind edge on a particular node from this FR evaluator.
	* @param pOldNode The automaton node associated with which the rebind 
			 edge is to be deleted.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 26, 2005
	* @version 1.0
	*/
	void deleteRebindEdgeOnNode(IntermediateNode* pOldNode);

#ifdef _DEBUG
	/**
	* @brief Output the collected statistics of this component
	* @param out The output stream handle.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 26, 2005
	* @version 1.0
	*/
	void outputStats(ostream& out = cout);
#endif //_DEBUG

private:
	/**
	* @brief Evaluate the static FR predicates, and link those forward edges 
			 of whose static parts are satisfied.
	* @param pNode The node of which all FR predicates are to be evaluated
	* @param ev The current event content
	* @return True iff the caller should continue to evaluate the 
			remaining predicates.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 26, 2005
	* @version 1.0
	*/
	bool evalStaticForwardPred(StartNode* pNode, /*const*/ Event* ev);

	/**
	* @brief Evaluate the dynamic predicate of a given edge, and if satisfied, 
	* make a new instance and put it on the destination node of that edge.
	* @param pNode The current node being processed
	* @param pCurEdge The current edge being processed
	* @param pInst The current instance being processed
	* @param ev The current event content
	* @return True iff the edge predicate is satisfied
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 26, 2005
	* @version 1.0
	*/
	bool evalRemainingPredAndMakeNewInstance(Node* pNode,
		RebindEdge* pCurEdge, 
		Instance* pInst, 
     	/*const*/  Event* ev);

	/**
	* @brief Evaluate the dynamic predicate of a given edge, and if satisfied, 
	* make a new instance and put it on the destination node of that edge.
	* @param pNode The current node being processed
	* @param pCurEdge The current edge being processed
	* @param pInst The current instance being processed
	* @param ev The current event content
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 26, 2005
	* @version 1.0
	*/
	//void outputEvent(IntermediateNode* pDestNode, 
	//	Instance* pNewInstance,
	//	ostream& out = cout);

	/** 
	* @brief Change the last part of the key of the predicate on the given edge,
			 the node which it is associated with, to the input node
	* @param pOldNode The old node where the old edge is associated
	* @param pOldEdge The old edge which the predicate is current on
	* @param pNewNode The new node with which the predicate is to be 
			 associated
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 26, 2005
	* @version 1.0
	*/
	//void changePredKey(StartNode* pOldNode, Edge* pOldEdge, Node* pNewNode);
	
	/** 
	* @brief Delete the predicates on the given edge
	* @param pOldNode The node where the edge is associated
	* @param pOldEdge The old edge from which the predicates are to be deleted
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 26, 2005
	* @version 1.0
	*/
	void deletePredOnEdge(StartNode* pOldNode,
		Edge* pOldEdge);

	/**
	* This variable stores the pointer to the query processor associated with
	* FR filter evaluator.
    */
	QueryProcessorImpl* _qpImpl;

	/**
	* This variable stores the set of predicate indices associated with
	* this FR evaluator.
    */
	vector<HashIndexType*> _frIndices;
	
	/**
	* This variable stores the set of predicate index specs associated with
	* this FR evaluator.
    */
	vector<_HashPredIndexSpec> _indexSpec;
	
	/**
	* This variable stores the map from index schemas to indices associated with
	* this FR evaluator.
    */
	IndexMap<HashIndexType> _indexMap;

	/**
	* This variable stores the map from edges to their FR predicates.
    */
	HashIndexMap _predIDMap;

	HashIndexType* pDefaultHashIndex;		
}; //class FREvaluator
}  //namespace QueryProcessor
}  //namespace Cayuga

#endif //_FR_EVALUATOR_H
