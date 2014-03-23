/**
 * @file AttrMap.h
 * @date November 26, 2005
 * @brief Defines a list of utility functions that returns the value of a 
		  parameter by looking up attribute map by the name of the 
		  parameter. 
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _ATTR_MAP_H
#define _ATTR_MAP_H

#include "StringMap.h"

#include "SAXHandler/SAXEventType.h"

#include "SAXHandler/DataSource.h"

#include <string>
#include <cassert>
using namespace std;

using namespace Cayuga;

namespace Cayuga {

	//The followign declaration of enum structs do not work for Mac.
	//This is used to eliminate inclusion dependency from Utils to SAXHandler.
	/*
namespace SAXHandler {
enum SAXEventType;
enum DataSource;
}
	*/
using namespace Cayuga::SAXHandler;

namespace Utils {
/////////////////////////////////////////////
//The convert functions
//Used by SetParameter below
////////////////////////////////////////////
/**
* @brief Convert the parameter value from string to a typed object
* @param str The string value of the parameter.
* @return The actual typed value of the parameter.
* @author Mingsheng Hong (mshong@cs.cornell.edu)
* @date November 26, 2005
* @version 1.0
*/
template <typename T>
T convert(const string& str);


template <>
bool convert<bool>(const string& str);

template <>
char convert<char>(const string& str);

template <>
unsigned int convert<unsigned int>(const string& str);

template <>
int convert<int>(const string& str);

template <>
short convert<short>(const string& str);

template <>
float convert<float>(const string& str);

template <>
long convert<long>(const string& str);

template <>
string convert<string>(const string& str);

template <>
SAXEventType convert<SAXEventType>(const string& str);

template <>
DataSource convert<DataSource>(const string& str);

string toString(const long long n);

/////////////////////////////////////////////
//getParameterValue functions
//Returns the value of a parameter by
//looking up attrMap to find the name of the parameter. 
////////////////////////////////////////////
/**
* @brief Get the parameter value of a given name from the given 
		 attribute map.
* @param attrMap The attribute map that contains all parameter name, values 
		 pairs in string format
* @param attrName The name of the attribute
* @param defaultValue The default value of the attribute. It will be returned if
         the attribute map does not contain information of this attribute.
* @return The typed parameter value
* @author Mingsheng Hong (mshong@cs.cornell.edu)
* @date November 26, 2005
* @version 1.0
*/
template <typename T>
T getParameterValue(const StringMap& attrMap, 
				  const string& attrName, 
				  T defaultValue) {
	StringMap::const_iterator find_it = attrMap.find(attrName);
	if (find_it == attrMap.end()) {
		return defaultValue;
	}
	else {
		return convert<T>(find_it->second);
	}
}

/**
* @brief Get the parameter value of a given name from the given 
		 attribute map.
* @param attrMap The attribute map that contains all parameter name, values 
		 pairs in string format
* @param attrName The name of the attribute
* @return The typed parameter value
* @author Mingsheng Hong (mshong@cs.cornell.edu)
* @date November 26, 2005
* @version 1.0
*/
template <typename T>
T getParameterValue(const StringMap& attrMap, 
				  const string& attrName) {
	StringMap::const_iterator find_it = attrMap.find(attrName);
	assert (find_it != attrMap.end());
	return convert<T>(find_it->second);
}
}  //namespace Utils
}  //namespace Cayuga

#endif //_ATTR_MAP_H
