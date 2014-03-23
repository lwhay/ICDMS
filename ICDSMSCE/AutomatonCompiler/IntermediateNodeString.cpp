/**
 * @file IntermediateNodeString.cpp
 * @date November 27, 2005
 * @brief Implementation file of IntermediateNodeString class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "AutomatonCompiler/IntermediateNodeString.h"
#include "AutomatonCompiler/ForwardEdgeString.h"
#include "AutomatonCompiler/Context.h"
using namespace Cayuga::AutomatonCompiler;

#include "BasicDataStructure/Automaton/NodeType.h"
#include "BasicDataStructure/Automaton/IntermediateNode.h"
#include "BasicDataStructure/Automaton/NodeID.h"
#include "BasicDataStructure/Automaton/ForwardEdge.h"
#include "BasicDataStructure/StreamID.h"
using namespace Cayuga::BasicDataStructure::Automaton;

#include "SystemMgr/StreamSchema.h"
#include "SystemMgr/StreamSchemaMgr.h"
using namespace Cayuga::SystemMgr;

#include "Utils/AttrMap.h"
#include "Utils/RAssert.h"
using namespace Cayuga::Utils;

#include <cassert>
using namespace std;


////////////////////////////////////////////////////////////////////////////////
// IntermediateNodeString member function definitions
////////////////////////////////////////////////////////////////////////////////

IntermediateNodeString::IntermediateNodeString(const StringMap& attrMap) :
StartNodeString(attrMap)  {
	NodeString::_nodeType = "INTERMEDIATE_OR_END";
	_pNodeSchema = new StreamSchema();
	_predWindow = NULL;
}

IntermediateNodeString::~IntermediateNodeString() {
	delete _pNodeSchema;
}

void IntermediateNodeString::write(ostream& out) {
	//luoluo 7-31
//	writeCommonBeforeSchema(out);
	_pNodeSchema->write(out);
	writeCommonAfterSchema(out);

	if (!_outputStreams.empty()) {
		for (vector<pair<string, bool> >::iterator it = _outputStreams.begin();
			it != _outputStreams.end();
			++it) {
				string streamType = (it-> second) ? "OUT" : "RESUB";
				out<<"<OutputStreamName Name=\""<<it-> first<<"\" Type=\""<<
					streamType <<"\"/>"<<endl;
		}
	}
	out<<"</Node>"<<endl;

}

StreamSchema* IntermediateNodeString::getNodeSchema() const {
	return _pNodeSchema;
}

void IntermediateNodeString::setNodeSchema(StreamSchema* pSchema) {
	delete _pNodeSchema;
	_pNodeSchema = pSchema;
}

void IntermediateNodeString::addSchemaAttr(const string& attrName,
										   const string& attrType) {
	_pNodeSchema->putAttributeAtEnd(attrName, attrType);
}


Node* IntermediateNodeString::makeNode(const StreamID sid) {
	Node* pNode = new IntermediateNode(sid, _pNodeSchema);
	_pNodeSchema = NULL; 
	return pNode;
}

Node* IntermediateNodeString::compile(Context& context) {
	IntermediateNode* pInternalNode = (IntermediateNode*)
		StartNodeString::compile(context);

	//maintain the workset context._patchDestNode
	NodeID nodeID = StartNodeString::_nodeID;

	hash_map<NodeID, ForwardEdge*>::iterator find_pdn_it = 
		context._patchDestNode.find(nodeID);
	if (find_pdn_it != context._patchDestNode.end()) {
		ForwardEdge* pOldForwardEdge = find_pdn_it->second;
		pOldForwardEdge->setDestNode((IntermediateNode*)pInternalNode);

		//Mingsheng 06.09.26 shouldn't erase this entry since there may be
		//multiple nodes that point to this destination node
		context._patchDestNode.erase(find_pdn_it);
	}

	//maintain the workset for context._patchSchemaMap
	hash_map<NodeID, pair<pair<ForwardEdge*, ForwardEdgeString*>, Node*> >
		::iterator find_psm_it = context._patchSchemaMap.find(nodeID);
	if (find_psm_it != context._patchSchemaMap.end()) {
		//great! set the schema map of pNewEdge right here
		Node* pSourceNode = find_psm_it->second.second;
		size_t schemaLen = 0;
		char* schemaMapProgram = 
			find_psm_it->second.first.second->_schemaMap.compile(pSourceNode,
			pInternalNode,
			schemaLen);

		pInternalNode->setSchemaLen(schemaLen);
		((ForwardEdge*)find_psm_it->second.first.first)->
			setSchemaMapProgram(schemaMapProgram);

		//Mingsheng 06.09.26 shouldn't erase this entry since there may be
		//multiple nodes that point to this destination node
		context._patchSchemaMap.erase(find_psm_it);
	}

	//manage _outputStreams
	pInternalNode->getNodeSchema()->setGeneratingNode(pInternalNode);
	for (vector<pair<string, bool> >::iterator it = _outputStreams.begin();
		it != _outputStreams.end(); ++it) {
			StreamID outputStreamID = StreamSchemaMgr::getInstance()->
				insertNewSchema(it->first, *pInternalNode->getNodeSchema());
			pInternalNode->addOutputStream(outputStreamID, it-> second);
	}
	return pInternalNode;
}

void IntermediateNodeString::addOutputStream(const string& outputStream, 
											const bool isOut) {
	_outputStreams.push_back(pair<string, bool>(outputStream, isOut));
}


void IntermediateNodeString::clearOutputStreams() {
	_outputStreams.clear();
}

const pair<string, bool> IntermediateNodeString::getOutputStream() {
	Assert (_outputStreams.size() == 1);
	return _outputStreams.front();
}

void IntermediateNodeString::setRationalPredWindow(predWindowString * ptr)
{
	//the predwindow is not null.
/*	Assert(*ptr);*/
	_predWindow = ptr;
}

predWindowString* IntermediateNodeString ::returnPredWindowString()
{
	return _predWindow;
}

// predWindow *  IntermediateNodeString::predwindow_compile()
// {
// 	assert(_predWindow != NULL);
// 	predWindow * expr = _predWindow->recompile();
// 	return expr;
// 	
// }

bool IntermediateNodeString::whether_endNode()
{
	//false means the intermediate node.
	//true  means the end node.
	if (_outputStreams.empty())return false;
	else return true;
	
	
}
