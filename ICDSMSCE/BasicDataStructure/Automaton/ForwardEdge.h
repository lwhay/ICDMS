/**
 * @file ForwardEdge.h
 * @date November 27, 2005
 * @brief Stores a rebind edge in Cayuga engine.

   ForwardEdge class inherits RebindEdge, since it has some additional stuff
   other than what's in a RebindEdge. Specifically, it's the destination node.

 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _FORWARD_EDGE_H
#define _FORWARD_EDGE_H

#include "RebindEdge.h"

namespace Cayuga {
namespace QueryProcessor {
class FREvaluator;
}

namespace BasicDataStructure {
namespace Automaton {	
class IntermediateNode;

/**
 * @brief A ForwardEdge object represents a forward edge in Cayuga engine.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 27, 2005
 * @version 1.0
 */
class ForwardEdge : public RebindEdge {
	friend class Cayuga::QueryProcessor::FREvaluator;
	friend class Query;
public:
	/**
	* @brief Default constructor
	* @param thisEdgeID the edge ID assigned to this edge
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	ForwardEdge (EdgeID thisEdgeID);

	/**
	* @brief Virtual destructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual ~ForwardEdge();

	/**
	* @brief Output member variable information in XML format.
	* @param out The output stream handle.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual void write(ostream& out = cout) const;

	/**
	* @brief Set the destination node of this node. 
	* @param pDestNode The destination node to set to.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	void setDestNode(IntermediateNode* pDestNode);

	/**
	* @brief Equality comparison operator.
	* @param rhs The input edge for this edge to compare to
	* @return True iff this edge has the same DNF predicate and schema map 
			  program as that of rhs
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual bool operator == (const Edge& rhs) const;
	
	/**
	* @brief Inequality comparison operator.
	* @param rhs The input edge for this edge to compare to
	* @return True iff this edge does not have the same DNF predicate and 
			  schema map program as that of rhs
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual bool operator != (const Edge& rhs) const;

private:
	/** 
	* This variable stores the destination node of this forward edge. 
	*/
	IntermediateNode* _pDestNode;
}; //class ForwardEdge;
}  //namespace Automaton
}  //namespace BasicDataStructure
}  //namespace Cayuga

#endif //_FORWARD_EDGE_H
