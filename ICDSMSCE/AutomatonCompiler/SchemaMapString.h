/**
 * @file SchemaMapString.h
 * @date November 27, 2005
 * @brief Stores the information of a schema map read from AIR.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _SCHEMA_MAP_STRING_H
#define _SCHEMA_MAP_STRING_H

#include "BasicDataStructure/StreamID.h"

#include <cassert>
#include <iostream>
#include <set>
#include <string>
#include <map>
#include <vector>
#include <typeinfo>

using namespace std;

namespace Cayuga {
namespace BasicDataStructure {
namespace Automaton {
	class Node;
	class IntermediateNode;
}
}

using namespace Cayuga::BasicDataStructure::Automaton;

namespace AutomatonCompiler {

/**
 * @brief Stores an old, new attribute pair or an aggregate specification 
		 for a schema map.
		 In the latter case, the three member fields _src, _oldAttr and _newAttr
		 are actually reused for other meanings as follows: 
		 name of aggregate, the event attribute and the node attribute involved
		 in the aggregate computation.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 27, 2005
 * @version 1.0
 */
class OldNewAttrPair {
public:
	/**
	* @brief Default constructor
	* @param src Source of the attribute, which could be EV or NODE
	* @param oldAttr Name of the attribute in the source 
	* @param newAttr Name of the attribute in the destination
	* @param bIsAggregate Indicates whether this is an aggregate specification.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	OldNewAttrPair (const string& src,
		const string& oldAttr,
		const string& newAttr,
		bool bIsAggregate = false);

	/**
	* @brief second constructor. 
	* @invariant The client needs to make sure to properly set the members of
				this object before it is used
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 18, 2007
	* @version 1.0
	*/
	OldNewAttrPair ();

	/**
	* @brief Output member variable information in XML format, aiming at 
			 reconstructing the XML piece in AIR which encodes this old,
			 new attribute pair.
	* @param out The output stream handle.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	void write(ostream& out = cout) const;

	/** 
	* This variable stores the source of the attribute, which could be EV 
	* or NODE.
	*/
	string _src;
	
	/** 
	* This variable stores the name of the attribute in the source.
	*/
	string _oldAttr;
	
	/** 
	* This variable stores the name of the attribute in the destination.
	*/
	string _newAttr;

	/** 
	* This variable indicates whether this three string member fields of this 
	* class is reused for storing aggregate specification.
	*/
	bool _bIsAggregate;
}; //class OldNewAttrPair

/**
 * @brief Stores a schema map on a FR edge
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 27, 2005
 * @version 1.0
 */
class SchemaMapString {
public:
	//maps from the attribute in destination node schema to this
	//schema map entry
	//typedef hash_map<string, OldNewAttrPair> _SchemaMap;
	typedef map<string, OldNewAttrPair> _SchemaMap;

	/**
	* @brief Default constructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	SchemaMapString();

	/**
	* @brief Destructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	~SchemaMapString();

	/**
	* @brief Output member variable information in XML format, aiming at 
			 reconstructing the XML piece in AIR which encodes this schema map.
	* @param out The output stream handle.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	void write(ostream& out = cout) const;


	/**
	* @brief Compile the schema map read from AIR into a program.
	* @param pSourceNode The pointer to the source node of the edge with which 
			 the schema map is associated.
	* @param pDestNode The pointer to the destination node.
	* @param schemaLen The length of the schema to be set by this function.
	* @return The pointer to the program.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 25, 2006
	* @version 1.0
	*/
	char* compile(Node* pSourceNode, 
		IntermediateNode* pDestNode,
		size_t& schemaLen) const;

	/**
	* @brief Return true iff this schema map is empty.
	* @return As above.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	bool empty() const;

	/**
	* @brief Append an old, new attribute pair in this schema map
	* @param attrPair The old, new attribute pair to append
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	void push_back(const OldNewAttrPair& attrPair);

	/**
	* @brief Checks whether the input function name is an SDF
	* @return True iff it is one of the system-defined functions: SUM, COUNT,
			  MIN, MAX
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date September 22, 2006
	* @version 1.0
	*/
	static bool isSDF(const string& aggName);

	/**
	* @brief Return true iff some of the attribute name from the input 
			collection is created by a UDF entry in this schema map
	* @return As above
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 17, 2007
	* @version 1.0
	*/
	bool attributeCreatedByUDF(const set<string>& referredAttrs) const;

	/**
	* @brief Compose and merge the input schema map into this one. Let this
			 map be g, the input map be f, then the composed one should be
			 f(g). The new dest schema conforms to that of f.
	* @param inputSchemaMap The input schema map to compose 
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 18, 2007
	* @version 1.0
	*/
	void composeSchemaMapToSchemaOfInputMap(
		const SchemaMapString* inputSchemaMap);

	/**
	* @brief Compose and merge the input schema map into this one. Let this
			 map be g, the input map be f, then the composed one should be
			 g(f). The new dest schema conforms to that of g.
	* @param inputSchemaMap The input schema map to compose 
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 19, 2007
	* @version 1.0
	*/
	void composeSchemaMapToSchemaOfCurrentMap(
		const SchemaMapString* inputSchemaMap);

	/**
	* @brief Find the old, new attribute pair where the input attribute is 
	         the destination attribute
    * @invariant The input attribute must be found.
	* @return Reference to the found old, new attribute pair
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	const OldNewAttrPair& findDestAttr(const string& destAttr) const;

	/**
	* @brief Find the old, new attribute pair where the input attribute is 
	         the destination attribute. If cannot find return NULL.
	* @return Pointer to the found old, new attribute pair, or NULL.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 19, 2007
	* @version 1.0
	*/
	const OldNewAttrPair* findDestAttrOrNULL(const string& destAttr) const;

	/**
	* @brief Return true if this schema map is identity. That is, for each
			 dest attr, its source attr has the same name, and comes from
			 either NODE or EV. Note that the map is still identity if it 
			 simply projects out some attributes.
	* @return Pointer to the found old, new attribute pair, or NULL.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 19, 2007
	* @version 1.0
	*/
	//bool IsIdentityMap() const;

	/**
	* @brief Given the input attribute, if it is in the range of this schema
			 map, return true, and set outAttr its inverse under this map.		
			 Otherwise return false.
	* @param inAttr The input attribute.
	* @param outAttr Output parameter. Only set when the input attribute is 
			 in the range of this map.
			 In that case set outAttr to its inverse under this map.
	* @param outAttrFromEvent Output parameter. Only set when the input 
			 attribute is in the range of this map.
			 In that case outAttrFromEvent is true iff outAttr is from EV.
	* @return As above.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 22, 2007
	* @version 1.0
	*/
	bool renameAttrRefAndSource(const string& inAttr, 
		string& outAttr,
		bool& outAttrFromEvent) const;
private:
	/** 
	* This variable stores the schema map indexed on destination attribute.
	*/
	_SchemaMap _schemaMap;

	/** 
	* This variable stores the destination attributes in insertion order.
	* It is used to output schema map entries in insertion order.
	*/
	vector<string> _destAttrs;

}; //class SchemaMapString
}  //namespace AutomatonCompiler
}  //namespace Cayuga

#endif //_SCHEMA_MAP_STRING_H
