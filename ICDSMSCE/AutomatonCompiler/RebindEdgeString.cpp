/**
 * @file RebindEdgeString.cpp
 * @date November 27, 2005
 * @brief Implementation file of RebindEdgeString class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "AutomatonCompiler/RebindEdgeString.h"
#include "AutomatonCompiler/Context.h"
using namespace Cayuga::AutomatonCompiler;

#include "BasicDataStructure/Automaton/RebindEdge.h"
#include "BasicDataStructure/Automaton/IntermediateNode.h"
using namespace Cayuga::BasicDataStructure::Automaton;

#include <iostream>
#include <cassert>
using namespace std;


////////////////////////////////////////////////////////////////////////////////
// ForwardEdgeString member function definitions
////////////////////////////////////////////////////////////////////////////////

RebindEdgeString::RebindEdgeString () : FilterEdgeString(), _hasDel(false) {
}

RebindEdgeString::~RebindEdgeString() {
}

void RebindEdgeString::appendAttrInSchema(const OldNewAttrPair& attrPair) {
	_schemaMap.push_back(attrPair);
}

void RebindEdgeString::write(ostream& out) const {
	out<<"<RebindEdge>"<<endl;
	EdgeString::write(out);
	_schemaMap.write(out);
	out<<"</RebindEdge>"<<endl;
}

Edge* RebindEdgeString::compile(Context& context) {
	Edge* pNewEdge = FilterEdgeString::compile(context);
	
	//Set the Del attribute on the edge
	if (this->_hasDel)
		((RebindEdge*)pNewEdge)->setDel(_hasDel);

	//schema map code here
	if (!_schemaMap.empty()) {
		assert (context._pSourceNode != NULL);
		//maintain the workset
		Node* pSourceNode = context._pSourceNode;
		size_t schemaLen = 0;
		char* schemaMapProgram = _schemaMap.compile(pSourceNode,
			(IntermediateNode*)pSourceNode,
			schemaLen);

		((IntermediateNode*)pSourceNode)->setSchemaLen(schemaLen);
		((RebindEdge*)pNewEdge)->setSchemaMapProgram(schemaMapProgram);
	}

	assert (typeid(*pNewEdge) == typeid(RebindEdge));
	((RebindEdge*)pNewEdge)->setSchemaMap(_schemaMap);

	return pNewEdge;
}

void RebindEdgeString::setDel(const bool& hasDel)  {
	_hasDel = hasDel;
}

void RebindEdgeString::setSchemaMap(SchemaMapString* pSchemaMap) {
	assert (_schemaMap.empty());
	assert (!pSchemaMap->empty());
	_schemaMap = *pSchemaMap;
}

//SchemaMapString* RebindEdgeString::getSchemaMap() {
//	return &_schemaMap;
//}
