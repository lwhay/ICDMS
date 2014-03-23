/**
 * @file StreamSchemaMgr.h
 * @date December 8, 2005
 * @brief This file defines the stream schema manager, which manages the schemas
	      of all streams registered in the system.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _STREAM_SCHEMA_MGR_H
#define _STREAM_SCHEMA_MGR_H

#include "StreamSchema.h"
#include "BasicDataStructure/StreamID.h"

#include "Utils/Singleton.h"

#include "BasicDataStructure/Predicate/AttributeID.h"

namespace Cayuga {
using Cayuga::BasicDataStructure::Predicate::AttributeID;
using Cayuga::Utils::Singleton;
namespace SystemMgr {

class StreamSchemaMgr;
/**
 * @brief A StreamSchemaMgr object manages the schemas
	      of all streams registered in the system.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date December 8, 2005
 * @version 1.0
 */
class StreamSchemaMgr  : public Singleton<StreamSchemaMgr> {
	typedef hash_map<StreamID, StreamSchema> SchemaMap;
public:
	friend class Cayuga::Utils::Singleton<StreamSchemaMgr>;   

	/**
	* @brief Output member variable information in non-XML format.
	* @param out The output stream handle.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date December 8, 2005
	* @version 1.0
	*/
	void write(ostream& out = cout) const;
	
	/**
	* @brief Insert a new stream schema into this symbol table.
	* @param name The string name of the stream
	* @param schema The schema of the stream
	* @return The newly assigned ID of the stream
	* @invariant The stream must NOT exist in the symbol table
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date December 8, 2005
	* @version 1.0
	*/
	StreamID insertNewSchema(const string& name, const StreamSchema& schema);


	//query attribute info of a given stream
	/**
	* @brief Get the ID of a given attribute in a stream.
	* @param streamID The stream given to look up
	* @param attrName The attribute given to look up
	* @return The ID of the input attribute
	* @invariant The attribute must exist in the schema
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date December 8, 2005
	* @version 1.0
	*/
	AttributeID getStreamAttrID (const StreamID& streamID, 
		const string& attrName) const;
	
	/**
	* @brief Get the type of a given attribute in a stream.
	* @param streamID The stream given to look up
	* @param attrID The attribute given to look up
	* @return The type of the input attribute
	* @invariant The attribute must exist in the schema
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date December 8, 2005
	* @version 1.0
	*/
	AttrTypeID getStreamAttrType (const StreamID& streamID, 
		const AttributeID attrID) const;

	/**
	* @brief Get the internal size of a given attribute in a stream.
			 See DataTypeMgr for the definition of internal size.
	* @see DataTypeMgr
	* @param streamID The stream given to look up
	* @param attrID The attribute given to look up
	* @return The internal size of the input attribute
	* @invariant The attribute must exist in the schema
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date December 8, 2005
	* @version 1.0
	*/
	size_t getInternalAttributeSize (const StreamID& streamID, 
		const AttributeID attrID) const;

	/**
	* @brief Get the internal offset of a given attribute in a stream.
			 See DataTypeMgr for the definition of internal size.
	* @see DataTypeMgr
	* @param streamID The stream given to look up
	* @param attrID The attribute given to look up
	* @return The internal offset of the input attribute
	* @invariant The attribute must exist in the schema
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date December 8, 2005
	* @version 1.0
	*/
	size_t getInternalAttributeOffset (const StreamID& streamID, 
		const AttributeID attrID) const;
	
	//query a given stream
	/**
	* @brief Get the ID of a given stream.
	* @param streamName The stream given to look up
	* @return The ID of the input stream
	* @invariant The stream schema must exist in the symbol table
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date December 8, 2005
	* @version 1.0
	*/
	StreamID getStreamID(const string& streamName) const;
	
	/**
	* @brief Get the name of a given stream.
	* @param sid The ID of the stream given to look up
	* @return The name of the input stream
	* @invariant The stream schema must exist in the symbol table
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 20, 2006
	* @version 1.0
	*/
	string getStreamName(const StreamID sid) const;

	/**
	* @brief Get the stream schema.
	* @param streamID The stream given to look up
	* @return The pointer to the stream schema
	* @invariant The stream schema must exist in the symbol table
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date December 8, 2005
	* @version 1.0
	*/
	const StreamSchema* getStreamSchema (const StreamID& streamID) const;

	/**
	* @brief Get the stream schema.
	* @param streamName The stream given to look up
	* @return The pointer to the stream schema
	* @invariant The stream schema must exist in the symbol table
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date September 13, 2006
	* @version 1.0
	*/
	const StreamSchema* getStreamSchema (const string& streamName) const;

	/**
	* @brief Get the internal size of the stream schema.
	* @param streamID The stream given to look up
	* @return The internal size of the stream schema
	* @invariant The stream schema must exist in the symbol table
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date December 8, 2005
	* @version 1.0
	*/
	size_t getInternalSchemaSize (const StreamID& streamID) const;
	
private:
	/**
	* This variable stores the map from schema IDs to schemas.
	*/
	SchemaMap _schemas;
	
	/**
	* This variable stores the map between schema IDs to names.
	*/
	ValueIDMap<string> _schemaIDMgr;
}; //class StreamSchemaMgr
}  //namespace SystemMgr
}  //namespace Cayuga

#endif //_STREAM_SCHEMA_MGR_H
