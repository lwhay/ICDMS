/**
 * @file FilterEdgeString.h
 * @date November 27, 2005
 * @brief Stores the information of a filter edge read from AIR.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */


#ifndef _FILTER_EDGE_STRING_H
#define _FILTER_EDGE_STRING_H

#include "EdgeString.h"

namespace Cayuga {
namespace AutomatonCompiler {

/**
 * @brief A FilterEdgeString object represents a filter edge constructed from 
		  SAX events.	
 * @see	EdgeString
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 27, 2005
 * @version 1.0
 */
class FilterEdgeString : public EdgeString {
public:
	/**
	* @brief Default constructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	FilterEdgeString ();

	/**
	* @brief Virtual destructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual ~FilterEdgeString();

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

}; //class FilterEdgeString
}  //namespace AutomatonCompiler
}  //namespace Cayuga

#endif //_FILTER_EDGE_STRING_H
