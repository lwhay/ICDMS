/**
 * @file ValueIDMap.cpp
 * @date November 26, 2005
 * @brief This file defines the utility function below.
		  It also explicitly instantiates a few uses of ValueIDMap.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include <vector>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Explicit instantiations for ValueIDMap
////////////////////////////////////////////////////////////////////////////////

#include "Utils/ValueIDMapDef.h"
template class ValueIDMap<string>;

////////////////////////////////////////////////////////////////////////////////
// Utility function definitions
////////////////////////////////////////////////////////////////////////////////

/**
* @brief Equality comparison operator.
* @param lhs The left hand side input vector
* @param rhs The right hand size input vector
* @return True iff the two vectors have the same sequence of values
* @author Mingsheng Hong (mshong@cs.cornell.edu)
* @date November 26, 2005
* @version 1.0
*/
template <typename T> 
bool operator ==(const vector<T>& lhs,
				 const vector<T>& rhs) {
	if (lhs.size() != rhs.size()) {
		return false;
	}

	for (size_t i = 0; i < lhs.size(); ++i) {
		if (lhs[i] != rhs[i]) {
			return false;
		}
	}
	return true;
}
