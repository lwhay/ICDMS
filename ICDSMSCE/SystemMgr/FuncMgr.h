/**
 * @file FuncMgr.h
 * @date September 18, 2006
 * @brief This file defines a symbol table storing information for functions
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _FUNC_MGR_H
#define _FUNC_MGR_H

#include "BasicDataStructure/ByteCode.h"
using namespace Cayuga::BasicDataStructure;

#include "Utils/Singleton.h"
#include "Utils/ValueIDMap.h"

#include <map>
using namespace std;

namespace Cayuga {
namespace SystemMgr {
#define VARIABLE_LENGTH 0xffff

using Cayuga::Utils::Singleton;
using Cayuga::Utils::ValueIDMap;

//The supported user-defined function is at most binary, returning a value.
/**
 * @brief A FuncEntry object stores the return data type of the function and			  the function pointer.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date September 18, 2005
 * @version 1.0
 */
typedef void (*UDF_Ptr)(void* arg1, void* arg2, void* ret);

/**
* @brief A FuncEntry object is represents an entry for a user-defined function
	     stored in FuncMgr.
* @author Mingsheng Hong (mshong@cs.cornell.edu)
* @date September 18, 2006
* @version 1.0
*/
struct FuncEntry {
	ByteCodeNS::ByteCode returnType;
	UDF_Ptr fp;
	int numArgs;
	ByteCodeNS::ByteCode inputArg1Type;
	ByteCodeNS::ByteCode inputArg2Type;

	FuncEntry(ByteCodeNS::ByteCode type, UDF_Ptr thisFP, int thisNumArgs,
		ByteCodeNS::ByteCode thisInputArg1Type = ByteCodeNS::INVALID,
		ByteCodeNS::ByteCode thisInputArg2Type = ByteCodeNS::INVALID) 
		: returnType(type), fp(thisFP), numArgs(thisNumArgs),
			inputArg1Type(thisInputArg1Type), inputArg2Type(thisInputArg2Type) {
	}

};

class FuncMgr;
/**
 * @brief A FuncMgr object represents a symbol table storing information for 
		  functions.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date September 18, 2006
 * @version 1.0
 */
class FuncMgr : public Singleton<FuncMgr> {
	friend class Cayuga::Utils::Singleton<FuncMgr>;
public:
	/**
	* @brief Get the function pointer of a given function name. Throw exception
			 if the function name does not exist.
	* @param funcName The name given to look up
	* @return The function pointer
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date September 18, 2006
	* @version 1.0
	*/
	UDF_Ptr getPointer(const string& funcName) const;

	/**
	* @brief Get the return type of a given function name. Throw exception
			 if the function name does not exist.
	* @param funcName The name given to look up
	* @return The data type
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date September 18, 2006
	* @version 1.0
	*/
	ByteCodeNS::ByteCode getReturnType(const string& funcName) const;

	/**
	* @brief Get the number of input arguments of a given function name. 
			Throw exception if the function name does not exist.
	* @param funcName The name given to look up
	* @return The number of arguments
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date September 20, 2006
	* @version 1.0
	*/
	int getNumArgs (const string& funcName) const;
	

	/**
	* @brief Insert a new function entry into the symbol table.
	* @param funcName The name of the function.
	* @param retType The return data type of the function.
	* @param fp The pointer to the function.
	* @param numArgs The number of input arguments to the function.
	* @param thisInputArg1Type The data type of the first input argument.
	* @param thisInputArg2Type The data type of the second input argument.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date September 18, 2006
	* @version 1.0
	*/
	void insertFuncEntry(const string& funcName, 
		const ByteCodeNS::ByteCode retType,
		const UDF_Ptr fp,
		const int numArgs,
		const ByteCodeNS::ByteCode thisInputArg1Type = ByteCodeNS::INVALID,
		const ByteCodeNS::ByteCode thisInputArg2Type = ByteCodeNS::INVALID
		);
	
	/**
	* @brief Return the function entry of the input function name. Throw 
	exception if the function name does not exist.
	* @param funcName The name given to look up
	* @return The function entry.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date September 18, 2006
	* @version 1.0
	*/
	FuncEntry getEntry(const string& funcName) const;

private:

	/**
	* This variable stores the map between type names and IDs
	*/
	map<string, FuncEntry> _funcMgr;
	
	
}; //class FuncMgr
}  //namespace SystemMgr
}  //namespace Cayuga

#endif //_FUNC_MGR_H
