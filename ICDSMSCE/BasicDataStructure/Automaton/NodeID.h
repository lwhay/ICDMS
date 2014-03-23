/**
 * @file NodeID.h
 * @date November 17, 2005
 * @brief Definition of a node ID. Put in a separate file to make it easy for
	      other files to include it.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _NODE_ID_H
#define _NODE_ID_H

#include <cstdio>

namespace Cayuga {
namespace BasicDataStructure {
namespace Automaton {
typedef long NodeID;

#define BAD_NODE_ID -1
}  //namespace Automaton
}  //namespace BasicDataStructure
}  //namespace Cayuga


#endif //_NODE_ID_H
