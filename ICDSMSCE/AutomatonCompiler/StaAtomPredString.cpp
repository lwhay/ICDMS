/**
 * @file StaAtomPredString.cpp
 * @date November 27, 2005
 * @brief Implementation file of StaAtomPredString class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "AutomatonCompiler/StaAtomPredString.h"	
#include "AutomatonCompiler/ByteCodeUtils.h"
#include "AutomatonCompiler/ComplexAtomPredString.h"	
#include "AutomatonCompiler/convertConstToByteArray.h"
#include "AutomatonCompiler/Context.h"
#include "AutomatonCompiler/SchemaMapString.h"	
using namespace Cayuga::AutomatonCompiler;

#include "BasicDataStructure/ByteCode.h"
using namespace Cayuga::BasicDataStructure::ByteCodeNS;

#include "SystemMgr/DataTypeMgr.h"
#include "SystemMgr/StreamSchemaMgr.h"
using namespace Cayuga::SystemMgr;

#include "Utils/RAssert.h"
using namespace Cayuga::Utils;

StaAtomPredString::StaAtomPredString(const string& attr, 
									 const string& op, 
									 const string& nConst) :
NormalAtomPredString(attr, op), _rhs(nConst) {
}


void StaAtomPredString::write(ostream& out) {
	out<<"<StaticPred";
	NormalAtomPredString::write(out);
	out<<"Const=\""<<_rhs<<"\"";
	out<<"/>"<<endl;
}

StaAtomPredString::~StaAtomPredString() {
}

string StaAtomPredString::getConst() {
	return _rhs;
}

char* StaAtomPredString::compile(Context& context) {

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

	//RHS: constant
	setValToProgram<ByteCode>(instruction, ip, CONSTANT);
	setValToProgram<ByteCode>(instruction, ip, dataType);
	convertConstToByteArray(typeName,
		_rhs, 
		pStreamSchemaMgr->getInternalAttributeSize(context._streamID, attrID), 
		instruction+ip,
		true,		//ping string constants from queries/predicates
		true		//put strings into external space
		);
	ip += pStreamSchemaMgr->getInternalAttributeSize(context._streamID, attrID);
	
	//operator type
	makeOpInstruction(instruction, ip, opType, dataType);
	return instruction;
}


AtomPredString* StaAtomPredString::clone() {
	return new StaAtomPredString(_eventAttr, 
		_op, 
		_rhs);
}

void StaAtomPredString::getReferredAttributes(
	set<string>& referredAttrs) const {
	referredAttrs.insert(_eventAttr);
}

AtomPredString* StaAtomPredString::renameAttrRefInPredsToAttrIn(
	const SchemaMapString* schemaMap) {
	bool bFromEvent;
	schemaMap->renameAttrRefAndSource(_eventAttr, _eventAttr, bFromEvent);
	if (!bFromEvent) {
		//problem... must create a non sta predicate now
		//Assert (false);
		return new ComplexAtomPredString("NODE."+_eventAttr+_op+_rhs);
	}
	return NULL;
}
