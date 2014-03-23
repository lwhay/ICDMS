/**
 * @file convertConstToByteArray.cpp
 * @date November 27, 2005
 * @brief Implementation file of AtomPredString.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "AutomatonCompiler/convertConstToByteArray.h"	
using namespace Cayuga::AutomatonCompiler;

#include "MemoryMgr/CY_IntStringFace.h"
#include "MemoryMgr/CY_GCFace.h"
using namespace CY_IntString;
using CY_Heap::registerRootObject;

#include "Utils/AttrMap.h"
using namespace Cayuga::Utils;

#include <cassert>
using namespace std;

//covert const_rhs to its appropriate type 
//and copy it to destArray
//return the size of the attribute copied
void Cayuga::AutomatonCompiler::convertConstToByteArray(
							 const string& attrTypeName,
							 const string& const_rhs,
							 const size_t size_rhs,
							 char* destArray,
							 bool bPinStringConst,
							 bool bUseExternalSpace) {
	if (attrTypeName == "int") {
		*(int*)(destArray) = convert<int>(const_rhs);		
	}
	else if (attrTypeName == "float") {
		*(float*)(destArray) = convert<float>(const_rhs);
	}
	else if (attrTypeName == "shortstring") {
		memcpy(destArray, const_rhs.c_str(), size_rhs);
	}
	else if (attrTypeName == "SAXEventType") {
		*(SAXEventType*)(destArray) = convert<SAXEventType>(const_rhs);
	}
	else if (attrTypeName == "string") {
		IntStringHandle hTag;

		hTag = lookup(const_rhs, 
			true,				//need to install
			bUseExternalSpace	
			);

		if (bPinStringConst) {
			//pin the string handle so that it is never garbage collected
			registerRootObject(hTag);
		}
		*(IntStringHandle*)(destArray) = hTag;
	}
	else {
		assert(false);
	}
}
