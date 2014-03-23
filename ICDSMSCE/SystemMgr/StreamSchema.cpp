/**
 * @file StreamSchema.cpp
 * @date December 8, 2005
 * @brief This implementation file of class StreamSchema.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "SystemMgr/DataTypeMgr.h"
#include "SystemMgr/StreamSchema.h"
#include "SystemMgr/StreamSchemaMgr.h"
#include "SystemMgr/Options.h"
using namespace Cayuga::SystemMgr;

#include "MemoryMgr/CY_HeapFace.h"
#include "MemoryMgr/CY_IntStringFace.h"
using namespace CY_IntString;

#include "BasicDataStructure/Event/Event.h"
using namespace Cayuga::BasicDataStructure::EventNS;

#include "BasicDataStructure/OutputStreamType.h"
using Cayuga::BasicDataStructure::OutputStreamType;

#include "BasicDataStructure/CMessage.h"
using Cayuga::BasicDataStructure::CMessage;

#include "SAXHandler/SAXEventType.h"
#include "SAXHandler/ConfigMgr.h"
using namespace Cayuga::SAXHandler;

#include <cassert>
#include <sstream>
using namespace std;

StreamSchema::StreamSchema() {
	_dataTypeMgr = DataTypeMgr::getInstance();
}

StreamSchema::~StreamSchema() {
}

void StreamSchema::putAttributeAtEnd(const string& attrName, 
									 const string& typeName) {
	putValueAtEnd(attrName);

	const size_t typeID = _dataTypeMgr->getID(typeName);
	_types.push_back(typeID);
}

void StreamSchema::setGeneratingNode(Node* pNode) {
	_pGeneratingNodes.push_back(pNode);
}

vector<Node*> StreamSchema::getGeneratingNodes() const {
	return _pGeneratingNodes;
}

AttrTypeID StreamSchema::getAttributeType(const AttributeID attrID) const {
	assert (attrID >=0 && attrID < _types.size());
	return _types[attrID];
}

size_t 
StreamSchema::getInternalAttributeOffset(const AttributeID attrID) const {
	assert (attrID >=0 && attrID < _types.size());
	size_t offset = 0;
	for (size_t i = 0; i < attrID; i++) {
		offset += _dataTypeMgr->getInternalLength(_types[i]);
	}
	return offset;
}

size_t StreamSchema::getInternalAttributeSize(const AttributeID attrID) const {
	assert (attrID >=0 && attrID < _types.size());
	return _dataTypeMgr->getInternalLength(_types[attrID]);
}

size_t StreamSchema::getNumAttributes() const {
	return _values.size();
}

size_t StreamSchema::getInternalSchemaSize() const {
	size_t len = 0;
	for (size_t i = 0; i < _values.size(); ++i) {
		len += getInternalAttributeSize(i);
	}
	return len;
}

void StreamSchema::write(ostream& out) const {
	assert (_values.size() == _types.size());
	if (!_values.empty()) {
		out<<"<Schema>"<<endl;
		for (size_t attr_idx = 0; attr_idx < _values.size(); ++attr_idx) {
			out<<"<AttrName Name=\""<<_values[attr_idx]<<"\"";

			AttrTypeID attrTypeID = _types[attr_idx];
			string typeName = _dataTypeMgr->getName(attrTypeID);
			out<<" Type=\""<<typeName<<"\"";
			out<<"/>"<<endl;
		}
		out<<"</Schema>"<<endl;
	}
}


void StreamSchema::_displayTypedAttributeValue(const string& attrTypeName, 
								const char* ev, 
								size_t offset, 
								ostream& out) const {
	if (attrTypeName == "int") {
		out<<*(int*)(ev+offset);
	}
	else if (attrTypeName == "float") {
		out<<*(float*)(ev+offset);
	}
	else if (attrTypeName == "shortstring") {
		out<<ev[offset];
		out<<ev[offset+1];
		out<<ev[offset+2];
		out<<ev[offset+3];
	}
	else if (attrTypeName == "SAXEventType") {
		out<<*(SAXEventType*)(ev+offset);
	}
	else if (attrTypeName == "string") {
		IntStringHandle h = *(IntStringHandle*)(ev+offset);
		const char* str = h->getBody();
		out<<str;
	}
	else {
		assert(false);
	}
}

void StreamSchema::printEvent(const Event* ev, ostream& out) const {
	for (ValueSequence::const_iterator it = _values.begin();
		it != _values.end();
		++it) {
			out<<*it<<"\t";
	}
	out<<"t0\t"<<"t1";
	out<<endl;

	const char* evBody = ev->getBody();
	size_t offset = 0;
	for (TypeSequence::const_iterator it = _types.begin();
		it != _types.end(); ++it) {
			size_t attrLen = _dataTypeMgr->getInternalLength(*it);
			_displayTypedAttributeValue(
				_dataTypeMgr->getName(*it),
				evBody, 
				offset, 
				//attrLen,
				out);
			out<<"\t";

			offset += attrLen;
	}
	out<<ev->getStartTime()<<"\t"<<ev->getEndTime();
	out<<endl;
}

void StreamSchema::printEventInFileERFormat(
	const Event* ev, 
	const vector<OutputStreamType>& outputStreams,
	ostream& out) const {
	const char* evBody = ev->getBody();

	char delim = ConfigMgr::getInstance()->getOptions()->_attrDelimiter;
	for (vector<OutputStreamType>::const_iterator sit 
		= outputStreams.begin(); sit != outputStreams.end(); ++sit) {
		if (!sit->bOutput) continue; 
		size_t offset = 0;
		for (TypeSequence::const_iterator it = _types.begin();
			it != _types.end(); ++it) {
				size_t attrLen = _dataTypeMgr->getInternalLength(*it);
				_displayTypedAttributeValue(
					_dataTypeMgr->getName(*it),
					evBody, 
					offset, 
					//attrLen,
					out);
				out<<delim;

				offset += attrLen;
		}
		StreamID sid = (*sit).sid;
		string streamName = StreamSchemaMgr::getInstance()->getStreamName(sid);
		out<<streamName<<delim;
		out<<ev->getStartTime()<<delim<<ev->getEndTime();
		out<<endl;
	}
}


void StreamSchema::genEventSchemaContentForTraceMsg(const Event* ev, 
								 CMessage& msgOut) const {
	const char* evBody = ev->getBody();
	size_t offset = 0;
	for (size_t i = 0; i < _values.size(); ++i) {
		msgOut.appendField(_values[i]);
		size_t attrLen = _dataTypeMgr->getInternalLength(_types[i]);
		ostringstream ostr;
		_displayTypedAttributeValue(
			_dataTypeMgr->getName(_types[i]),
			evBody, 
			offset, 
			//attrLen,
			ostr);
		msgOut.appendField(ostr.str());
		offset += attrLen;
	}
}

bool StreamSchema::operator == (const StreamSchema& rhs) const {
	if (ValueIDMap<string>::operator !=(rhs)) {
		return false;
	}

	if (_types != rhs._types) {
		return false;
	}

	return true;
}

bool StreamSchema::operator != (const StreamSchema& rhs) const {
	return !(*this == rhs);
}

void StreamSchema::clear() {
	ValueIDMap<string>::clear();
	_types.clear();
}
