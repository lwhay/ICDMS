/**
 * @file ForwardEdge.cpp
 * @date November 27, 2005
 * @brief Implementation file of ForwardEdge class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "BasicDataStructure/Automaton/ForwardEdge.h"
#include "BasicDataStructure/Automaton/IntermediateNode.h"
using namespace Cayuga::BasicDataStructure::Automaton;

#include <cassert>
using namespace std;


ForwardEdge::ForwardEdge (EdgeID thisEdgeID) : RebindEdge(thisEdgeID), 
_pDestNode(NULL) {
}

ForwardEdge::~ForwardEdge() {
}

void ForwardEdge::setDestNode(IntermediateNode* pDestNode) {
	_pDestNode = pDestNode;
}

bool ForwardEdge::operator == (const Edge& rhs) const {
	if (RebindEdge::operator !=(rhs)) {
		return false;
	}
	return true;
}


bool ForwardEdge::operator != (const Edge& rhs) const {
	return !(ForwardEdge::operator ==(rhs));
}

void ForwardEdge::write(ostream& out) const {
	assert (this->_pDestNode != NULL);
	out<<"<ForwardEdge Destination=\""<<_pDestNode->getID()<<"\">"<<endl;
	Edge::write(out);
	//out<<"/>"<<endl;

	_schemaMap.write(out);

	out<<"</ForwardEdge>"<<endl;
}
