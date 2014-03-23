/**
 * @file EdgeID.h
 * @date April 28, 2007
 * @brief Definition of an edge ID. It is a local ID for each node, with 
		  the optional rebind edge being assigned ID REBIND_EDGE_ID.
		  Forward edge has ID starting from 0.
		  Put in a separate file to make it easy for
	      other files to include it.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _EDGE_ID_H
#define _EDGE_ID_H

#include <cstdio>

namespace Cayuga {
namespace BasicDataStructure {
namespace Automaton {
typedef long EdgeID;

#define REBIND_EDGE_ID -1

//we are not going to read FILTER_EDGE_ID for now
#define FILTER_EDGE_ID -2

#define INVALID_EDGE_ID -3

}  //namespace Automaton
}  //namespace BasicDataStructure
}  //namespace Cayuga


#endif //_EDGE_ID_H
