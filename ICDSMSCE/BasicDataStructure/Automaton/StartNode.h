/**
 * @file StartNode.h
 * @date November 27, 2005
 * @brief Stores start nodes in Cayuga engine.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _START_NODE_H
#define _START_NODE_H

#include "node.h"

#include <vector>
using namespace std;

namespace Cayuga {
namespace QueryProcessor {
class FREvaluator;
}

namespace SystemMgr {
class StreamSchema;
}

namespace BasicDataStructure {
namespace Automaton {
class Edge;
class FilterEdge;
class RebindEdge;
class ForwardEdge;

/**
 * @brief A StartNode object represents a start node in Cayuga engine.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 27, 2005
 * @version 1.0
 */
class StartNode : public Node {
	friend class Cayuga::QueryProcessor::FREvaluator;
	friend class Query;
	
public:
	typedef vector<ForwardEdge*> ForwardEdgeSet;

	/**
	* @brief Default constructor
	* @param inputStream Input stream of this node
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	StartNode(const StreamID inputStream);

	/**
	* @brief Virtual destructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual ~StartNode();

	/**
	* @brief Output member variable information in XML format.
	* @param out The output stream handle.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual void write(ostream& out = cout) const;

	/**
	* @brief Return true iff this node has the same content as that of the 
			 input node. (They read the same input stream). Used for state 
			 merging procedure.
    * @param rhs The node to compare to
	* @return As above.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 6, 2006
	* @version 1.0
	*/
	bool operator ==(const StartNode& rhs) const;

	/**
	* @brief Add an out-going edge to this node.
	* @param pEdge Pointer to the edge to add
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual void addEdge(Edge* pEdge);

	/**
	* @brief Find a forward edge of the same content of that of the input edge.
	* @param rhs Pointer to the edge to find
	* @return Pointer to the found edge, or NULL if there does not exist one
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 6, 2006
	* @version 1.0
	*/
	virtual ForwardEdge* findSameForwardEdge(const ForwardEdge* rhs) const;

	/**
	* @brief Clear the sword edges of this node.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 20, 2006
	* @version 1.0
	*/
	virtual void clearSwordEdges();

	/**
	* @brief Get the next forward edge ID of this node.
	* @return As above.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 28, 2007
	* @version 1.0
	*/
	EdgeID getNextForwardEdgeID();

protected:
	/**
	* @brief Output node header information in XML format.
	* @param out The output stream handle.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual void writeCommonPart1(ostream& out = cout) const;
	
	/**
	* @brief Output forward edge information in XML format.
	* @param out The output stream handle.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual void writeCommonPart2(ostream& out = cout) const;

	/** 
	* This variable stores a set of at least one forward edges 
	*/
	ForwardEdgeSet _pForwards;

	/** 
	* This variable stores the head of a sword edge.

	A sword edge is defined to be a forward/rebind edge with static pred part 
	being satisfied on some clause. 
	*/
	RebindEdge* _pHeadSword;
}; //class StartNode
}  //namespace Automaton
}  //namespace BasicDataStructure
}  //namespace Cayuga


#endif //_START_NODE_H
