/**
 * @file ForwardEdgeString.h
 * @date November 27, 2005
 * @brief Stores the information of a forward edge read from AIR.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _FORWARD_EDGE_STRING_H
#define _FORWARD_EDGE_STRING_H

#include "RebindEdgeString.h"

#include "BasicDataStructure/Automaton/NodeID.h"
using Cayuga::BasicDataStructure::Automaton::NodeID;

#include "BasicDataStructure/StreamID.h"
using Cayuga::BasicDataStructure::StreamID;

namespace Cayuga {
namespace AutomatonCompiler {
/**
 * @brief A ForwardEdgeString object represents a forward edge constructed from 
		  SAX events.	
 * @see	EdgeString
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 27, 2005
 * @version 1.0
 */
class ForwardEdgeString : public RebindEdgeString {
public:
	/**
	* @brief Default constructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	ForwardEdgeString ();

	/**
	* @brief Virtual destructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual ~ForwardEdgeString();

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
	* @brief Set the destination node ID of this forward edge.
	* @param destNodeID The destination node ID to be set
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	void setDestNodeID(NodeID destNodeID);

private:
	/** 
	* This variable stores the destination node ID read from AIR.
	* 
	* Since this ID is local to the current query, it will be replaced with 
	* appropriate node pointer value once the query is internalized in 
	* Cayuga engine. 
	*/
	NodeID _destNodeID;
}; //class ForwardEdgeString
}  //namespace AutomatonCompiler
}  //namespace Cayuga

#endif //_FORWARD_EDGE_STRING_H
