/**
 * @file DurationStruct.h
 * @date April 22, 2006
 * @brief Definition of a duration predicate.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _DURATION_STRUCT_H
#define _DURATION_STRUCT_H

#include <vector>
#include <iostream>
#include <cassert>
using namespace std;

#include "BasicDataStructure/ByteCode.h"

#include "BasicDataStructure/Timestamp.h"
using Cayuga::BasicDataStructure::TimeStamp;

namespace Cayuga {
namespace BasicDataStructure {
namespace Predicate {
/**
* @brief A DurationStruct object represents a duration predicate.
* @author Mingsheng Hong (mshong@cs.cornell.edu)
* @date April 24, 2007
* @version 1.0
*/
struct DurationStruct {
	ByteCodeNS::ByteCode _op;
	TimeStamp _rhs;

	/**
	* @brief Evaluate the dynamic predicate.
	* @param lhs The left hand side of the constraint
	* @param bDeleteInst Set to true if the instance has "expired".
			 For <= or < constraint, if the predicate is violated here, due to
			 monotonicity of timestamp value, the instance has "expired" and can
			 be safely deleted.
	* @return True iff the predicate is satisfied.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 22, 2006
	* @version 1.0
	*/
	bool eval(TimeStamp lhs, bool& bDeleteInst) {
		bDeleteInst = false;
		bool ret;
		switch (_op) {
			case ByteCodeNS::EQ:
				return lhs == _rhs;
			case ByteCodeNS::NE:
				return lhs != _rhs;
			case ByteCodeNS::LE:
				ret = (lhs <= _rhs);
				if (!ret) {
					bDeleteInst = true;
				}
				return ret;
			case ByteCodeNS::LT:
				ret = (lhs < _rhs);
				if (!ret) {
					bDeleteInst = true;
				}
				return ret;
			case ByteCodeNS::GE:
				return lhs >= _rhs;
			case ByteCodeNS::GT:
				return lhs > _rhs;
			default:
				assert (false);
				return false;
		}
	}

	/**
	* @brief Equality comparison operator.
	* @param rhs The input DurationStruct for this struct to compare to
	* @return True iff the two structs are the same 
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date October 24, 2007
	* @version 1.0
	*/
	bool operator == (const DurationStruct& rhs) const {
		return (_op == rhs._op && _rhs == rhs._rhs);
	}

}; //struct DurationStruct
}  //namespace Predicate
}  //namespace BasicDataStructure
}  //namespace Cayuga

#endif //_DURATION_STRUCT_H
