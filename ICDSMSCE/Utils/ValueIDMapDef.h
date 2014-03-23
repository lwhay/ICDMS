/**
 * @file ValueIDMapDef.h
 * @date October 26, 2006
 * @brief The file contains the definition of template class ValueIDMap.

 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _VALUE_ID_MAP_DEF_H
#define _VALUE_ID_MAP_DEF_H

#include <cassert>
using namespace std;

#include "ValueIDMap.h"

using Cayuga::Utils::ValueIDMap;

template <typename T>
ValueIDMap<T>::ValueIDMap() {
}

template <typename T>
ValueIDMap<T>::~ValueIDMap() {
}

template <typename T>
void ValueIDMap<T>::write(ostream& out) const  {
	for (typename ValueSequence::const_iterator it = _values.begin(); 
		it != _values.end(); ++it) {
			out<<*it<<"\t";
	}
	out<<endl;
}

template <typename T>
const unsigned ValueIDMap<T>::getID(const T& rhs) const{
	typename ValueMap::const_iterator it = _valueIDMap.find(rhs);
	if (it == _valueIDMap.end()) {
		throw "No ID for value " + string(rhs);    
	}
	return it->second;
}

template <typename T>
T ValueIDMap<T>::getValue(const unsigned id) const {
	//Mingsheng 06.10.27 For some reason I don't understand, GCC
	//says assert was not declared in this scope
	//assert (id >= 0 && id < _values.size());
	return _values[id];
}

template <typename T>
unsigned ValueIDMap<T>::putValueAtEnd(const T& rhs) {
	pair<typename ValueMap::iterator, bool> return_it =
		_valueIDMap.insert(make_pair(rhs, (unsigned)_values.size()));
	if (return_it.second) {
		_values.push_back(rhs);
		return (unsigned)_values.size()-1;
	}
	else {
		//the same T has been inserted before, return it
		return (return_it.first)->second;
		//cerr<<"error: "<<rhs<<" already exists."<<endl;
		//exit (-1);
		//assert (false);
		//return 0;

	}
}

template <typename T>
void ValueIDMap<T>::clear() {
	_values.clear();
	_valueIDMap.clear();
}

template <typename T>
bool ValueIDMap<T>::operator == (const ValueIDMap<T>& rhs) const {
	if (_values != rhs._values) {
		return false;
	}

	return true;
}

template <typename T>
bool ValueIDMap<T>::operator != (const ValueIDMap<T>& rhs) const {
	return !(*this == rhs);
}

#endif //_VALUE_ID_MAP_DEF_H
