/**
 * @file StreamSchema.h
 * @date December 8, 2005
 * @brief This file defines the schema of a stream, which consists of a 
		  sequence of pairs of string attribute names and their data types.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */


#ifndef _STREAM_SCHEMA_H
#define _STREAM_SCHEMA_H

#include "BasicDataStructure/Predicate/AttributeID.h"
#include "BasicDataStructure/StreamID.h"

#include "Utils/ValueIDMap.h"
using Cayuga::Utils::ValueIDMap;

#include <vector>

namespace Cayuga {
	namespace BasicDataStructure {
		struct OutputStreamType;

		namespace Automaton {
			class Node;
			class Instance;
		}

		namespace EventNS {
			class Event;
		}

		class CMessage;
	}

namespace SystemMgr {
using Cayuga::BasicDataStructure::Automaton::Node;
using Cayuga::BasicDataStructure::Automaton::Instance;
using Cayuga::BasicDataStructure::Predicate::AttributeID;
using Cayuga::BasicDataStructure::Predicate::AttrTypeID;
using Cayuga::BasicDataStructure::StreamID;

using Cayuga::BasicDataStructure::EventNS::Event;

using Cayuga::BasicDataStructure::OutputStreamType;
using Cayuga::BasicDataStructure::CMessage;

class DataTypeMgr;

/**
 * @brief A StreamSchema object represents the schema of a stream, which 
		  consists of a sequence of pairs of string attribute names and their 
		  data types.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date December 8, 2005
 * @version 1.0
 */
class StreamSchema: public ValueIDMap<string> {
	typedef vector<AttrTypeID> TypeSequence;
public:
	/**
	* @brief Default constructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date December 8, 2005
	* @version 1.0
	*/
	StreamSchema();
	
	/**
	* @brief Virtual destructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date December 8, 2005
	* @version 1.0
	*/
	virtual ~StreamSchema();

	/**
	* @brief Output member variable information in non-XML format.
	* @param out The output stream handle.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date December 8, 2005
	* @version 1.0
	*/
	void write(ostream& out = cout) const;

	/**
	* @brief Print the event conforming to the schema
	* @param ev The event
	* @param out The output stream handle.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 5, 2006
	* @version 1.0
	*/
	void printEvent(const Event* ev, 
		ostream& out = cout) const;

	/**
	* @brief Print the event conforming to the schema in FileER format.
	* @param ev The event
	* @param outputStreams The names of the output streams
	* @param out The output stream handle.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date September 18, 2006
	* @version 1.0
	*/
	void printEventInFileERFormat(
		const Event* ev, 
		const vector<OutputStreamType>& outputStreams,
		ostream& out = cout) const;

	/**
	* @brief put the event schema and content information as fields 
			into the event trace message.
	* @param ev The input event
	* @param msgOut The msg in which the event fields are stored.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 3, 2007
	* @version 1.0
	*/
	void genEventSchemaContentForTraceMsg(const Event* ev, 
		CMessage& msgOut) const;

	/**
	* @brief Put the input attribute at the end of the schema.
	* @param attrName The name of the attribute
	* @param typeName The type of the attribute
	* @invariant The attribute must exist in the schema
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date December 8, 2005
	* @version 1.0
	*/
	void putAttributeAtEnd(const string& attrName, const string& typeName);

	/**
	* @brief Update the generating node to the input node.
	* @param pNode The point to the node that outputs this stream.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 5, 2006
	* @version 1.0
	*/
	void setGeneratingNode(Node* pNode);

	/**
	* @brief Get the generating nodes. Note that there could be multiple end
	* nodes generating the same schema.
	* @return A collection of pointers to the generating nodes.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 5, 2006
	* @version 1.0
	*/
	vector<Node*>
		getGeneratingNodes() const; 

	/**
	* @brief Get the type of a given attribute.
	* @param attrID The attribute given to look up
	* @return The type of the input attribute
	* @invariant The attribute must exist in the schema
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date December 8, 2005
	* @version 1.0
	*/
	AttrTypeID getAttributeType(const AttributeID attrID) const;
	
	/**
	* @brief Get the internal size of a given attribute. See DataTypeMgr for the
			 definition of internal size.
	* @see DataTypeMgr
	* @param attrID The attribute given to look up
	* @return The internal size of the input attribute
	* @invariant The attribute must exist in the schema
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date December 8, 2005
	* @version 1.0
	*/
	size_t getInternalAttributeSize(const AttributeID attrID) const;

	/**
	* @brief Get the internal offset of a given attribute in the schema. 

			 To calculate internal offset, internal lengths of attributes are 
			 used.
    * @see DataTypeMgr
	* @param attrID The attribute given to look up
	* @return The internal offset of the input attribute
	* @invariant The attribute must exist in the schema
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date December 8, 2005
	* @version 1.0
	*/
	size_t getInternalAttributeOffset(const AttributeID attrID) const;

	/**
	* @brief Get the internal size of the schema.
	* @return As above
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date December 8, 2005
	* @version 1.0
	*/
	size_t getInternalSchemaSize() const;

	/**
	* @brief Get the number of attributes in the schema.
	* @return As above
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 7, 2006
	* @version 1.0
	*/
	size_t getNumAttributes() const;

	/**
	* @brief Equality comparison operator.
	* @param rhs The input schema for this schema to compare to
	* @return True iff this schema has the same sequence of attributes as that 
			  of rhs
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual bool operator == (const StreamSchema& rhs) const;
	
	/**
	* @brief Inequality comparison operator.
	* @param rhs The input schema for this schema to compare to
	* @return True iff this schema does not have the same sequence of 
			  attributes as that of rhs
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual bool operator != (const StreamSchema& rhs) const;

	/**
	* @brief Clear this schema
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date Feburary 16, 2006
	* @version 1.0
	*/
	virtual void clear();

private:
	/**
	* @brief pretty print the input attribute into out
	* @param attrTypeName The input attribute type name
	* @param ev The input data array from which attribute value can be read
	* @param offset The offset in ev where this attribute starts
	* @param out The output stream handle
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date Feburary 16, 2006
	* @version 1.0
	*/
	void _displayTypedAttributeValue(const string& attrTypeName, 
								const char* ev, 
								size_t offset, 
								ostream& out = cout) const;

	/**
	* This variable stores a pointer to the data type manager.
	* It is used for DLL code, where DataTypeMgr has to be passed in, and 
	* invoking DataTypeMgr::getInstance() in DLL is wrong.
	*/
	DataTypeMgr* _dataTypeMgr;

	/**
	* This variable stores the types for the sequence of the attributes in this
	* schema.
	*/
	TypeSequence _types;

	/**
	* This variable stores the pointer to the node that generates this stream.
	* NULL if this stream is external.
	*/
	vector<Node*> _pGeneratingNodes;
}; //class StreamSchema
}  //namespace SystemMgr
}  //namespace Cayuga

#endif //_STREAM_SCHEMA_H
