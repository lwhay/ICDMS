/**
 * @file EdgeBuilder.h
 * @date November 27, 2005
 * @brief Responsible of building an automaton edge from SAX events.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */


#ifndef _EDGE_BUILDER_H
#define _EDGE_BUILDER_H

#include "BasicDataStructure/Automaton/NodeID.h"
using Cayuga::BasicDataStructure::Automaton::NodeID;

#include "SAXHandler/SimpleHandler.h"
using namespace Cayuga::SAXHandler;

#include "Utils/StringMap.h"
using namespace Cayuga::Utils;

#include <string>
using namespace std;

namespace Cayuga {
namespace AutomatonCompiler {
class EdgeString;
class Predicates_DNFString;
class ConjunctiveClauseString;
class AtomPredString;
class OldNewAttrPair;

/**
 * @brief An EdgeBuilder object builds automaton edges from SAX events.	
 * @see	NodeBuilder
 * @see QueryBuilder
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 27, 2005
 * @version 1.0
 */
class EdgeBuilder : public SimpleHandler {
public:
	/**
	* @brief Default constructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	EdgeBuilder();

	void startElement(const std::string& namespaceURI, 
		const std::string& localName,
		const std::string& qName, 
		const AttributesString& atts);

	void endElement(const std::string& namespaceURI, 
		const std::string& localName,
		const std::string& qName);

	/**
	* @brief Return the constructed edge to the caller. Reset the state of this
			 edge builder at this call.
	* @return Pointer to the constructed edge.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	EdgeString* getEdge();

private:
	/**
	* @brief Create a new edge at the SAX start element event of an XML element
			 encoding an automaton edge.
	* @param edgeType Edge type represented in a string. Possible values are
					  "FilterEdge", "RebindEdge" and "ForwardEdge"
	* @param attrMap Stores the attribute name/value pairs asssociated with the
					 XML element. Specifically, for a forward edge element, 
					"Destination" is an attributed required to occur.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	void createEdge(const string& edgeType, const StringMap& attrMap);

	/**
	* @brief Return the type of the current edge being constructed.
	* @return as above.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	const type_info& getEdgeType();

	/**
	* @brief Set the destination node ID of the current edge being constructed
			 to nid.
	* @invariant The current edge must be a forward edge. 
	* @invariant This function can only be called once for each forward edge. 
	* @param nid Local node ID (node ID in the AIR format)
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	void setDestNodeID(NodeID nid);

	/**
	* @brief Set the DNF of the current edge being constructed to the DNF that
			 we just finished constructing.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	void setDNF();

	/**
	* @brief Create a new DNF of edge predicates.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	void createNewDNF();

	/**
	* @brief Create a new conjunctive predicate clause for the DNF.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	void createNewClause();

	/**
	* @brief Add the conjunctive predicate clause whose construction is just 
		     finished to the current DNF being constructed.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	void addClause();

	/**
	* @brief Add an atomic predicate that has just been constructed to the 
			 current conjunctive clause being constructed.
	* @param pAtomPred Pointer to the atomic predicate to add.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	void addAtomPred(AtomPredString* pAtomPred);

	/**
	* @brief Add an old, new attribute pair for constructing the schema map of
			 the current edge being constructed.
	* @invariant The current edge must be a rebind or forward edge. 
	* @param attrPair Reference to the old, new attribute pair.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	void addOldNewAttrPair(const OldNewAttrPair& attrPair);

	/**
	* @brief Reset any book-keeping (state) information of this edge builder.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	void reset();

	/**
	* This variable stores the state of this edge builder w.r.t.\ which XML 
	* element are we at when reading the AIR xml file.

	* The parser is required to keep its state since we are using a push-based
	* XML parser (SAX parser), and thus the builder is driven by the parser. If
	* we are to use an application-driven parser instead (such as th StAX Java 
	* parser or the C# XMLReader parser, this can be avoided. 
	*/
	enum {
		INIT,
		IN_EDGE, 
		IN_EVAL_FUNC, 
		IN_SCHEMA_MAP, 
		IN_DESTINATION,
		IN_PRED_CLAUSE,
		IN_UDA
	} _state;

	/**
	* This variable stores the pointer to the current edge being constructed.
	*/
	EdgeString* _pThisEdge;
	
	/**
	* This variable stores the destination node ID of the current edge being 
	* constructed. Its value is BAD_NODE_ID if the current edge is not a foward
	* edge.
	*/	
	NodeID _destNodeID;
	
	/**
	* This variable stores DNF edge predicate of the current edge being 
	* constructed.
	*/
	Predicates_DNFString* _pDNF;
	
	/**
	* This variable stores the conjunctive predicate clause of the current DNF 
	* edge predicate being constructed.
	*/
	ConjunctiveClauseString* _pClause;
}; //class EdgeBuilder
}  //namespace AutomatonCompiler
}  //namespace Cayuga

#endif //_EDGE_BUILDER_H
