/**
 * @file IntermediateNode.cpp
 * @date November 27, 2005
 * @brief Implementation file of IntermediateNode class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "BasicDataStructure/Automaton/IntermediateNode.h"
#include "BasicDataStructure/Automaton/ForwardEdge.h"
#include "BasicDataStructure/Automaton/Instance.h"
using namespace Cayuga::BasicDataStructure::Automaton;

#include "SystemMgr/StreamSchema.h"
#include "SystemMgr/StreamSchemaMgr.h"
using namespace Cayuga::SystemMgr;

#include <cassert>
using namespace std;

IntermediateNode::IntermediateNode(const StreamID inputStream, 
								   StreamSchema* pNodeSchema) :
StartNode(inputStream), _pFilter(NULL), _pRebind(NULL),_predWindow(NULL),
_pNodeSchema(pNodeSchema) {
	//set value to false initially
	_toBeOutput = false;
	//counter = 0;
	_dynFilterPredIndex = NULL;
	_dynIndexedClause = -1;
}

IntermediateNode::~IntermediateNode() {
	delete _pFilter;
	delete _pRebind;
	delete _pNodeSchema;
	delete _predWindow;
}

void IntermediateNode::setSchemaLen(const size_t schemaLen) {
	_schemaLen = schemaLen;
}

size_t IntermediateNode::getSchemaLen() const {
	return _schemaLen;
}

StreamSchema* IntermediateNode::getNodeSchema() const {
	return _pNodeSchema;
}


void IntermediateNode::addEdge(Edge* pEdge) {
	if (pEdge->getEdgeType() == typeid(FilterEdge)) {
		assert (_pFilter == NULL);
		_pFilter = (FilterEdge*)pEdge;
	}
	else if (pEdge->getEdgeType() == typeid(RebindEdge)) {
		assert (_pRebind == NULL);
		_pRebind = (RebindEdge*)pEdge;
	}
	else {
		assert (pEdge->getEdgeType() == typeid(ForwardEdge));
		//_pForwards.push_back((ForwardEdge*)pEdge);
		StartNode::addEdge(pEdge);
	}
}

bool isSame(Edge* lhs, Edge* rhs) {
	if (lhs == NULL || rhs == NULL) {
		if (lhs != rhs)
			return false;
	}
	else if (*lhs != *rhs) {
		return false;
	}
	return true;
}

bool IntermediateNode::operator ==(const IntermediateNode& rhs) const {
	if (!(StartNode::operator ==(rhs))) {
		return false;
	}

	if (!isSame(_pFilter, rhs._pFilter)) {
		return false;
	}
	
	if (!isSame(_pRebind, rhs._pRebind)) {
		return false;
	}

	//this equality is used for merging nodes, and so their forward edges
	//are not taken into account.

	//SHORT CUT
	///@bug Assume the sequence of forward edges is the same in both
	//if (_pForwards.size() != rhs._pForwards.size()) {
	//	return false;
	//}

	//for (size_t i = 0; i < _pForwards.size(); ++i) {
	//	if (!isSame(_pForwards[i], rhs._pForwards[i])) {
	//		return false;
	//	}
	//}

	if (*_pNodeSchema != *rhs._pNodeSchema) {
		return false;
	}

	return true;
}

void IntermediateNode::_writeOutputStreams(ostream& out) const {
	for (vector<OutputStreamType>::const_iterator it 
		= _outputStreams.begin(); it != _outputStreams.end(); ++it) {
			//Checks whether the stream is to be output or not
			const OutputStreamType& thisOutputStream = *it;
			thisOutputStream.write(out);				
	}
}

void IntermediateNode::write(ostream& out) const {
	//node id, input stream and type
	StartNode::writeCommonPart1(out);

	//predicatWindow
/*	assert(_predWindow);*/
	if (_predWindow != NULL)	_predWindow->write(out);

	//node schema
	assert (_pNodeSchema);
	_pNodeSchema->write(out);

	if (_pFilter) {
		_pFilter->write(out);
	}

	if (_pRebind) {
		_pRebind->write(out);
	}

	//forward edges
	StartNode::writeCommonPart2(out);

	_writeOutputStreams(out);	

	out<<"</Node>"<<endl;
}

void IntermediateNode::addOutputStream(const StreamID outputStream,
									  const bool isOut) {
	_outputStreams.push_back(OutputStreamType(outputStream, false, isOut));
	//_streamsToBeOutput.push_back(isOut);
	//_toBeOutput |= isOut;
}

bool IntermediateNode::hasOutputStream() {
	return !_outputStreams.empty();
}

void IntermediateNode::markOutputStreamResub(const StreamID sid) {
	for (size_t i = 0; i < _outputStreams.size(); ++i) {
		if (_outputStreams[i].sid == sid) {
			_outputStreams[i].bResubscribed = true;
			return;
		}
	}
	assert (false);
}

bool IntermediateNode::hasInstance() const{
	return !_instances.empty();
}


void IntermediateNode::addNewInstance(Instance* pNewInstance) {
	_newInstances.push_back(pNewInstance);
}


void IntermediateNode::copyHandles(CY_Heap::GHandle* buf, 
								   Instance* pCurInstance) {
	size_t nBytes = getSchemaLen();
	memcpy(buf, pCurInstance->getBody(), nBytes);
}

size_t IntermediateNode::getHandleBytes() const {
	return getSchemaLen();
}

//bool IntermediateNode::toBeOutput() const {
//	return _toBeOutput;
//}

//void IntermediateNode::updateCounter() {
//	counter += _instances.size();
//}

//void IntermediateNode::incCounter() {
//	counter++;
//}
//
//size_t IntermediateNode::getCounter() const {
//	return counter;
//}

void IntermediateNode::setDynIndex(DynHashIndexType* dynFilterPredIndex,
					_HashDynPredIndexSpec dynIndexSpec)
{
	_dynFilterPredIndex = dynFilterPredIndex;
	_dynIndexSpec = dynIndexSpec;
}

bool IntermediateNode::hasDynFilterIndex() const {
	return (_dynFilterPredIndex != NULL);
}

void IntermediateNode::setDynIndexedClause(int dynIndexedClause) {
	_dynIndexedClause = dynIndexedClause;
}

bool IntermediateNode::noDynIndexedClause() const {
	return (_dynIndexedClause == -1);
}

IntermediateNode::DynHashIndexType* 
IntermediateNode::getDynFilterPredIndex() const {
	return _dynFilterPredIndex;
}

vector<pair<AttributeID, AttributeID> > IntermediateNode::getDynIndexSpec()
const {
	return _dynIndexSpec;
}

bool IntermediateNode::isClauseDynIndexed(size_t clauseno) const {
	return ((int)clauseno == _dynIndexedClause);
}

void IntermediateNode::setPredWindow(predWindow * ptr)
{
    _predWindow = ptr;
}
predWindow * IntermediateNode::returnPredWindow()
{
  return _predWindow;
}