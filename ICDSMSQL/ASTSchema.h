/**
 * @file ASTSchema.h
 * @date April 4, 2007
 * @brief This file declares ASTSchema class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#pragma once

#include <string>
#include <map>
#include <vector>
using namespace std;

namespace Cayuga {

namespace SystemMgr {
class StreamSchema;
}

namespace QLCompiler {

struct NameType {
	string _name;
	string _type;
	string _alias;
	NameType(const string& name, 
		const string& type) 
		: _name(name), _type(type), _alias(name) {
	}

	NameType(const string& name, 
		const string& type,
		const string alias) 
		: _name(name), _type(type), _alias(alias) {
	}
};

typedef map<string, string> NameTypeMap;
typedef multimap<string, string> NameAliasMap;
typedef vector<string> AliasVector;

class ASTSchema;
using Cayuga::QLCompiler::ASTSchema;

/**
 * @brief An ASTSchemaIterator object represents an iterator over an
		  ASTSchema object.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date April 4, 2007
 * @version 1.0
 */
class ASTSchemaIterator {
	AliasVector::iterator _it;
	const ASTSchema* _thisSchema;
public:
	ASTSchemaIterator(AliasVector::iterator it, const ASTSchema* thisSchema);

	ASTSchemaIterator& operator =(const ASTSchemaIterator& rhs);

	void operator ++();

	bool operator !=(const ASTSchemaIterator& rhs);

	string getName();

	string getAliasName();

	string getDataType();

};

/**
 * @brief An ConstASTSchemaIterator object represents a constant iterator over 
		  an ASTSchema object.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date April 4, 2007
 * @version 1.0
 */
class ConstASTSchemaIterator {
	AliasVector::const_iterator _it;
	const ASTSchema* _thisSchema;
public:
	ConstASTSchemaIterator(AliasVector::const_iterator it, 
		const ASTSchema* thisSchema);

	//string getDataType(const string& name) {
	//	return "string";
	//}
	
	ConstASTSchemaIterator& operator =(const ConstASTSchemaIterator& rhs);

	void operator ++();

	bool operator !=(const ConstASTSchemaIterator& rhs);

	string getName();

	string getAliasName();

	string getDataType();

};

/**
 * @brief An ASTSchema object represents the schema of an AST node.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date April 4, 2007
 * @version 1.0
 */
class ASTSchema {

public:
	//friend class ASTSchemaIterator;

	//iterate over name and alias
	ASTSchemaIterator begin();

	ConstASTSchemaIterator begin( ) const;

	ASTSchemaIterator end();

	ConstASTSchemaIterator end() const;
	
	void insert(const NameType& nt);

	string getDataType(const string& name) const;

	bool empty() const;

	//set this schema with attributes from input pSchema
	void fromStreamSchema(const Cayuga::SystemMgr::StreamSchema* pSchema);

	//create a stream pSchema on heap from attributes in this schema, 
	//and return it. Note that the alias names will be used to create
	//the stream schema.
	Cayuga::SystemMgr::StreamSchema* toStreamSchema() const;

	//look for the input alias name
	bool findAliasName(const string& aliasName) const;

	//return the alias name of this attribute name. It could be identical to
	//the input name if "as" construct is not used
	string getAliasName(const string& name) const;

	//look for the input attribute name
	bool findName(const string& name) const;

	//return the name of the input alias. The alias must exist in the schema.
	string getName(const string& aliasName) const;

	//return true iff the two schema have overlapping attribute *alias*
	bool intersection(const ASTSchema& rhs) const;

	//return true iff this schema contains attributes of the RHS schema
	bool contain(const ASTSchema& rhs) const;

	//append attributes from the input schema into this schema
	void append(const ASTSchema& rhs);

	//set attribute names to alias
	void setNameToAlias();

private:
	//reset the state
	void reset();

	//vector of attribute names. could have duplicates if this is the schema
	//for the intermediate state of a mu operator
	AliasVector _aliases;

	//a map from name to type
	NameTypeMap _nameTypeMap;

	//a map from name to alias
	//in an intermediate NFA node for mu, there could be two attributes with
	//the same name. Therefore we use a multimap here.
	NameAliasMap _nameAliasMap;
};//class ASTSchema
} //QLCompiler
} //Cayuga
