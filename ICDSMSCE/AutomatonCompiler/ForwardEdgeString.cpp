/**
 * @file ForwardEdgeString.cpp
 * @date November 27, 2005
 * @brief Implementation file of ForwardEdgeString class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "AutomatonCompiler/ForwardEdgeString.h"
#include "AutomatonCompiler/Context.h"
using namespace Cayuga::AutomatonCompiler;

#include "BasicDataStructure/Automaton/ForwardEdge.h"
#include "BasicDataStructure/Automaton/IntermediateNode.h"
using namespace Cayuga::BasicDataStructure::Automaton;

#include <cassert>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// ForwardEdgeString member function definitions
////////////////////////////////////////////////////////////////////////////////

ForwardEdgeString::ForwardEdgeString (): 
RebindEdgeString(), _destNodeID(BAD_NODE_ID) {
}

ForwardEdgeString::~ForwardEdgeString() {
}

void ForwardEdgeString::setDestNodeID(NodeID destNodeID) {
	_destNodeID = destNodeID;
}

//void addQueryID(QueryID qid);

void ForwardEdgeString::write(ostream& out) const {
	assert (_destNodeID != BAD_NODE_ID);
	out<<"<ForwardEdge Destination=\""<<_destNodeID<<"\">"<<endl;
	EdgeString::write(out);
	RebindEdgeString::_schemaMap.write(out);
	out<<"</ForwardEdge>"<<endl;
}

Edge* ForwardEdgeString::compile(Context& context) {
	assert (context._edgeType == FORWARD);
	Edge* pNewEdge = FilterEdgeString::compile(context);
	
	//TODO: have a long term solution here.
	//we do not always set del by default
	//This change is presumably made by Varun
	//since in EdgeString we already set it via EdgeBuilder,
	//should use that value here
	if (this->_hasDel)
		((RebindEdge*)pNewEdge)->setDel(_hasDel);
	
	//maintain the workset for context._patchDestNode
	hash_map<NodeID, IntermediateNode*>::iterator find_it 
		= context._nodeMap.find(_destNodeID);
	if (find_it != context._nodeMap.end()) {
		((ForwardEdge*)pNewEdge)->setDestNode(find_it->second);
	}
	else {
		context._patchDestNode.insert(make_pair(_destNodeID, 
			(ForwardEdge*)pNewEdge));
		//context._patchDestNode.insert(_destNodeID, 
		//	(ForwardEdge*)pNewEdge);
	}

	//maintain the workset for context._patchSchemaMap
	find_it = context._nodeMap.find(_destNodeID);
	if (find_it != context._nodeMap.end()) {
		IntermediateNode* pDestNode = (IntermediateNode*)find_it->second;
		Node* pSourceNode = context._pSourceNode;
		size_t schemaLen = 0;
		char* schemaMapProgram = _schemaMap.compile(pSourceNode,
			pDestNode,
			schemaLen);
		
		pDestNode->setSchemaLen(schemaLen);

		((ForwardEdge*)pNewEdge)->setSchemaMapProgram(schemaMapProgram);
	}
	else {
		context._patchSchemaMap.insert(
			make_pair(_destNodeID, make_pair( 
			make_pair((ForwardEdge*)pNewEdge, this), context._pSourceNode)));
//		EdgeEdgeStringNode* e = new EdgeEdgeStringNode(
//			(ForwardEdge*)pNewEdge, this, context._pSourceNode);
			
		//context._patchSchemaMap.insert(_destNodeID, e);
	}

	assert (typeid(*pNewEdge) == typeid(ForwardEdge));
	((ForwardEdge*)pNewEdge)->setSchemaMap(_schemaMap);

	return pNewEdge;
}
