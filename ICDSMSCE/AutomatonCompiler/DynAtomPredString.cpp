/**
 * @file DynAtomPredString.cpp
 * @date April 22, 2006
 * @brief Implementation file of DynAtomPredString.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */
#include "AutomatonCompiler/ByteCodeUtils.h"
#include "AutomatonCompiler/Context.h"
#include "AutomatonCompiler/ComplexAtomPredString.h"	
#include "AutomatonCompiler/DynAtomPredString.h"	
#include "AutomatonCompiler/SchemaMapString.h"	
using namespace Cayuga::AutomatonCompiler;

using namespace Cayuga::BasicDataStructure::ByteCodeNS;

#include "SystemMgr/DataTypeMgr.h"
#include "SystemMgr/StreamSchemaMgr.h"
using namespace Cayuga::SystemMgr;

#include "BasicDataStructure/Automaton/IntermediateNode.h"
using namespace Cayuga::BasicDataStructure::Automaton;

#include "Utils/RAssert.h"
using namespace Cayuga::Utils;

DynAtomPredString::DynAtomPredString(const string& attr, 
									 const string& op, 
									 const string& boundAttr) :
	NormalAtomPredString(attr, op), _boundAttr(boundAttr) {
}

DynAtomPredString::~DynAtomPredString() {
}

void DynAtomPredString::write(ostream& out) {
	out<<"<DynamicPred";
	NormalAtomPredString::write(out);
	out<<"NodeAttr=\""<<_boundAttr<<"\"";
	out<<"/>"<<endl;
}

char* DynAtomPredString::compile(Context& context) {
	char* instruction = allocatePredInstruction(context);

	StreamSchemaMgr* pStreamSchemaMgr = StreamSchemaMgr::getInstance();
	AttributeID attrID = pStreamSchemaMgr->
		getStreamAttrID(context._streamID, NormalAtomPredString::_eventAttr);
	const size_t attrOffset = 
		pStreamSchemaMgr->getInternalAttributeOffset(context._streamID, attrID);
		
	AttrTypeID typeID 
		= pStreamSchemaMgr->getStreamAttrType(context._streamID, attrID);

	const string typeName = DataTypeMgr::getInstance()->getName(typeID);
	
	ByteCode opType = getOp();

	size_t ip = 0;
	//SHORT CUT: Here we assume the type of the RHS contant is the same as that
	//of the event attribute, and therefore the type of comparison operator 
	//depends on the latter only
	ByteCode dataType = convertDataTypeNameToByteCode(typeName);

	//LHS: event attribute
	makeAttrInstruction(instruction, ip, EVENT, dataType, attrOffset);

	//RHS: node attribute
	assert (context._pSourceNode->getNodeType() != typeid(StartNode));
	AttributeID nodeAttrID = ((IntermediateNode*)context._pSourceNode)->
		getNodeSchema()->getID(_boundAttr);
	size_t nodeAttrOffset = ((IntermediateNode*)context._pSourceNode)->
		getNodeSchema()->getInternalAttributeOffset(nodeAttrID);
	makeAttrInstruction(instruction, ip, NODE, dataType, nodeAttrOffset);

	//operator type
	makeOpInstruction(instruction, ip, opType, dataType);
	return instruction;
}

string DynAtomPredString::getNodeAttr() const {
	return _boundAttr;
}


AtomPredString* DynAtomPredString::clone() {
	return new DynAtomPredString(_eventAttr, 
		_op, 
		_boundAttr);
}

void DynAtomPredString::getReferredAttributes(
	set<string>& referredAttrs) const {
	referredAttrs.insert(_eventAttr);
	referredAttrs.insert(_boundAttr);
}

AtomPredString* DynAtomPredString::renameAttrRefInPredsToAttrIn(
	const SchemaMapString* schemaMap) {
	bool bFromEvent;
	bool success = schemaMap->renameAttrRefAndSource(
		_eventAttr, _eventAttr, bFromEvent);
	if (!bFromEvent) {
		//problem... must create a non sta predicate now
		//Assert (false);
		return new ComplexAtomPredString("NODE."+_eventAttr+_op+_boundAttr);
	}

	string newBoundAttr;
	success = schemaMap->renameAttrRefAndSource(
		_boundAttr, newBoundAttr, bFromEvent);
	//the schema map will only affect attributes coming from the event stream
	Assert (success == false);

	return NULL;

	//const OldNewAttrPair* evAttrPair 
	//	= schemaMap->findDestAttrOrNULL(_eventAttr);
	////the attribute cannot be created from UDF
	//if (evAttrPair) {
	//	Assert (!evAttrPair->_bIsAggregate);
	//	_eventAttr = evAttrPair->_oldAttr;
	//}

	//const OldNewAttrPair* nodeAttrPair 
	//	= schemaMap->findDestAttrOrNULL(_boundAttr);
	////the attribute cannot be created from UDF
	//if (nodeAttrPair) {
	//	Assert (!nodeAttrPair->_bIsAggregate);
	//	_boundAttr = nodeAttrPair->_oldAttr;
	//}
}
