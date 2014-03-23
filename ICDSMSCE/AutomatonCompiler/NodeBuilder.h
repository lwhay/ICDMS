/**
 * @file NodeBuilder.h
 * @date November 22, 2005
 * @brief Responsible of building an automaton node from SAX events.

   It invokes EdgeBuilder to build out-going edges associated with this node.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */


#ifndef _NODE_BUILDER_H
#define _NODE_BUILDER_H

#include "AutomatonCompiler/EdgeBuilder.h"
#include "AutomatonCompiler/PredIndexSpecString.h"

#include "BasicDataStructure/Automaton/EdgeType.h"
using Cayuga::BasicDataStructure::Automaton::EdgeType;

#include "SAXHandler/SimpleHandler.h"
#include "AutomatonCompiler/predWindowBuilder.h"
#include "predWindowString.h"

#include <string>

namespace Cayuga {
namespace AutomatonCompiler {
class NodeString;

/**
 * @brief A NodeBuilder object builds automaton nodes from SAX events.	
 * @see	EdgeBuilder
 * @see QueryBuilder
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 22, 2005
 * @version 1.0
 */
class NodeBuilder : public SimpleHandler {

	/**
	* @todo : Move this to another header file IndexType.h
	*/
	enum IndexType {STATIC, DYNAMIC};
public:
	/**
	* @brief Default constructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 22, 2005
	* @version 1.0
	*/
	NodeBuilder();

	void startElement(const std::string& namespaceURI, 
		const std::string& localName,
		const std::string& qName, 
		const AttributesString& atts);

	void endElement(const std::string& namespaceURI, 
		const std::string& localName,
		const std::string& qName);
	
	/**
	* @brief Return the constructed node to the caller. Reset the state of this
			 node builder at this call.
	* @return Pointer to the constructed node.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 22, 2005
	* @version 1.0
	*/
	NodeString* getNode();


private:
	/**
	* @brief Create a new node at the SAX start element event of an XML element
			 encoding an automaton node.
	* @param attrMap Stores the attribute name/value pairs asssociated with the
					 XML element. Specifically, for each node element, 
					"NodeType" is an attributed required to occur, which can 
					take the value of "START" or "INTERMEDIATE_OR_END"
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 22, 2005
	* @version 1.0
	*/
	void createNode(const StringMap& attrMap);

	/**
	* @brief Add the edge whose construction is just 
		     finished to the current node being constructed.
    * @param pEdge Pointer to the edge being added.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 22, 2005
	* @version 1.0
	*/
	void addEdge(EdgeString* pEdge);

	/**
	* @brief Add The output stream name to the current node being constructed.
    * @param streamName Name of the output stream being added.
	* @param isOut Whether the stream is to be output to Disc/Network or not.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 22, 2005
	* @version 1.0
	*/
	void addOutputStream(const string& streamName, const bool isOut);

	/**
	* @brief Add an attribute name,type pair to schema of the current node 
			 being constructed.
    * @param attrName Name of the attribute being added.
    * @param attrType Type of the attribute being added.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 22, 2005
	* @version 1.0
	*/
	void addSchemaAttr(const string& attrName,
		const string& attrType);

	/**
	* @brief Add an entry of the predicate index specification to the current 
	         predicate index specification being constructed.
    * @param indexType Type of this index, which can take value of either 
					   "STATIC" or "DYNAMIC"
    * @param attr Name of the attribute in the event, indicating which event 
	              binding in the event to extract as the input value for index 
				  probing.
    * @param nodeAttr Name of the node attribute to index, of this is a dynamic
					  index.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 22, 2005
	* @version 1.0
	*/
	void addPredIndexSpecEntry(const string& indexType,
		const string&  attr,
		const string&  nodeAttr);

	/**
	* @brief Add a predicate index specification that has just been constructed
			 to filter predicate spec or FR predicate spec of the current node 
			 being constructed.
    * @param eType Indicates whether this predicate index works on filter edge
	               or FR edges. 
				   
				   enum EdgeType is borrowsed for this use. Its value is 
				   EdgeType::FILTER if this index works on filter predicates, 
				   and EdgeType::FORWARD otherwise.
	* @param iType Indicates the type of the predicate. 
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 22, 2005
	* @version 1.0
	*/
	void addOnePredIndexSpec(const EdgeType& eType, const IndexType& iType);

	/**
	* @brief Set the Index Specs of the Node String being constructed
    * @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 22, 2005
	* @version 1.0
	*/
	void addPredIndexSpec();

	/**
	* @brief Reset any book-keeping (state) information of this node builder.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 22, 2005
	* @version 1.0
	*/
	void reset();

	/**
	* This variable stores the state of this node builder w.r.t.\ which XML 
	* element are we at when reading the AIR xml file.

	* See comment of EdgeBuilder::_state for more details.
	*/
	enum {
		INIT,
		IN_NODE, 
		IN_SCHEMA, 
		IN_EDGE,
		IN_FILTER_INDEX,
		IN_DYN_FILTER_INDEX,
		IN_FR_INDEX
	} _state;

	/**
	* This variable stores the edge builder driven by this node builder.
	*/
	EdgeBuilder _eBuilder;

	/**
	* This variable stores the pointer to the current node being constructed.
	*/
	NodeString* _pThisNode;

	//luoluo 8-4
	/**
	*@brief		Thid variable stores the pointer to the predwindowbuilder driven by
					this node builder ,and then will be added into _pThisNode.
	*/
	predWindowBuilder * _PredWindowBuilder;
	/**
	* This variable stores the filter predicate index specs of the current node 
	* being constructed.
	*/
	vector<PredIndexSpecString> _filterPredIndices;
	
	/**
	* This variable stores the FR predicate index specs of the current node 
	* being constructed.
	*/
	vector<PredIndexSpecString> _frPredIndices;

	/**
	* This variable stores the Dynamic AI Filter predicate index spec (only one
	* Index according to Policy Decision) of the current node being constructed.
	*/
	PredIndexSpecString _dynFilterPredIndex;
	
	/**
	* This variable stores the current predicate index spec being constructed.
	*/
	PredIndexSpecString _predIndexSpec;

	/**
	* This variable stores whether there is a dynamic predicate index on the
	* node
	*/
	bool _isDynIndexed;

}; //class NodeBuilder
}  //namespace AutomatonCompiler
}  //namespace Cayuga

#endif //_NODE_BUILDER_H
