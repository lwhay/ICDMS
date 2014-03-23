/**
 * @file FilterEvaluator.h
 * @date November 26, 2005
 * @brief Filter Evaluator evaluates filter edge predicates.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _FILTER_EVALUATOR_H
#define _FILTER_EVALUATOR_H

#include "Utils/Singleton.h"

#include "BasicDataStructure/Automaton/NodeID.h"
using Cayuga::BasicDataStructure::Automaton::NodeID;

#include "BasicDataStructure/Event/EventID.h"
using Cayuga::BasicDataStructure::EventNS::EventID;

#include "BasicDataStructure/Predicate/AttributeID.h"

#include "BasicDataStructure/StreamID.h"

#include "IndexMap.h"

#include <iostream>
#include <list>
using namespace std;

namespace Cayuga {
	using Cayuga::BasicDataStructure::StreamID;

namespace AutomatonCompiler {
class PredIndexSpecString;
class Context;
class NormalAtomPredString;
}

namespace BasicDataStructure {
namespace Automaton {
class Node;
class IntermediateNode;
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

class QueryProcessorImpl;

using Cayuga::AutomatonCompiler::PredIndexSpecString;
using Cayuga::AutomatonCompiler::Context;
using Cayuga::AutomatonCompiler::NormalAtomPredString;

using Cayuga::BasicDataStructure::Automaton::Node;
using Cayuga::BasicDataStructure::Automaton::Instance;
using Cayuga::BasicDataStructure::Automaton::IntermediateNode;
using Cayuga::BasicDataStructure::EventNS::Event;
using Cayuga::Index::HashIndex;

/**
* @brief A FilterIndex object is a predicate index to be used 
		 in event processing.
* @author Mingsheng Hong (mshong@cs.cornell.edu)
* @date November 26, 2005
* @version 1.0
*/
class FilterIndex {
	/** 
	* A pair of node pointer and a non-genative integer, denoting which
	* Conjunctive clause of the filter edge predicate this is on 
	*/
	typedef pair<NodeID, size_t> HashIndexIDType;

	typedef HashIndex<HashIndexIDType*,HashIndexIDType*,
		hash_set<HashIndexIDType*> > HashIndexType;

	typedef hash_multimap<NodeID, HashIndexIDType*> HashIndexMap;
public:
	/**
	* @brief Default constructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 26, 2005
	* @version 1.0
	*/
	FilterIndex();

	/**
	* @brief Virtual destructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 26, 2005
	* @version 1.0
	*/
	~FilterIndex();
	
	/**
	* @brief Evaluate the input event.
	* @param ev The input event to evaluate
	* @param affectedNodes The set of nodes whose filter edges are not 
			 traversed. This include start nodes reading the stream the 
			 current event comes from.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 26, 2005
	* @version 1.0
	*/
	void eval(const Event* ev,
		hash_set<NodeID>& affectedNodes);

	/**
	* @brief Add a predicate index to this filter evaluator.
	* @param indexSpec The spec of the predicate index to add
	* @param context The compilation context of this index
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 26, 2005
	* @version 1.0
	*/
	void addIndexSpec(const PredIndexSpecString& indexSpec, 
		const Context& context);

	/**
	* @brief Insert a predicate to the predicate index of this filter evaluator.
	* @param context The compilation context of this index
	* @param pHashIndexSpec Pointer to the spec of the index into which the 
							predicate should be inserted. If NULL, it is the 
							special "index" where predicates not indexed are
							stored.
	* @param indexedPreds The predicate to insert
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 26, 2005
	* @version 1.0
	*/
	void insertPred(const Context& context, 
		const vector<AttributeID>* pHashIndexSpec,
		const vector<NormalAtomPredString*>& indexedPreds);

	/**
	* @brief Delete a predicate on a particular node from the predicate index 
			 of this filter evaluator.
	* @param pOldNode The automaton node on which the predicate is to be 
			 deleted.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 26, 2005
	* @version 1.0
	*/
	void deletePredOnNode(IntermediateNode* pOldNode);

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
	* This variable stores the set of predicate indices associated with
	* this filter evaluator.
    */
	vector<HashIndexType*> _index;
	
