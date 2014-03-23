/**
 * @file convertConstToByteArray.h
 * @date April 7, 2007
 * @brief Forward declaration of 
			Cayuga::AutomatonCompiler::convertConstToByteArray().
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */
#ifndef _CONVERT_CONST_TO_BYTE_ARRAY_H
#define _CONVERT_CONST_TO_BYTE_ARRAY_H

#include <string>

namespace Cayuga {
namespace AutomatonCompiler {
/**
* @brief Convert an input constant into the event body, which is a byte array
* @param attrTypeName The data type of the attribute
* @param const_rhs The string value of the constant.
* @param size_rhs The size of the input constant.
* @param destArray The destination of the attribute value.
* @param bPinStringConst If the constant is of type string, and thus need to be
         internalized into heap, the heap string object should be pinned (not 
		 to be collected) iff bPinStringConst is true.
* @param bUseExternalSpace Only applicable when the constant is of type string, 
		 and thus need to be internalized into heap.
		 If bUseExternalSpace is true, the string will be internalized into
		 external space. Otherwise it is internalized into from space
* @author Mingsheng Hong (mshong@cs.cornell.edu)
* @date November 30, 2005
* @version 1.0
*/
void convertConstToByteArray(const std::string& attrTypeName,
							 const std::string& const_rhs,
							 const size_t size_rhs,
							 char* destArray,
							 bool bPinStringConst,
							 bool bUseExternalSpace);
}  //namespace AutomatonCompiler
}  //namespace Cayuga

#endif //_CONVERT_CONST_TO_BYTE_ARRAY_H
