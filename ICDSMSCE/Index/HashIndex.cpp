/**
 * @file HashIndex.cpp
 * @date November 27, 2005
 * @brief Implementation file of HashTableMonitor class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "BasicDataStructure/Automaton/NodeID.h"
using Cayuga::BasicDataStructure::Automaton::NodeID;

#include "BasicDataStructure/Automaton/EdgeID.h"
using Cayuga::BasicDataStructure::Automaton::EdgeID;

#include "BasicDataStructure/Event/EventID.h"
using Cayuga::BasicDataStructure::EventNS::EventID;

#include "Index/HashIndexDef.h"
using namespace Cayuga::Index;

#include <map>
using namespace std;

namespace Cayuga {
namespace BasicDataStructure {
namespace Automaton {
class Node;
class Edge;
class Instance;
}; //Automaton
}; //BasicDataStructure
}; //Cayuga

using namespace Cayuga::BasicDataStructure::Automaton;

////////////////////////////////////////////////////////////////////////////////
// Explicit instantiations for ValueIDMap
////////////////////////////////////////////////////////////////////////////////
template class HashIndex<pair<long, size_t>*, 
						pair<long, size_t>*,
						hash_set<pair<long, size_t>*> >;

//template class HashIndex<pair<Node*, unsigned int>*>;
//template class HashIndex<pair<NodeID, size_t>*>;

//template class HashIndex<pair<Edge*, unsigned int>*>;
//template class HashIndex<pair<EdgeID, size_t>*>;

//template class HashIndex<Instance*>;
template class HashIndex<EventID, 
						pair<EventID, Instance*>, 
						hash_map<EventID, Instance*> 
>;
