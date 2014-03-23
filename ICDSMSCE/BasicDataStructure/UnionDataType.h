/**
 * @file UnionDataType.h
 * @date April 20, 2007
 * @brief Definition of a union data type struct.		  
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _UNION_DATA_TYPE_H
#define _UNION_DATA_TYPE_H

#include "MemoryMgr/CY_IntStringFace.h"
using namespace CY_IntString;

namespace Cayuga {
namespace BasicDataStructure {
/**
 * @brief A UnionDataType is a data type object used in Cayuga interpreter.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date April 20, 2007
 * @version 1.0
 */
struct UnionDataType {
	//union {
		int _i;
		float _f;
		IntStringHandle _s;
	//} _value;

		ByteCodeNS::ByteCode _type;
};//struct UnionDataType
} //namespace BasicDataStructure
} //namespace Cayuga

#endif //_UNION_DATA_TYPE_H
