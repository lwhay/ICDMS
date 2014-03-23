/**
 * @file StartNode.cpp
 * @date November 27, 2005
 * @brief Implementation file of StartNode class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "BasicDataStructure/Automaton/StartNode.h"
#include "BasicDataStructure/Automaton/ForwardEdge.h"
using namespace Cayuga::BasicDataStructure::Automaton;

#include <cassert>
using namespace std;

StartNode::StartNode(const StreamID inputStream) 
: Node(inputStream), _pHeadSword(NULL) {
}

StartNode::~StartNode() {
	for (ForwardEdgeSet::iterator edge_it = _pForwards.begin();
		 edge_it != _pForwards.end(); ++edge_it) {
		delete *edge_it;
	}
}

void StartNode::addEdge(Edge* pEdge) {
	assert (pEdge->getEdgeType() == typeid(ForwardEdge));
	pEdge->setID(_pForwards.size());
	_pForwards.push_back((ForwardEdge*)pEdge);
}

void StartNode::writeCommonPart1(ostream& out) const {
	out<<"<Node NodeID=\""<<_nodeID<<"\" ";
	out<<"NodeType=\"";
	if (typeid(*this) == typeid(StartNode)) {
		out<<"START";
	}
	else {
		out<<"INTERMEDIATE_OR_END";
	}
	out<<"\" ";

	if (_inputStream != BAD_STREAM_ID) {
		out<<"InputStreamName=\""<<_inputStreamName<<"\"";
	}

	out<<">"<<endl;
}


void StartNode::writeCommonPart2(ostream& out) const {
	if (!_pForwards.empty()) {
		for (ForwardEdgeSet::const_iterator it = _pForwards.begin();
			it != _pForwards.end();
			++it) {
				(*it)->write(out);
		}
	}
}


void StartNode::write(ostream& out) const {
	writeCommonPart1(out);
	writeCommonPart2(out);
	out<<"</Node>"<<endl;
}

ForwardEdge* StartNode::findSameForwardEdge(const ForwardEdge* rhs) const {
	for (ForwardEdgeSet::const_iterator edge_it = _pForwards.begin();
		edge_it != _pForwards.end(); ++edge_it) {
			if (**edge_it == *rhs) {
				return *edge_it;
			}
	}
	return NULL;
}

bool StartNode::operator ==(const StartNode& rhs) const {
	if (_inputStream != rhs._inputStream) {
		return false;
	}

	return true;
}

void StartNode::clearSwordEdges() {
	if (_pHeadSword) {
		_pHeadSword->clearSwordEdges();
		_pHeadSword = NULL;
	}
}

EdgeID StartNode::getNextForwardEdgeID() {
	return _pForwards.size();
}
