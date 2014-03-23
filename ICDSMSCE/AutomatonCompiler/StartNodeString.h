/**
 * @file StartNodeString.h
 * @date November 27, 2005
 * @brief Stores the information of a start node read from AIR.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _START_NODE_STRING_H
#define _START_NODE_STRING_H

#include "NodeString.h"

#include "Utils/StringMap.h"
using Cayuga::Utils::StringMap;

#include <vector>
#include <iostream>
#include <string>
#include <typeinfo>

using namespace std;

namespace Cayuga {
namespace SystemMgr {
class StreamSchema;
}
using Cayuga::SystemMgr::StreamSchema;

namespace AutomatonCompiler {
class FilterEdgeString;
class ForwardEdgeString;
class RebindEdgeString;

/**
 * @brief A StartNodeString object represents a start node constructed from 
		  SAX events.	
 * @see	NodeString
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 27, 2005
 * @version 1.0
 */
class StartNodeString : public NodeString {
public:
	typedef vector<ForwardEdgeString*> ForwardEdgeSet;

	/**
	* @brief Default constructor
	* @param attrMap Stores the attribute name, value pairs associated with the
					 SAX start element event of the node element in AIR. In
					 particular, NodeID is required to occur, and 
					 InputStreamName is optional.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	StartNodeString(const StringMap& attrMap);

	/**
	* @brief Virtual destructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual ~StartNodeString();

	/**
	* @brief Get the input stream name of the node.
	* @return The input stream name.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date September 12, 2006
	* @version 1.0
	*/
	virtual string getInputStreamName();

	/**
	* @brief Set the input stream name of the node.
	* @param input The input stream name to set to.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date September 17, 2006
	* @version 1.0
	*/
	virtual void setInputStreamName(const string& input);


	/**
	* @brief Compiles this node into Cayuga engine, by translating
			 strings into integers with symbol tables, and so on.
	* @param context The compilation context
	* @return Pointer to the internalized node.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual Node* compile(Context& context);

	/**
	* @brief Output member variable information in XML format, aiming at 
			 reconstructing the XML piece in AIR which encodes this node
	* @param out The output stream handle.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual void write(ostream& out = cout);

	/**
	* @brief Add an out-going edge to this node.
	* @param pEdge Pointer to the edge to add.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual void addEdge(EdgeString* pEdge);

//protected:
	/**
	* @brief Output member variable information common to all nodes in XML.
			 This is the first part, before the node schema
	* @param out The output stream handle.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual void writeCommonBeforeSchema(ostream& out = cout);

	/**
	* @brief Output member variable information common to all nodes in XML.
			 This is the second part, after the node schema.
	* @param out The output stream handle.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual void writeCommonAfterSchema(ostream& out = cout);

	/**
	* @brief Construct a node with the input sid. 
			 Refactored existing code to create this makeNode virtual function.
	* @param sid The input stream ID for the new node.
	* @return Point to the newly constructed node.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date September 11, 2006
	* @version 1.0
	*/
	virtual Node* makeNode(const StreamID sid);

	/**
	* This variable stores the input stream name of the node read from AIR.
	*/
	string _inputStream;

	/** 
	* This variable stores a set of at least one forward edges 
	*/
	ForwardEdgeSet _pForwards;
	
	/** 
	* This variable stores the optional filter edge. 
	*/
	FilterEdgeString* _pFilter;

	/** 
	* This variable stores the optional rebind edge. 
	*/
	RebindEdgeString* _pRebind;

}; //class StartNodeString
}  //namespace AutomatonCompiler
}  //namespace Cayuga

#endif //_START_NODE_STRING_H
