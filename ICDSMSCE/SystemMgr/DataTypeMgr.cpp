/**
* @file DataTypeMgr.cpp
* @date April 25, 2006
* @brief This implementation file of class DataTypeMgr.
* @author Mingsheng Hong (mshong@cs.cornell.edu)
*/

#include "SystemMgr/DataTypeMgr.h"
using namespace Cayuga::SystemMgr;

#include "MemoryMgr/CY_HeapFace.h"


AttrTypeID DataTypeMgr::getID(const string& typeName) const {
	return _typeNameMgr.getID(typeName);
}

unsigned DataTypeMgr::getInternalLength(const AttrTypeID typeID) const {
	if (isComplexType(typeID)) {
		return sizeof(CY_Heap::GHandle);
	}
	else {
		return (unsigned)_typeLengthMgr[typeID];
	}
}

unsigned DataTypeMgr::getInternalLength(const string& typeName) const  {
	AttrTypeID typeID = _typeNameMgr.getID(typeName);
	return getInternalLength(typeID);
}


string DataTypeMgr::getName(const AttrTypeID typeID) const {
	return _typeNameMgr.getValue(typeID);
}

void DataTypeMgr::insertType(const string& typeName, 
							 const unsigned typeLen,
							 const bool bIsComplexType
							 )  {
	 _typeNameMgr.putValueAtEnd(typeName);
	 _typeLengthMgr.push_back(typeLen);
	 _complexTypeMgr.push_back(bIsComplexType);
}

bool DataTypeMgr::isComplexType(const string& typeName) const {
	AttrTypeID typeID = _typeNameMgr.getID(typeName);
	return _complexTypeMgr[typeID];
}

bool DataTypeMgr::isComplexType(const AttrTypeID typeID) const {
	return _complexTypeMgr[typeID];
}
