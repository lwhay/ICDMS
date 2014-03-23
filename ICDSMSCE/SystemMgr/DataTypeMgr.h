/**
 * @file DataTypeMgr.h
 * @date December 8, 2005
 * @brief This file defines a symbol table storing pairs of string name and 
		  integer ID for each data type. For example, <"int", 0>
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _DATA_TYPE_MGR_H
#define _DATA_TYPE_MGR_H

#include "BasicDataStructure/Predicate/AttributeID.h"
using Cayuga::BasicDataStructure::Predicate::AttrTypeID;

#include "Utils/Singleton.h"
#include "Utils/ValueIDMap.h"

namespace Cayuga {
namespace SystemMgr {
#define VARIABLE_LENGTH 0xffff
using Cayuga::Utils::Singleton;
using Cayuga::Utils::ValueIDMap;

class DataTypeMgr;
/**
 * @brief A DataTypeMgr object represents a symbol table storing pairs of 
		  string name and integer ID for each data type. 
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date December 8, 2005
 * @version 1.0
 */
class DataTypeMgr : public Singleton<DataTypeMgr> {
	friend class Cayuga::Utils::Singleton<DataTypeMgr>;
public:
	/**
	* @brief Get the ID of a given type.
	* @param typeName The type given to look up
	* @return The ID of the input type
	* @invariant The ID, type pair must exist
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date December 8, 2005
	* @version 1.0
	*/
	AttrTypeID getID(const string& typeName) const;

	/**
	* @brief Get the internal length of a given type. That is, if the type is 
	         stored in the GC heap, the handle size will be returned. The size
			 of primitive types (those not handled by heap) is not affected.
	* @param typeName The type given to look up
	* @return The internal length of the input type
	* @invariant The type must exist in the symbol table
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date December 8, 2005
	* @version 1.0
	*/
	unsigned getInternalLength(const string& typeName) const;

	/**
	* @brief Get the internal length of a given type. That is, if the type is 
	         stored in the GC heap, the handle size will be returned. The size
			 of primitive types (those not handled by heap) is not affected.
	* @param typeID The type given to look up
	* @return The internal length of the input type
	* @invariant The type must exist in the symbol table
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date December 8, 2005
	* @version 1.0
	*/
	unsigned getInternalLength(const AttrTypeID typeID) const;

	/**
	* @brief Get the name of a given type.
	* @param typeID The type given to look up
	* @return The string name of the input type
	* @invariant The type must exist in the symbol table
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date December 8, 2005
	* @version 1.0
	*/
	string getName(const AttrTypeID typeID) const;

	/**
	* @brief Insert a type into the symbol table.
	* @param typeName The name of the type
	* @param typeLen The length of the type
	* @param bIsComplexType Indicate whether this type is a complex type, define
			 to be a type that is not primitive. That is, a type handled in GC
			 heap.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date December 8, 2005
	* @version 1.0
	*/
	void insertType(const string& typeName, 
		const unsigned typeLen,
		const bool bIsComplexType = false
		);

private:
	/**
	* @brief Query whether the given type is a complex type
	* @param typeName The name of the type to query
	* @return True iff the type is complex
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date December 8, 2005
	* @version 1.0
	*/
	bool isComplexType(const string& typeName) const;

	/**
	* @brief Query whether the given type is a complex type
	* @param typeID The ID of the type to query
	* @return True iff the type is complex
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date December 8, 2005
	* @version 1.0
	*/
	bool isComplexType(const AttrTypeID typeID) const;

	/**
	* This variable stores the map between type names and IDs
	*/
	ValueIDMap<string> _typeNameMgr;
	
	/**
	* This variable stores the length of types in the symbol table.
	*/
	vector<unsigned> _typeLengthMgr;
	
	/**
	* This variable indicates whether each type in the symbol table is complex.
	*/
	vector<bool> _complexTypeMgr;
}; //class DataTypeMgr
}  //namespace SystemMgr
}  //namespace Cayuga

#endif //_DATA_TYPE_MGR_H
