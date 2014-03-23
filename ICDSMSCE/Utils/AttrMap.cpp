/**
 * @file AttrMap.cpp
 * @date November 26, 2005
 * @brief The implementation of the utility functions defined in AttrMap.h
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "Utils/AttrMap.h"

#include "SAXHandler/SAXEventType.h"
#include "SAXHandler/DataSource.h"

#include <cstdio>
#include <cstdlib>
using namespace std;

template <>
bool Cayuga::Utils::convert<bool>(const string& str) {
	if (str == "true")
		return true;
	else
		return false;
}

template <>
char Cayuga::Utils::convert<char>(const string& str) {
	assert (str.size() == 1);
	return (str.at(0));
}


template <>
unsigned int Cayuga::Utils::convert<unsigned int>(const string& str) {
	return atoi(str.c_str());
}

template <>
int Cayuga::Utils::convert<int>(const string& str) {
	return atoi(str.c_str());
}

template <>
short Cayuga::Utils::convert<short>(const string& str) {
	return (short)atoi(str.c_str());
}

template <>
long Cayuga::Utils::convert<long>(const string& str) {
	return atol(str.c_str());
}

template <>
float Cayuga::Utils::convert<float>(const string& str) {
	return (float)atof(str.c_str());
}

template <>
string Cayuga::Utils::convert<string>(const string& str) {
	return str;
}

template <>
SAXEventType Cayuga::Utils::convert<SAXEventType>(const string& str) {
	if (str == "START_ELEM") {
		return START_ELEM;
	}
	else if (str == "END_ELEM") {
		return END_ELEM;
	}
	else if (str == "CHUNK") {
		return CHUNK;
	}
	else {
		assert(false);
		return START_ELEM;
	}
}

template <>
DataSource Cayuga::Utils::convert<DataSource>(const string& str) {
	if (str == "FILE") {
		return Cayuga::SAXHandler::FILE;
	}
	else if (str == "DIR") {
		return Cayuga::SAXHandler::DIR;
	}
	else if (str == "NETWORK") {
		return Cayuga::SAXHandler::NETWORK;
	}
	else {
		assert(false);
		return Cayuga::SAXHandler::FILE;
	}
}

string Cayuga::Utils::toString(const long long n) {
	//_itoa does not exist on Mac
	//char strBuffer[100];
	//char* pBuffer = _itoa(n, strBuffer, 10);
	//return string(pBuffer);

    char intStr[100] ;
    sprintf( intStr, "%lld", n ) ;
    return string(intStr);
}
