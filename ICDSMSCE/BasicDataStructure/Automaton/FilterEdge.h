/**
 * @file FilterEdge.h
 * @date November 10, 2005
 * @brief Stores a filter edge in Cayuga engine.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _FILTER_EDGE_H
#define _FILTER_EDGE_H

#include "Edge.h"

namespace Cayuga {
namespace BasicDataStructure {
namespace Automaton {
/**
 * @brief A FilterEdge object represents a filter edge in Cayuga engine.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 27, 2005
 * @version 1.0
 */
class FilterEdge : public Edge {
public:
	/**
	* @brief Default constructor
	* @param thisEdgeID the edge ID assigned to this edge
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	FilterEdge (EdgeID thisEdgeID);

	/**
	* @brief Virtual destructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual ~FilterEdge();

	/**
	* @brief Output member variable information in XML format.
	* @param out The output stream handle.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual void write(ostream& out = cout) const;

}; //class FilterEdge
}  //namespace Automaton
}  //namespace BasicDataStructure
}  //namespace Cayuga

#endif //_FILTER_EDGE_H
