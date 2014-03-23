/**
 * @file DurationPredString.cpp
 * @date April 22, 2006
 * @brief Implementation file of DurationPredString.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "AutomatonCompiler/DurationPredString.h"	
using namespace Cayuga::AutomatonCompiler;

#include "BasicDataStructure/Predicate/DurationStruct.h"
using namespace Cayuga::BasicDataStructure::Predicate;

#include "Utils/AttrMap.h"	
using namespace Cayuga::Utils;

DurationPredString::DurationPredString(const string& type,
									   const string& op, 
									   const string& rhs) :
AtomPredString(op), _type(type), _rhs(rhs) {
	assert (_type == "EVENT" || _type == "COMPOSITE");
}

DurationPredString::~DurationPredString() {
}

void DurationPredString::write(ostream& out) {
	out<<"<DurationPred ";
	out<<"Type=\""<<_type<<"\" ";
	//out<<"OP=\""<<_op<<"\"";
	AtomPredString::write(out);
	out<<"Const=\""<<_rhs<<"\"/>"<<endl;
}

char* DurationPredString::compile(Context& context) {
	return NULL;
}

DurationStruct* DurationPredString::createDurationStruct() {
	DurationStruct* pDurStruct = new DurationStruct();
	pDurStruct->_op = getOp();
	pDurStruct->_rhs = convert<unsigned>(_rhs);
	return pDurStruct;
}

AtomPredString* DurationPredString::clone() {
	return new DurationPredString(_type, 
		_op, 
		_rhs);
}

void DurationPredString::getReferredAttributes(
	set<string>& referredAttrs) const {
}

AtomPredString* DurationPredString::renameAttrRefInPredsToAttrIn(
	const SchemaMapString* schemaMap) {
	return NULL;
}
