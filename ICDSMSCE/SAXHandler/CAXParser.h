/**
 * @file CAXParser.h
 * @date November 17, 2005
 * @brief Read XML documents and relationalize the SAX events to feed
		  into Cayuga.
 * @note Previous file name is Doc_Handler.h
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _CAX_PARSER_H
#define _CAX_PARSER_H

#include "SimpleHandler.h"

#include "BasicDataStructure/Timestamp.h"
using Cayuga::BasicDataStructure::TimeStamp;

#include <stack>
#include <fstream>
#include <string>

using namespace std;

namespace Cayuga {
namespace BasicDataStructure {
namespace EventNS {
class Event;
}
}

namespace SAXHandler {
using Cayuga::BasicDataStructure::EventNS::Event;

/**
 * @brief A CaxEventString object represents a relationalized but not yet 
 *        internalized SAX event.
 *		
 * The schema of Each CAX event consists of the follow sequence of 5 attributes.
 *
 * @see	CaxParser
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 17, 2005
 * @version 1.0
 */
struct CaxEventString {
	/**
	* This variable stores the pre-order traversal ID assigned by a CAX parser.
	*/
	unsigned elementID;

	/**
	* This variable stores the type of the CAX event in string format. When the
	* event is internalized, it will be converted into a enumeration value of 
	* enum SAXHandler::SAXEventType.
	* @see SAXEventType
	*/
	string type;

	/**
	* This variable stores the tag name of the XML element.
	*/
	string tag;

	/**
	* This variable stores the level of the current XML element in the document.
	*/
	unsigned level;

	/**
	* This variable is organized as an associated array, and stores attribute
	* name/value pairs for START_ELEM events, or the text chunk data for CHUNK
	* events.
	*/
	string data;
};

/**
 * @brief A CaxParser object represents a CAX parser.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 17, 2005
 * @version 1.0
 */
class CaxParser : public SimpleHandler
{
public:
	/**
	* @brief Default constructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 17, 2005
	* @version 1.0
	*/
	CaxParser();

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

	/**
	* @brief Call back function for SAX parser when a characters event is 
	         produced.
	* @param ch The string value of the characters event
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 17, 2005
	* @version 1.0
	*/
	void characters(const std::string& ch);

	/**
	* @brief Reset the state of this parser.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 17, 2005
	* @version 1.0
	*/
	void reset();

	/**
	* @brief Create a CAX event and put it in the priority queue.
	* @param xmlElem It contains the information of the CAX event, such as
	*		         the tag name string.
	* @return The pointer to the constructed event.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date December 9, 2005
	* @version 1.0
	*/
	Event* createCaxEventWithHeader(const CaxEventString& xmlElem);
private:

	/**
	* This variable stores the pre-order id counter of elements in XML document.
	* @note A characters event has the same id as that of its parent 
	* element, and an end element event has the same id as that of the 
	* start element event associated with the same element
	*/
	unsigned _elIDCount;

	/**
	* This variable stores the CAX event id counter. It is incremented for each 
	* CAX parser event.
	*/
	unsigned _caxIDCount;

	/**
	* This variable stores a stack of IDs of the elements in CAX parser's 
	* pre-order traversal stack.
	*/
	std::stack<unsigned> _elIDStack;

	/** 
	* This variable maintains the level information of the current CAX event.
	* Level starts at 1 for root element in each document.
	*/
	unsigned _level;

	/** 
	* This variable stores the received time of the current XML document.
	* It is used along with CaxParser::_caxIDCount to assign the timestamp
	* for each CAX event.
	*/
	TimeStamp _docReceivedTime;
}; //class CaxParser
}  //namespace SAXHandler
}  //namespace Cayuga

#endif //_CAX_PARSER_H

