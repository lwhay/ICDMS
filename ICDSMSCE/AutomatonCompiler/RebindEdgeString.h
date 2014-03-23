/**
 * @file RebindEdgeString.h
 * @date November 27, 2005
 * @brief Stores the information of a rebind edge read from AIR.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _REBIND_EDGE_STRING_H
#define _REBIND_EDGE_STRING_H

#include "FilterEdgeString.h"

#include "AutomatonCompiler/SchemaMapString.h"

namespace Cayuga {
namespace AutomatonCompiler {
class Context;
class SchemaMapString;

/**
 * @brief A RebindEdgeString object represents a rebind edge constructed from 
		  SAX events.	
 * @see	EdgeString
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 27, 2005
 * @version 1.0
 */
class RebindEdgeString : public FilterEdgeString {
public:
	friend class IntermediateNodeString;

	/**
	* @brief Default constructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	RebindEdgeString ();

	/**
	* @brief Virtual destructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual ~RebindEdgeString();

	/**
	* @brief Compiles this edge into Cayuga engine, by translating
			 strings into integers with symbol tables, and so on.
	* @param context The compilation context
	* @return Pointer to the internalized edge.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual Edge* compile(Context& context);

	/**
	* @brief Output member variable information in XML format, aiming at 
			 reconstructing the XML piece in AIR which encodes this edge
	* @param out The output stream handle.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual void write(ostream& out = cout) const;

	/**
	* @brief Append an old, new attribute pair into the schema map.
	* @param attrPair The old, new attribute pair to be appended into the 
			          schema map
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual void appendAttrInSchema(const OldNewAttrPair& attrPair);

	/**
	* @brief Sets the hasDel attribute.
	* @param hasDel Whether the edge has a Del on it or not.
	* @author Varun Sharma (varun13687@yahoo.com)
	* @date June 19, 2006
	* @version 1.0
	*/
	void setDel(const bool& hasDel);
		
	/**
	* @brief Sets _schemaMap attribute. Will be used in QL Compiler.
	* @param pSchemaMap The input schema map to set to.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date October 1, 2006
	* @version 1.0
	*/
	void setSchemaMap(SchemaMapString* pSchemaMap);

//protected:
	/**
	* This variable stores the schema map on this edge.
	*/
	SchemaMapString _schemaMap;

	/**
	* This variable stores whether the edge has a del on it or not.
	*/

	bool _hasDel;
}; //class RebindEdgeString
}  //namespace AutomatonCompiler
}  //namespace Cayuga

#endif //_REBIND_EDGE_STRING_H
