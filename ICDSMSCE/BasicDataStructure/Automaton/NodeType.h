/**
 * @file NodeType.h
 * @date November 14, 2005
 * @brief Type of an automaton node in Cayuga engine.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */


#ifndef NODE_TYPE_H
#define NODE_TYPE_H

namespace Cayuga {
namespace BasicDataStructure {
namespace Automaton {
/**
 * @brief A NodeType object represents the type of an automaton node.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 14, 2005
 * @version 1.0
 */
enum NodeType {
	START = 0,
	INTERMEDIATE_OR_END,
}; //enum NodeType
}  //namespace Automaton
}  //namespace BasicDataStructure
}  //namespace Cayuga


#endif //NODE_TYPE_H
