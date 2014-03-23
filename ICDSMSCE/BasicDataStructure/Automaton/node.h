/**
 * @file Node.h
 * @date November 27, 2005
 * @brief Based class of automaton nodes in Cayuga engine.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _NODE_H
#define _NODE_H

#include "BasicDataStructure/Automaton/NodeID.h"
using Cayuga::BasicDataStructure::Automaton::NodeID;

#include "BasicDataStructure/Automaton/EdgeID.h"
using Cayuga::BasicDataStructure::Automaton::EdgeID;

#include "BasicDataStructure/Predicate/AttributeID.h"
#include "BasicDataStructure/StreamID.h"

#include <iostream>
#include <map>
#include <typeinfo>
using namespace std;

#include "arch-dep.h"

namespace CY_Heap {
class GHandle;
}

namespace Cayuga {
namespace QueryProcessor {
class FilterEvaluator;
class FREvaluator;
class QueryProcessorImpl;
}


namespace Index {
template <typename ID_T, typename ID_SET_ENTRY, typename ID_SET> class HashIndex;
}

namespace BasicDataStructure {
namespace Automaton {
class Instance;
class Edge;

using Cayuga::BasicDataStructure::StreamID;
using Cayuga::Index::HashIndex;
using Cayuga::BasicDataStructure::Predicate::AttributeID;

/**
 * @brief A Node object represents an automaton node in Cayuga engine.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 27, 2005
 * @version 1.0
 */
class Node {
	friend class Cayuga::QueryProcessor::FilterEvaluator;
	//friend class Cayuga::QueryProcessor::FREvaluator;
	//friend class Cayuga::QueryProcessor::QueryProcessorImpl;
	//friend class InstanceFinder;

public:
	/** 
	* A pair of node pointer and a non-genative integer, denoting which
	* Conjunctive clause of the FR edge predicate this is on 
	*/
	typedef pair<EdgeID, size_t> HashIndexIDType;
	typedef HashIndex<HashIndexIDType*, HashIndexIDType*,
		hash_set<HashIndexIDType*> > HashIndexType;
	typedef map<size_t, HashIndexType*> PredicateIndex;

	/**
	* @brief Default constructor
	* @param inputStream Input stream of this node
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	Node(const StreamID inputStream);
	
	/**
	* @brief Virtual destructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual ~Node();

	/**
	* @brief Output member variable information in XML format.
	* @param out The output stream handle.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual void write(ostream& out = cout) const = 0;

	/**
	* @brief Get the type of this node.
	* @return As above.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual const type_info& getNodeType();

	/**
	* @brief Get the input stream on this node.
	* @return As above.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual StreamID getInputStream();

	/**
	* @brief Add an out-going edge to this node.
	* @param pEdge Pointer to the edge to add
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual void addEdge(Edge* pEdge) = 0;

	/**
	* @brief Add an index specification associated with FR edges of this node
	* @param pIndex Pointer to the index to add
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 6, 2006
	* @version 1.0
	*/
	virtual void addIndexSpec(HashIndexType* pIndex);


	/**
	* @brief Get the ID of this node
	* @return As above
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 16, 2006
	* @version 1.0
	*/
	NodeID getID() const;

	/**
	* @brief Set the input stream name of the node
	* @param streamName The input stream name to set
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 3, 2007
	* @version 1.0
	*/
	void setInputStreamName(const string& streamName);

	/**
	* This variable stores the set of nodes in the system.
	*/
	static vector<Node*> _nodes;

protected:
	/**
	* This variable stores the ID of this node assigned by Cayuga.
	*/
	NodeID _nodeID;

	/** 
	* This variable stores the ID of input stream of this node.
	*/
	StreamID _inputStream;

	/** 
	* This variable stores the ID of input stream of this node.
	*/
	string _inputStreamName;

	/**
	* This variable stores the set of FR predicate indices associated with
	* this node.
	*/
	PredicateIndex _index;

	/**
	* This variable stores the number of node IDs used by Cayuga.
	*/
	static NodeID _nodeIDCnt;

}; //class Node
}  //namespace Automaton
}  //namespace BasicDataStructure
}  //namespace Cayuga

#endif //_NODE_H
