/**
 * @file CMessage.cpp
 * @date April 3, 2007
 * @brief Implementation of a Cayuga message.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "BasicDataStructure/CMessage.h"
using namespace Cayuga::BasicDataStructure;

CMessage::CMessage(const string& msgType, const char delim) 
: _msgType(msgType), _delim(delim) {
}

void CMessage::appendField(const string& field) {
	_fields.push_back(field);
}

void CMessage::write(ostream& out) const {
	out<<_fields.size()+1; //+1 to include the message type
	out<<_delim;

	out<<_msgType; 
	out<<_delim;

	for (vector<string>::const_iterator it = _fields.begin();
		it != _fields.end(); ++it) {
			out<<*it;
			out<<_delim;
	}
	out<<endl;
}
