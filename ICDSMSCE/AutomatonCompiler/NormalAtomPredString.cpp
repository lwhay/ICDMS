/**
 * @file NormalAtomPredString.cpp
 * @date April 22, 2006
 * @brief Implementation file of NormalAtomPredString.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */
#include "AutomatonCompiler/Context.h"
#include "AutomatonCompiler/NormalAtomPredString.h"	
using namespace Cayuga::AutomatonCompiler;

using namespace Cayuga::BasicDataStructure::ByteCodeNS;

#include "SystemMgr/DataTypeMgr.h"
#include "SystemMgr/StreamSchemaMgr.h"
using namespace Cayuga::SystemMgr;

NormalAtomPredString::NormalAtomPredString(const string& attr, 
										   const string& op) :
	AtomPredString(op), _eventAttr(attr) {
}

NormalAtomPredString::~NormalAtomPredString() {
}

void NormalAtomPredString::write(ostream& out) {
	out<<" EventAttr=\""<<_eventAttr<<"\" ";
	//out<<"OP=\""<<_op<<"\"";
	AtomPredString::write(out);
}


char* NormalAtomPredString::allocatePredInstruction(Context& context) {
	StreamSchemaMgr* pStreamSchemaMgr = StreamSchemaMgr::getInstance();
	AttributeID attrID = pStreamSchemaMgr->
		getStreamAttrID(context._streamID, NormalAtomPredString::_eventAttr);

	AttrTypeID attrType = pStreamSchemaMgr->
		getStreamAttrType(context._streamID, attrID);

	string attrTypeName = DataTypeMgr::getInstance()->getName(attrType);

	//get the size of rhs, to allocate memory for the predicate instruction
	size_t size_rhs = DataTypeMgr::getInstance()->
		getInternalLength(attrTypeName);

	context._predInstructionLength = 
		sizeof(ByteCode)*6 + sizeof(size_t) + size_rhs;
	char* instruction = new char[context._predInstructionLength];
	
	return instruction;
}

const string NormalAtomPredString::getEventAttr() const {
	return _eventAttr;
}

const string NormalAtomPredString::getStringOp() const {
	return _op;
}

bool NormalAtomPredString::canBeIndexed() {
	return true;
}
