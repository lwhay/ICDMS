/**
 * @file Context.h
 * @date November 21, 2005
 * @brief Stores the context information for constructing/compiling
		  a query automaton from AIR.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */


#ifndef _CONTEXT_H
#define _CONTEXT_H

#include "arch-dep.h"

#include "BasicDataStructure/StreamID.h"

#include "BasicDataStructure/Automaton/EdgeID.h"
#include "BasicDataStructure/Automaton/EdgeType.h"
#include "BasicDataStructure/Automaton/NodeID.h"
#include "BasicDataStructure/Automaton/NodeType.h"

#include "AutomatonCompiler/PredIndexSpecString.h"

#include <set>
#include <vector>
using namespace std;

namespace Cayuga {
namespace BasicDataStructure {
namespace Automaton {
class Node;
class ForwardEdge;
class Edge;
class IntermediateNode;
}
}

namespace AutomatonCompiler {
using namespace Cayuga::BasicDataStructure::Automaton;

class ForwardEdgeString;

/**
 * @brief A Context object stores the context information for compiling a
		  query automaton from SAX events.	
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 21, 2005
 * @version 1.0
 */
class Context {
//using namespace Cayuga::BasicDataStructure::Automaton;

public:
	/**
	* @brief Default constructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 21, 2005
	* @version 1.0
	*/
	Context() : _pSourceNode(NULL) {
	}
	
////////////////////////////////////////////////////////////////////////////////
/// Edge information
////////////////////////////////////////////////////////////////////////////////

	/**
	* This variable stores the pointer to the current edge being constructed.
	*/	
	Edge* _pCurrentEdge;

	/**
	* This variable stores the type of the current edge being constructed.
	*/	
	EdgeType _edgeType;
	

	/**
	* This variable stores the ID of the next forward edge to be constructed.
	*/	
	EdgeID _nextForwardEdgeID;

	/**
	* This variable stores the instruction length of the current predicate
	* being constructed.
	*/
	size_t _predInstructionLength;

	/**
	* This variable stores the ordinal of the current conjunctive clause
	* being constructed.
	*/
	unsigned int _clauseNo;

////////////////////////////////////////////////////////////////////////////////
/// Node information
////////////////////////////////////////////////////////////////////////////////
	
	/**
	* This variable stores the pointer to the current node being constructed.
	*/	
	Node* _pSourceNode;
	
	/**
	* This variable stores the type of the current node being constructed.
	*/	
	NodeType _nodeType;

	/**
	* This variable stores the input stream ID of the current node being 
	* constructed.
	*/
	StreamID _streamID;
	
	/**
	* This variable stores the predicate index specs of the current node
	* being constructed.
	*/
	vector<PredIndexSpecString> _predIndices;

	/**
	* This variable stores the dynamic predicate index specs of the current
	* node being constructed.
	*/
	PredIndexSpecString _dynFilterPredIndex;

	/**
	* This variable stores whether the current node under construction has a 
	* Dynamic Filter Index.
	*/
	bool _isDynIndexed;


////////////////////////////////////////////////////////////////////////////////
/// Accumulated context information
////////////////////////////////////////////////////////////////////////////////
	
	/** 
	* This variable stores entries that map from node IDs read from AIR to a 
	* pointers to the internalized intermediate nodes.
	
	* An entry is produced when a new internal intermediate node is created. 
	*/
	hash_map<NodeID, IntermediateNode*> _nodeMap;

	//work set

	/** 
	* This variable stores entries that maps from destination node IDs to 
	* forward edge pointers.
	
	* Entries are produced when forward edges are created, at which point
	* the destination node is unknown (_nodeMap does not have information of the 
	* destination node).
	
	* Entries are consumed when nodes are created, at which point we can "patch" 
	* the destination node ID field of forward edges already created. 
	*/
	hash_map<NodeID, ForwardEdge*> _patchDestNode;
	//KeySetValueMap<NodeID, ForwardEdge*> _patchDestNode;

	/** 
	* This variable stores entries that map from destination node IDs to 
	* triples of forward edge, forward edge string, and source node pointer.
	
	* Entries are produced when forward edges are created, at which point
	* the schema of the destination node is unknown (_nodeMap does not have 
	* information of the destination node).
	
	* Entries are consumed when nodes are created, at which point we can "patch" 
	* the schema map field of forward/rebind edges already created. 
	*/
	hash_map<NodeID, pair<pair<ForwardEdge*, ForwardEdgeString*>, Node*> >
		_patchSchemaMap;
	//KeySetValueMap<NodeID, EdgeEdgeStringNode*> _patchSchemaMap;

}; //class Context
}  //namespace AutomatonCompiler
}  //namespace Cayuga

#endif //_CONTEXT_H
