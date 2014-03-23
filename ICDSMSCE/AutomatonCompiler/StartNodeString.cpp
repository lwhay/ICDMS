/**
 * @file StartNodeString.cpp
 * @date November 27, 2005
 * @brief Implementation file of StartNodeString class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "AutomatonCompiler/StartNodeString.h"
#include "AutomatonCompiler/ForwardEdgeString.h"
#include "AutomatonCompiler/Context.h"
using namespace Cayuga::AutomatonCompiler;

#include "BasicDataStructure/Automaton/IntermediateNode.h"
//#include "BasicDataStructure/Automaton/ForwardEdge.h"
using namespace Cayuga::BasicDataStructure::Automaton;

#include "SystemMgr/StreamSchemaMgr.h"
using namespace Cayuga::SystemMgr;

#include "Utils/AttrMap.h"
using namespace Cayuga::Utils;

////////////////////////////////////////////////////////////////////////////////
// Foward declaration
////////////////////////////////////////////////////////////////////////////////

namespace Cayuga {
namespace QueryProcessor {
void insertIndexSpecs(const Context& context,
					  const NodeString::PredIndexSpecSet& filterPredIndices, 
					  const NodeString::PredIndexSpecSet& frPredIndices);
void insertDynIndexSpecs(const Context& context,
				const PredIndexSpecString& dynFilterPredIndex);
}
}

using namespace Cayuga::QueryProcessor;


////////////////////////////////////////////////////////////////////////////////
// StartNodeString member function definitions
////////////////////////////////////////////////////////////////////////////////

StartNodeString::StartNodeString(const StringMap& attrMap) 
: NodeString("START"), _pFilter(NULL), _pRebind(NULL) {
	_nodeID = getParameterValue<NodeID>(attrMap, "NodeID");
	_inputStream = getParameterValue<string>(attrMap, "InputStreamName", "NULL");
}

StartNodeString::~StartNodeString() {
}

string StartNodeString::getInputStreamName() {
	return _inputStream;
}

void StartNodeString::setInputStreamName(const string& input) {
	_inputStream = input;
}

void StartNodeString::writeCommonBeforeSchema(ostream& out) {
	out<<"<Node NodeID=\""<<_nodeID<<"\" ";
	out<<"NodeType=\""<<_nodeType<<"\" ";

	if (_inputStream != "NULL") {
		out<<"InputStreamName=\""<<_inputStream<<"\"";
	}

	out<<">"<<endl;
}

void StartNodeString::writeCommonAfterSchema(ostream& out) {
	if (_pFilter) {
		_pFilter->write(out);
	}

	if (!_filterPredIndices.empty()) {
		for (PredIndexSpecSet::iterator index_it = _filterPredIndices.begin();
			index_it != _filterPredIndices.end();
			++index_it) {
				out<<"<FilterPredIndex>"<<endl;
				index_it->write(out);
				out<<"</FilterPredIndex>"<<endl;
		}
	}

	if (_pRebind) {
		_pRebind->write(out);
	}

	if (!_pForwards.empty()) {
		for (ForwardEdgeSet::iterator it = _pForwards.begin();
			it != _pForwards.end();
			++it) {
				ForwardEdgeString* thisEdge = *it;
				thisEdge->write(out);
		}
	}

	if (!_frPredIndices.empty()) {
		for (PredIndexSpecSet::iterator index_it = _frPredIndices.begin();
			index_it != _frPredIndices.end();
			++index_it) {
				out<<"<FRPredIndex>"<<endl;
				index_it->write(out);
				out<<"</FRPredIndex>"<<endl;
		}
	}
}

void StartNodeString::write(ostream& out) {
	writeCommonBeforeSchema(out);
	writeCommonAfterSchema(out);
	out<<"</Node>"<<endl;
}

void StartNodeString::addEdge(EdgeString* pEdge) {
	if (typeid(*pEdge) == typeid(FilterEdgeString)) {
		assert (_pFilter == NULL);
		_pFilter = (FilterEdgeString*)pEdge;
	}
	else if (typeid(*pEdge) == typeid(RebindEdgeString)) {
		assert (_pRebind == NULL);
		_pRebind = (RebindEdgeString*)pEdge;
	}
	else {
		assert (typeid(*pEdge) == typeid(ForwardEdgeString));
		_pForwards.push_back((ForwardEdgeString*)pEdge);
	}
}

Node* StartNodeString::makeNode(const StreamID sid) {
	return new StartNode(sid);
}

Node* StartNodeString::compile(Context& context) {
	StreamSchemaMgr* pStreamSchemaMgr = StreamSchemaMgr::getInstance();
	StreamID sid;
	
	if (_inputStream == "NULL") {
		sid = BAD_STREAM_ID;
	}
	else {
		sid = pStreamSchemaMgr->getStreamID(_inputStream);
		vector<Node*> pGenNodes
			= pStreamSchemaMgr->getStreamSchema(sid)->getGeneratingNodes();
		for (vector<Node*>::iterator it = pGenNodes.begin(); 
			it != pGenNodes.end(); ++it) {
			Node* pGenNode = *it;
			assert (typeid(*pGenNode) == typeid(IntermediateNode));
			((IntermediateNode*)pGenNode)->markOutputStreamResub(sid);
		}
	}

	Node* pInternalNode = makeNode(sid); 
	pInternalNode->setInputStreamName(_inputStream);

	NodeID nodeID = _nodeID;
	//add a new entry to _nodeMap
	if (pInternalNode->getNodeType() != typeid(StartNode)) {
		context._nodeMap.insert(make_pair(nodeID, 
			(IntermediateNode*)pInternalNode));
	}

	context._streamID = sid;
	context._pSourceNode = pInternalNode;

	//insert index specs
	insertIndexSpecs(context, _filterPredIndices, _frPredIndices);

	//insert Dynamic Index Specs for the context
	context._isDynIndexed = _isDynIndexed;
	if(_isDynIndexed) {
		context._dynFilterPredIndex = _dynFilterPredIndex;
		insertDynIndexSpecs(context, _dynFilterPredIndex);
	}
	
	context._predIndices = _filterPredIndices;
	if (typeid(*this) == typeid(StartNodeString) ||
		this->_inputStream == "NULL") {
		assert (!_pFilter);
	}
	else {
		if (!_pFilter) {
			_pFilter = new FilterEdgeString();
		}
		context._edgeType = FILTER;
		pInternalNode->addEdge(_pFilter->compile(context));
	}

	context._predIndices = _frPredIndices;
	if (_pRebind) {
		context._edgeType = REBIND;
		pInternalNode->addEdge(_pRebind->compile(context));
	}

	context._edgeType = FORWARD;
	for (ForwardEdgeSet::iterator edge_it = _pForwards.begin();
		edge_it != _pForwards.end();
		++edge_it) {
			context._nextForwardEdgeID = 
				((StartNode*)pInternalNode)->getNextForwardEdgeID();
			
			Edge* pInternalEdge = (*edge_it)->compile(context);
			pInternalNode->addEdge(pInternalEdge);
	}
	return pInternalNode;
}
