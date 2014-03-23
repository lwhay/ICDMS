/**
 * @file NodeString.h
 * @date November 27, 2005
 * @brief Base class of all automaton nodes read from AIR.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _NODE_STRING_H
#define _NODE_STRING_H

#include "AutomatonCompiler/PredIndexSpecString.h"

#include "BasicDataStructure/Automaton/NodeID.h"
using Cayuga::BasicDataStructure::Automaton::NodeID;

#include "predWindowString.h"

#include <iostream>
#include <vector>
using namespace std;

namespace Cayuga {
namespace BasicDataStructure {
namespace Automaton {
class Node;
}
}

namespace AutomatonCompiler {
using Cayuga::BasicDataStructure::Automaton::Node;
class EdgeString;
class Context;

/**
 * @brief A NodeString object represents an automaton node constructed from 
		  SAX events.	
 * @see	EdgeString
 * @see QueryString
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 27, 2005
 * @version 1.0
 */
class NodeString {
public:
	typedef vector<PredIndexSpecString> PredIndexSpecSet;
	
	/**
	* @brief Default constructor
	* @param nodeType type of the node read from AIR
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	NodeString(const string& nodeType);

	/**
	* @brief Virtual destructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual ~NodeString();
	
	/**
	* @brief Compiles this node into Cayuga engine, by translating
			 strings into integers with symbol tables, and so on.
	* @param context The compilation context
	* @return Pointer to the compiled node.
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
	virtual void write(ostream& out = cout) = 0;

	/**
	* @brief Add an out-going edge to this node.
	* @param pEdge Pointer to the edge to add.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual void addEdge(EdgeString* pEdge) = 0;

	/**
	* @brief Add an attribute name, type pair to the schema of this node.
	* @param attrName Name of the attribute.
	* @param attrType Type of the attribute.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual void addSchemaAttr(const string& attrName,
		const string& attrType);

	/**
	* @brief Add an output stream to this node.
	* @param outputStream Name of the output stream.
	* @param isOut whether the stream is to be output to disc/network.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual void addOutputStream(const string& outputStream, const
								bool isOut);

	/**
	* @brief Add predicate index specs to this node.
	* @param filterPredIndices Filter predicate index specs
	* @param frPredIndices FR predicate index specs
	* @param dynFilterPredIndex Dynamic Filter Index Specs
	* @param isDynIndexed Whether there is Dynamic Indexing on the Node
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual void addPredIndexSpec(const PredIndexSpecSet& filterPredIndices,
		const PredIndexSpecSet& frPredIndices, const PredIndexSpecString&
		dynFilterPredIndex, const bool& isDynIndexed);

	/**
	* @brief Return the type of this node read from AIR.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	string getNodeType();

	/**
	* @brief Return the Id of this node.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date Semptember 17, 2006
	* @version 1.0
	*/
	NodeID getNodeID();

	//luoluo 8-4
	predWindowString * _predwind;
	void setRationalPredWindow(predWindowString * ptr)
	{
		_predwind = ptr;
	}
	predWindowString * getPredWind()
	{
		return _predwind;
	}

protected:
	/**
	* This variable stores the ID of this node read from AIR.
	*/
	NodeID _nodeID;

	/**
	* This variable stores the type of this node read from AIR.
	*/
	string _nodeType;

	/**
	* This variable stores filter predicate index specs.
	*/
	PredIndexSpecSet _filterPredIndices;
	
	/**
	* This variable stores FR predicate index specs.
	*/
	PredIndexSpecSet _frPredIndices;

	/**
	* This variable stores the Dynamic AI Filter index specs.
	*/
	PredIndexSpecString _dynFilterPredIndex;

	/**
	* This variable stores whether there is a dynamic predicate index
	* on this node
	*/
	bool _isDynIndexed;

// 	//luoluo 8-4
// 	predWindowString _predWindowString;


}; //class NodeString
}  //namespace AutomatonCompiler
}  //namespace Cayuga

#endif //_NODE_STRING_H
