/**
 * @file QueryBuilder.h
 * @date November 22, 2005
 * @brief Responsible of building a query automaton from SAX events.

   It invokes NodeBuilder to build automaton nodes associated with this query.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _QUERY_BUILDER_H
#define _QUERY_BUILDER_H

#include <string>

#include "SAXHandler/SimpleHandler.h"
using namespace Cayuga::SAXHandler;

namespace Cayuga {
//forward declaration
namespace BasicDataStructure {
namespace Automaton {
class Query;
}
}
using Cayuga::BasicDataStructure::Automaton::Query;

namespace AutomatonCompiler {
class QueryString;
class NodeBuilder;
class WindowBuilder;
// luoluo 7-28
// class predWindowBuilder;

/**
 * @brief A QueryBuilder object builds query automata from SAX events.	
 * @see NodeBuilder
 * @see	EdgeBuilder
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 22, 2005
 * @version 1.0
 */
class QueryBuilder : public SimpleHandler {
public:
	/**
	* @brief Default constructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 22, 2005
	* @version 1.0
	*/
	QueryBuilder();

	/**
	* @brief Destructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 22, 2007
	* @version 1.0
	*/
	~QueryBuilder();

	void startElement(const std::string& namespaceURI, 
		const std::string& localName,
		const std::string& qName, 
		const AttributesString& atts);

	void endElement(const std::string& namespaceURI, 
		const std::string& localName,
		const std::string& qName);
	
	/**
	* @brief Return the query in string format to the caller. The caller does
			 not own this query. To own it, reset() should be explicitly called 
			 to reset any book-keeping information.
	* @return Pointer to the query string.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date Feburary 16, 2006
	* @version 1.0
	*/
	QueryString* getQueryString();

	/**
	* @brief Reset any book-keeping (state) information of this query builder.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 22, 2005
	* @version 1.0
	*/
	void reset();

private:

	/**
	* This variable stores the pointer to the current query being constructed.
	*/
	QueryString* _pThisQ;
	
	
	/**
	* This variable stores the node builder driven by this query builder.
	*/
	NodeBuilder* _pNodeBuilder;

	/**
	* This variable stores the Window builder driven by this query builder.
	*@added by rainbow
	*@date 20011-4-8
	*/
	WindowBuilder* _pWindowBuilder;

// luoluo 7-28
// 		predWindowBuilder * _pPredWindowBuilder;
/*	bool _bInitialPredWin;*/

	/**
	* This variable stores the state of this query builder w.r.t.\ which XML 
	* element are we at when reading the AIR xml file.
	*/
	bool _bInitialState;

// 	/************************************************************************/
// 	/*  
// 	*@brief This variable identify whether the information have being store in 
// 	*windowstring from XML
// 	*@ author rainbow
// 	*@date 2011-4-8
// 	*/
// 	/************************************************************************/
    bool  _bInitialWindow;

}; //class QueryBuilder
}  //namespace AutomatonCompiler
}  //namespace Cayuga

#endif //_QUERY_BUILDER_H
