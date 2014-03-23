/**
 * @file NodeString.cpp
 * @date November 27, 2005
 * @brief Implementation file of NodeString class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "AutomatonCompiler/NodeString.h"

#include <iostream>
#include <vector>
#include <string>
#include <cassert>
using namespace std;

using namespace Cayuga::AutomatonCompiler;

NodeString::NodeString(const string& nodeType) 
: _nodeType(nodeType), _isDynIndexed(false) {
		_predwind = NULL;
}

NodeString::~NodeString() {
}

Node* NodeString::compile(Context& context) {
	assert (false);
	return NULL;
}

void NodeString::addSchemaAttr(const string& attrName,
							   const string& attrType) {
	assert (false);
}

void NodeString::addOutputStream(const string& outputStream, 
								const bool isOut) {
	assert (false);
}

string NodeString::getNodeType() {
	return _nodeType;
}

NodeID NodeString::getNodeID() {
	return _nodeID;
}


void NodeString::addPredIndexSpec(const PredIndexSpecSet& filterPredIndices,
								  const PredIndexSpecSet& frPredIndices,
								const PredIndexSpecString& dynFilterPredIndex,
								const bool& isDynIndexed) {
	_filterPredIndices = filterPredIndices;
	_frPredIndices = frPredIndices;
	_dynFilterPredIndex = dynFilterPredIndex;
	_isDynIndexed = isDynIndexed;
}
