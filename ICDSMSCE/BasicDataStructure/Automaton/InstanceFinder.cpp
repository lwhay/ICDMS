/**
 * @file InstanceFinder.cpp
 * @date December 9, 2005
 * @brief Implementation file of InstanceFinder class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "BasicDataStructure/Automaton/InstanceFinder.h"
#include "BasicDataStructure/Automaton/IntermediateNode.h"
using namespace Cayuga::BasicDataStructure::Automaton;

#include "BasicDataStructure/Event/Event.h"
using Cayuga::BasicDataStructure::EventNS::Event;

#include "MemoryMgr/CY_HeapFace.h"
using namespace CY_Heap;

#include <cassert>
using namespace std;

InstanceFinder::InstanceFinder(hash_set<NodeID>& nodesWithInstance) 
: RootFinder(), _nodesWithInstance(nodesWithInstance) {
	reset();
}

InstanceFinder::~InstanceFinder() {
}

void InstanceFinder::reset() {
	if (_nodesWithInstance.empty()) {
		_currNode_it = _nodesWithInstance.begin();
		assert (_nodesWithInstance.begin() == _nodesWithInstance.end());
		return;
	};
	_currNode_it = _nodesWithInstance.begin();
	IntermediateNode* pNode 
		= dynamic_cast<IntermediateNode*>(Node::_nodes[*_currNode_it]);
	assert (pNode);
	assert (pNode->hasInstance());
	_currInstance_it = pNode->_instances.begin();
}

int InstanceFinder::fill(unsigned bufsz, CY_Heap::GHandle * buf) {
	/* assume bufsz always big enough for at least one instance */
	GHandle *p = buf;
	while(_currNode_it != _nodesWithInstance.end()) {
		IntermediateNode* currNode 
			= dynamic_cast<IntermediateNode*>(Node::_nodes[*_currNode_it]);
		assert (currNode);
		unsigned nh = (unsigned)currNode->getHandleBytes();
		nh = nh / sizeof(GHandle);  // number of handles, not bytes
		while( _currInstance_it != currNode->_instances.end() ) {
			if( nh > bufsz ) break;
			currNode->copyHandles( p, *_currInstance_it);
			p += nh;  bufsz -= nh;  ++_currInstance_it;
		}
		++_currNode_it;
		if (_currNode_it != _nodesWithInstance.end()) {
			IntermediateNode* pNode 
				= dynamic_cast<IntermediateNode*>(Node::_nodes[*_currNode_it]);
			assert (pNode);
			assert (pNode->hasInstance());
			_currInstance_it = pNode->_instances.begin();
		}
	}
	return (p - buf);
}
