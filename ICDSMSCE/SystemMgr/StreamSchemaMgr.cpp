/**
 * @file StreamSchemaMgr.cpp
 * @date December 8, 2005
 * @brief This implementation file of class StreamSchemaMgr.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "SystemMgr/StreamSchemaMgr.h"
#include <cassert>

using namespace std;

using namespace Cayuga::SystemMgr;

StreamID StreamSchemaMgr::insertNewSchema(const string& name, 
										  const StreamSchema& schema) {
	StreamID streamID = (StreamID)_schemaIDMgr.putValueAtEnd(name);
	assert (streamID != -1);
	pair<SchemaMap::iterator, bool> ins_it 
		= _schemas.insert(make_pair(streamID, schema));
	if (ins_it.second) {
		//insertion ok
		return streamID;
	}
	else {
		//bug: temporary fix
		//if the schema already exists, should assert it is the same as the
		//input schema
		//also, if the input schema has generating node set,
		//we assume that set has only one element. Therefore, we
		//will input only the first element in that node set into 
		//the stream schema registered in StreamSchemaMgr
		SchemaMap::iterator find_it = ins_it.first;
		//assert (find_it);
		vector<Cayuga::BasicDataStructure::Automaton::Node*> pGenNodes 
			= schema.getGeneratingNodes();
		assert (pGenNodes.size() == 1);
		find_it->second.
			setGeneratingNode(pGenNodes.front());
		return find_it->first;
		//assert (false);
		//return BAD_STREAM_ID;
	}
}

StreamID StreamSchemaMgr::getStreamID(const string& streamName) const {
	return (StreamID)_schemaIDMgr.getID(streamName);
}

string StreamSchemaMgr::getStreamName(const StreamID sid) const {
	return _schemaIDMgr.getValue(sid);
}

size_t StreamSchemaMgr::getInternalSchemaSize (const StreamID& streamID) const {
	SchemaMap::const_iterator find_it = _schemas.find(streamID);
	assert (find_it != _schemas.end());
	return find_it->second.getInternalSchemaSize();
}

const StreamSchema* 
StreamSchemaMgr::getStreamSchema (const StreamID& streamID) const {
	SchemaMap::const_iterator find_it = _schemas.find(streamID);
	assert (find_it != _schemas.end());
	return &(find_it->second);
}

const StreamSchema* 
StreamSchemaMgr::getStreamSchema (const string& streamName) const {
	StreamID sid = this->getStreamID(streamName);
	return this->getStreamSchema(sid);
}

AttributeID StreamSchemaMgr::getStreamAttrID (const StreamID& streamID, 
											  const string& attrName) const {
	SchemaMap::const_iterator find_it = _schemas.find(streamID);
	//if (find_it == _schemas.end()) {
	//	throw 1;
	//}
	assert (find_it != _schemas.end());
	//throw "cannot find stream with ID " + streamID;
	
	return find_it->second.getID(attrName);
}

AttrTypeID StreamSchemaMgr::getStreamAttrType (const StreamID& streamID, 
											   const AttributeID attrID) const {
	SchemaMap::const_iterator find_it = _schemas.find(streamID);
	assert (find_it != _schemas.end());
	return find_it->second.getAttributeType(attrID);
}

size_t StreamSchemaMgr::getInternalAttributeOffset (const StreamID& streamID, 
											   const AttributeID attrID) const {
	SchemaMap::const_iterator find_it = _schemas.find(streamID);
	assert (find_it != _schemas.end());
	return find_it->second.getInternalAttributeOffset(attrID);
}

size_t StreamSchemaMgr::getInternalAttributeSize (const StreamID& streamID, 
											 const AttributeID attrID) const {
	SchemaMap::const_iterator find_it = _schemas.find(streamID);
	assert (find_it != _schemas.end());
	return find_it->second.getInternalAttributeSize(attrID);
}

void StreamSchemaMgr::write(ostream& out) const {
	for (SchemaMap::const_iterator it = _schemas.begin(); 
		it != _schemas.end(); ++it) {
		out<<"Schema ID: "<<it->first;
		out<<", Name: "<<getStreamName(it->first)<<endl;
		it->second.write(out);
		out<<endl;
	}
}
