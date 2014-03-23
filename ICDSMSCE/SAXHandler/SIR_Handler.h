/**
 * @file SIR_Handler.h
 * @date November 17, 2005
 * @brief SIR_Handler reads new stream schemas from XML files.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _SIR_Handler_H
#define _SIR_Handler_H

#include "SimpleHandler.h"
#include "SystemMgr/StreamSchema.h"

namespace Cayuga {
namespace SystemMgr {
class StreamSchemaMgr;
}

namespace SAXHandler {
using Cayuga::SystemMgr::StreamSchemaMgr;
using Cayuga::SystemMgr::StreamSchema;

/**
 * @brief A SIR_Handler object reads stream schemas in XML format.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 17, 2005
 * @version 1.0
 */
class SIR_Handler : public SimpleHandler
{
public:
	/**
	* @brief Default constructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 17, 2005
	* @version 1.0
	*/
	SIR_Handler();
	
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

private:
	/**
	* This variable stores the pointer to the stream schema manager.
	*/
	StreamSchemaMgr* _pStreamSchemaMgr;
	
	/**
	* This variable stores the name of the current stream.
	*/
	std::string _streamName;
	
	/**
	* This variable stores the schema of the current stream.
	*/
	StreamSchema _tmpSchema;
}; //class SIR_Handler
}  //namespace SAXHandler
}  //namespace Cayuga

#endif //_SIR_Handler_H

