/**
 * @file AIR_Handler.h
 * @date November 17, 2005
 * @brief AIR_Handler reads an XML file of AIR format, and invokes QueryBuilder 
          to build the query encoded in that file.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */


#ifndef _AIR_Handler_H
#define _AIR_Handler_H

#include "SimpleHandler.h"

namespace Cayuga {

namespace AutomatonCompiler {
	class QueryBuilder;
}
using namespace Cayuga::AutomatonCompiler;

namespace BasicDataStructure {
namespace Automaton {
class Query;
}
}
using namespace Cayuga::BasicDataStructure::Automaton;

namespace SAXHandler {
/**
 * @brief An AIR_Handler object reads automata in AIR format.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 17, 2005
 * @version 1.0
 */
class AIR_Handler : public SimpleHandler
{
public:
	/**
	* @brief Default constructor
	* @param existingQs The query DAG to insert queries into
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 17, 2005
	* @version 1.0
	*/
	AIR_Handler(Query& existingQs);
	
	/**
	* @brief Destructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date September 21, 2006
	* @version 1.0
	*/
	~AIR_Handler();

	/**
	* @brief Call back function for SAX parser when a start document event is 
	         produced.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 17, 2005
	* @version 1.0
	*/
	void startDocument();
	
	/**
	* @brief Call back function for SAX parser when a start document event is 
	         produced.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 17, 2005
	* @version 1.0
	*/
	void endDocument();
	
	/**
	* @brief Call back function for SAX parser when a start element event is 
	         produced.
	* @param namespaceURI The Namespace URI, or the empty string if the element 
	                      has no Namespace URI or if Namespace processing is not 
						  being performed
	* @param localName The local name (without prefix), or the empty string if 
	                   Namespace processing is not being performed
	* @param qName The qualified name (with prefix), or the empty string if 
	               qualified names are not available
	* @param atts Attribute name, value pairs associated with the start tag
	              of this element.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 17, 2005
	* @version 1.0
	*/
	void startElement(const std::string& namespaceURI, 
		const std::string& localName,
		const std::string& qName, 
		const AttributesString& atts);
	
	/**
	* @brief Call back function for SAX parser when an end element event is 
	         produced.
	* @param namespaceURI The Namespace URI, or the empty string if the element 
	                      has no Namespace URI or if Namespace processing is not 
						  being performed
	* @param localName The local name (without prefix), or the empty string if 
	                   Namespace processing is not being performed
	* @param qName The qualified name (with prefix), or the empty string if 
	               qualified names are not available
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 17, 2005
	* @version 1.0
	*/
	void endElement(const std::string& namespaceURI, 
		const std::string& localName,
		const std::string& qName);

private:
	/**
	* This variable stores the query DAG for new queries read from AIR to insert
	* into.
	*/
	Query& _existingQs;
	
	/**
	* This variable stores the query builder for AIR_Handler to invoke.
	*/
	QueryBuilder* _qBuilder;
}; //class AIR_Handler
}  //namespace SAXHandler
}  //namespace Cayuga

#endif //_AIR_Handler_H

