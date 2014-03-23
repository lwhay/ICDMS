/**
 * @file Node.cpp
 * @date November 27, 2005
 * @brief Implementation file of Node class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "BasicDataStructure/Automaton/node.h"
using namespace Cayuga::BasicDataStructure::Automaton;

#include "Index/HashIndex.h"
using Cayuga::Index::HashIndex;


#include <iostream>
#include <vector>
#include <string>
#include <cassert>
using namespace std;

NodeID Node::_nodeIDCnt = 0;

vector<Node*> Node::_nodes;

Node::Node(const StreamID inputStream) 
  : _nodeID(_nodeIDCnt++), _inputStream(inputStream) {
	_nodes.push_back(this);
}

Node::~Node() {
}

const type_info& Node::getNodeType() {
	return typeid(*this);
}

StreamID Node::getInputStream() {
	return _inputStream;
}

void Node::addIndexSpec(HashIndexType* pIndex) {
	_index.insert(make_pair(pIndex->getID(), pIndex));
}

NodeID Node::getID() const {
	return _nodeID;
}

void Node::setInputStreamName(const string& streamName) {
	_inputStreamName = streamName;
}
