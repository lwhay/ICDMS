/**
 * @file ASTSchema.cpp
 * @date April 4, 2007
 * @brief This file defines ASTSchema class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "ASTSchema.h"
using namespace Cayuga::QLCompiler;

#include "../ICDSMSCE/SystemMgr/DataTypeMgr.h"
#include "../ICDSMSCE/SystemMgr/StreamSchema.h"
using namespace Cayuga::SystemMgr;

#include "../ICDSMSCE/Utils/RAssert.h"
using namespace Cayuga::Utils;

#include <algorithm>
#include <iostream>

////////////////////////////////////////////////////////////////////////////////
// ASTSchemaIterator member function definitions
////////////////////////////////////////////////////////////////////////////////

ASTSchemaIterator ::ASTSchemaIterator(AliasVector::iterator it, 
									  const ASTSchema* thisSchema) 
: _it(it), _thisSchema(thisSchema) {

}

ASTSchemaIterator& ASTSchemaIterator::operator =(const ASTSchemaIterator& rhs) {
	if (this == &rhs)
		return *this;

	_it = rhs._it;
	_thisSchema = rhs._thisSchema;
	return *this;
}

void ASTSchemaIterator::operator ++() {
	++_it;
}

bool ASTSchemaIterator::operator !=(const ASTSchemaIterator& rhs) {
	Assert (_thisSchema == rhs._thisSchema);
	return _it != rhs._it;
}

string ASTSchemaIterator::getName() {
	string name = _thisSchema->getName(*_it);
	return name;
}

string ASTSchemaIterator::getAliasName() {
	return *_it;
}

string ASTSchemaIterator::getDataType() {
	string name = _thisSchema->getName(*_it);
	return _thisSchema->getDataType(name);
}

	
////////////////////////////////////////////////////////////////////////////////
// ConstASTSchemaIterator member function definitions
////////////////////////////////////////////////////////////////////////////////

ConstASTSchemaIterator::ConstASTSchemaIterator(
	AliasVector::const_iterator it, 
	const ASTSchema* thisSchema) 
: _it(it), _thisSchema(thisSchema) {

}


ConstASTSchemaIterator& ConstASTSchemaIterator::operator =(
	const ConstASTSchemaIterator& rhs) {
	if (this == &rhs)
		return *this;

	_it = rhs._it;
	_thisSchema = rhs._thisSchema;
	return *this;

}

void ConstASTSchemaIterator::operator ++() {
	++_it;
}

bool ConstASTSchemaIterator::operator !=(const ConstASTSchemaIterator& rhs) {
	Assert (_thisSchema == rhs._thisSchema);
	return _it != rhs._it;
}

string ConstASTSchemaIterator::getName() {
	string name = _thisSchema->getName(*_it);
	return name;
}

string ConstASTSchemaIterator::getAliasName() {
	return *_it;
}

string ConstASTSchemaIterator::getDataType() {
	string name = _thisSchema->getName(*_it);
	return _thisSchema->getDataType(name);
}

////////////////////////////////////////////////////////////////////////////////
// ASTSchema member function definitions
////////////////////////////////////////////////////////////////////////////////

ASTSchemaIterator ASTSchema::begin() {
	//return _nameAliasMap.begin();
	return ASTSchemaIterator(_aliases.begin(), this);
}

ConstASTSchemaIterator ASTSchema::begin( ) const {
	//return _nameAliasMap.begin();
	return ConstASTSchemaIterator(_aliases.begin(), this);
}

ASTSchemaIterator ASTSchema::end() {
	//return _nameAliasMap.end();
	return ASTSchemaIterator(_aliases.end(), this);
}

ConstASTSchemaIterator ASTSchema::end() const {
	//return _nameAliasMap.end();
	return ConstASTSchemaIterator(_aliases.end(), this);
}

void ASTSchema::reset() {
	_aliases.clear();
	_nameTypeMap.clear();
	_nameAliasMap.clear();
}

void ASTSchema::insert(const NameType& nt) {
	//assert (!findName(nt._name));

	_aliases.push_back(nt._alias);

	_nameTypeMap.insert(make_pair(nt._name, nt._type));
	_nameAliasMap.insert(make_pair(nt._name, nt._alias));
}

string ASTSchema::getDataType(const string& name) const {
	NameTypeMap::const_iterator find_it = _nameTypeMap.find(name);
	if (find_it == _nameTypeMap.end()) {
		//cerr<<"error: cannot find attribute name "<<name<<endl;
		throw "error: cannot find attribute name " + name;
	}
	return find_it->second;
}

bool ASTSchema::empty() const {
	return _nameTypeMap.empty();
}

void ASTSchema::fromStreamSchema(const StreamSchema* pSchema) {
	for (size_t attr_idx = 0; attr_idx < pSchema->getNumAttributes(); 
		++attr_idx) {
			AttrTypeID attrTypeID = pSchema->getAttributeType(attr_idx);
			string attrType = DataTypeMgr::getInstance()->getName(attrTypeID);
			string attrName = pSchema->getValue((const unsigned int)attr_idx);
			insert(NameType(attrName, attrType));	
	}
}

StreamSchema* ASTSchema::toStreamSchema() const {
	StreamSchema* pSchema = new StreamSchema();
	for (ConstASTSchemaIterator it = begin(); it != end(); ++it) {
		string attrName = it.getName();
		string aliasName = it.getAliasName();
		string attrType = getDataType(attrName);

		pSchema->putAttributeAtEnd(aliasName, attrType);
	}
	return pSchema;
}

bool ASTSchema::findAliasName(const string& aliasName) const {
	//return _nameAliasMap.find(name) != _nameAliasMap.end();
	for (NameAliasMap::const_iterator it = _nameAliasMap.begin();
		it != _nameAliasMap.end(); ++it) {
			if (it->second == aliasName)
				return true;
	}
	return false;
}

string ASTSchema::getAliasName(const string& name) const {
	NameAliasMap::const_iterator find_it = _nameAliasMap.find(name);
	if (find_it == _nameAliasMap.end()) {
		//cerr<<"error: cannot find attribute name "<<name<<endl;
		throw "error: cannot find attribute name " + name;
	}
	return find_it->second;
}

bool ASTSchema::findName(const string& name) const {
	return _nameTypeMap.find(name) != _nameTypeMap.end();
}

string ASTSchema::getName(const string& aliasName) const {
	//return _nameTypeMap.find(name) != _nameTypeMap.end();
	for(NameAliasMap::const_iterator it = _nameAliasMap.begin(); 
		it != _nameAliasMap.end(); ++it) {
			if (aliasName == it->second)
				return it->first;
	}

	Assert (false);
	return "";
}

bool ASTSchema::intersection(const ASTSchema& rhs) const {
	//NameTypeMap temp_result;
	//// build the result in a temporary container
	//set_intersection(begin(), end(), rhs.begin(), rhs.end(), 
	//	temp_result.begin()); 

	////two schemas intersect iff the intersection is non-empty
	//return !temp_result.empty();

	for(NameAliasMap::const_iterator it = _nameAliasMap.begin(); 
		it != _nameAliasMap.end(); ++it) {
		if (rhs.findAliasName(it->second))
			return true;
	}

	return false;
}

bool ASTSchema::contain(const ASTSchema& rhs) const {
	for(ConstASTSchemaIterator it = rhs.begin(); 
		it != rhs.end(); ++it) {
		if (!findAliasName(it.getName()))
			return false;
	}

	return true;
}

void ASTSchema::append(const ASTSchema& rhs) {
	for(ConstASTSchemaIterator it = rhs.begin(); it != rhs.end(); ++it) {
		string attrName = it.getName();
		string attrType = rhs.getDataType(attrName);

		insert(NameType(attrName, attrType));
	}
}

//set attribute names to alias
void ASTSchema::setNameToAlias() {
	//two loops for this process, in order not to mess up the iterators
	//in the first loop, collect all current name, type, alias
	vector<NameType> tmp;
	for(ASTSchemaIterator it = begin(); it != end(); ++it) {
		string attrName = it.getName();
		string alias = it.getAliasName();

		string attrType = getDataType(attrName);

		tmp.push_back(NameType(attrName, attrType, alias));
	}

	reset();

	for (vector<NameType>::iterator it = tmp.begin(); it != tmp.end(); ++it) {			
		//_nameTypeMap.erase(it->_name);
		//_nameAliasMap.erase(it->_name);
		insert(NameType(it->_alias, it->_type, it->_alias));
	}
}
