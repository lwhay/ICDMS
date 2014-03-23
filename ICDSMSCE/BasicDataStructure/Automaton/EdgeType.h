/**
 * @file EdgeType.h
 * @date November 10, 2005
 * @brief Type of an automaton edge.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef EDGE_TYPE_H
#define EDGE_TYPE_H

namespace Cayuga {
namespace BasicDataStructure {
namespace Automaton {

/**
 * @brief An EdgeType object represents the type of an automaton edge.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 10, 2005
 * @version 1.0
 */
enum EdgeType {
	FILTER,
	REBIND,
	FORWARD
}; //enum EdgeType
}  //namespace Automaton
}  //namespace BasicDataStructure
}  //namespace Cayuga

#endif //EDGE_TYPE_H