	/**
	* This variable stores the set of predicate index specs associated with
	* this filter evaluator.
    */
	vector<_HashPredIndexSpec> _indexSpec;
	
	/**
	* This variable stores the map from index schemas to indices associated with
	* this filter evaluator.
    */
	IndexMap<HashIndexType> _indexMap;

	/**
	* This variable stores the map from nodes to their filter predicates.
    */
	HashIndexMap _predIDMap;


	/**
	* This variable points to the default "index" where predicates no indexed 
	* are associated.
	*/
	HashIndexType* pDefaultHashIndex;
};

class FilterEvaluator;
/**
 * @brief A FilterEvaluator object represents a filter evaluator.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 26, 2005
 * @version 1.0
 */
class FilterEvaluator : public Cayuga::Utils::Singleton<FilterEvaluator> {	
	typedef vector<pair<AttributeID, AttributeID> > _HashDynPredIndexSpec;
	typedef HashIndex<EventID, pair<EventID, Instance*>, 
		hash_map<EventID, Instance*> > DynHashIndexType;	
public:
	/**
	* @brief Default constructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 26, 2005
	* @version 1.0
	*/
	FilterEvaluator();

	/**
	* @brief Virtual destructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 26, 2005
	* @version 1.0
	*/
	~FilterEvaluator();

	/**
	* @brief Set the query processor associated with this filter evaluator.
	* @param qpImpl The query processor to set
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 26, 2005
	* @version 1.0
	*/
	void setQP(QueryProcessorImpl* qpImpl) ;

	/**
	* @brief Insert the input start node into an appropriate filter index, so
			that when processing each input event on the stream which node
			reads, this node will be active -- returned by FilterEvaluator.
	* @param startNode The input start node
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 11, 2007
	* @version 1.0
	*/
	void indexStartNode(Node* startNode);

	FilterIndex* getFilterIndex(const StreamID streamID);

	/**
	* @brief Evaluate the input event.
	* @param ev The input event to evaluate
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 26, 2005
	* @version 1.0
	*/
	void eval(/*const*/ Event* ev);

	/**
	* @brief Add a predicate index to this filter evaluator.
	* @param indexSpec The spec of the predicate index to add
	* @param context The compilation context of this index
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 26, 2005
	* @version 1.0
	*/
	void addIndexSpec(const PredIndexSpecString& indexSpec, 
		const Context& context);

	void addDynIndexSpec(const PredIndexSpecString& indexSpec,
						   const Context& context);


	/**
	* @brief Insert a predicate to the predicate index of this filter evaluator.
	* @param context The compilation context of this index
	* @param pHashIndexSpec Pointer to the spec of the index into which the 
							predicate should be inserted. If NULL, it is the 
							special "index" where predicates not indexed are
							stored.
	* @param indexedPreds The predicate to insert
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 26, 2005
	* @version 1.0
	*/
	void insertPred(const Context& context, 
		const vector<AttributeID>* pHashIndexSpec,
		const vector<NormalAtomPredString*>& indexedPreds);

	void indexInstance(IntermediateNode* currNode,		  			
					list<Instance*>& Instances);

	void deleteInstance(IntermediateNode* pNode, Instance* pInst);

	/**
	* @brief Delete a predicate on a particular node from the predicate index 
			 of this filter evaluator.
	* @param pOldNode The automaton node on which the predicate is to be 
			 deleted.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 26, 2005
	* @version 1.0
	*/
	void deletePredOnNode(IntermediateNode* pOldNode);

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
	* This variable stores the pointer to the query processor associated with
	* this filter evaluator.
    */
	QueryProcessorImpl* _qpImpl;


	//manages a FilterIndex object per stream
	hash_map<StreamID, FilterIndex*> _filterIndices;

}; //class FilterEvaluator
}  //namespace QueryProcessor
}  //namespace Cayuga

#endif //_FILTER_EVALUATOR_H
