/**
* @file FuncMgr.cpp
* @date September 18, 2006
* @brief This implementation file of class FuncMgr.
* @author Mingsheng Hong (mshong@cs.cornell.edu)
*/

#include "SystemMgr/FuncMgr.h"
using namespace Cayuga::SystemMgr;

#include <cassert>

UDF_Ptr FuncMgr::getPointer(const string& funcName) const {
	const FuncEntry& fe = getEntry(funcName);
	return fe.fp;
}

ByteCodeNS::ByteCode FuncMgr::getReturnType(const string& funcName) const {
	const FuncEntry& fe = getEntry(funcName);
	return fe.returnType;
}

int FuncMgr::getNumArgs (const string& funcName) const {
	const FuncEntry& fe = getEntry(funcName);
	return fe.numArgs;
}

void FuncMgr::insertFuncEntry(const string& funcName, 
		const ByteCodeNS::ByteCode retType,
		const UDF_Ptr fp,
		const int numArgs,
		const ByteCodeNS::ByteCode thisInputArg1Type,
		const ByteCodeNS::ByteCode thisInputArg2Type) {
	assert (_funcMgr.find(funcName) == _funcMgr.end());
	
	_funcMgr.insert(make_pair(funcName, FuncEntry(retType, fp, numArgs,
		thisInputArg1Type, thisInputArg2Type)));

}

FuncEntry FuncMgr::getEntry(const string& funcName) const {
	map<string, FuncEntry>::const_iterator find_it = _funcMgr.find(funcName);
	if (find_it == _funcMgr.end()) {
		//cerr<<"cannot find UDF name "<<funcName<<endl;
		throw "cannot find UDF name " + funcName;
	}

	return find_it->second;
}
