/**
 * @file CAXParser.cpp
 * @date November 17, 2005
 * @brief Implementation file of CaxParser class.
 * @note Previous file name is Doc_Handler.cpp
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifdef _MSC_VER
#  pragma warning(disable:4786)
#endif

#include "AutomatonCompiler/convertConstToByteArray.h"
using Cayuga::AutomatonCompiler::convertConstToByteArray;

#include "BasicDataStructure/Event/Event.h"
using namespace Cayuga::BasicDataStructure::EventNS;

#include "MemoryMgr/CY_IntStringFace.h"
using namespace CY_IntString;

#include "PriorityQueue/PriorityQueue.h"
using namespace Cayuga::PriorityQueue;

#include "BasicDataStructure/Window/inputWindow.h"

#include "SAXHandler/CAXParser.h"
#include "SAXHandler/SAXEventType.h"
using namespace Cayuga::SAXHandler;

#include "SystemMgr/DataTypeMgr.h"
#include "SystemMgr/StreamSchemaMgr.h"
using namespace Cayuga::SystemMgr;

#include <ctime>
#include <iostream>
#include <cassert>
using namespace std;



////////////////////////////////////////////////////////////////////////////////
// Definition of utility functions
////////////////////////////////////////////////////////////////////////////////

/**
* @brief Mix the two input parameters to create timestamp for each CAX event.
* @param prefix The prefix of the timestamp value.
* @param uniqueID The postfix of the timestamp value.
* @return The produced timestamp.
* @author Mingsheng Hong (mshong@cs.cornell.edu)
* @date November 17, 2005
* @version 1.0
*/
TimeStamp mixTime(const TimeStamp& prefix, size_t uniqueID) {
	//long long mixedTime = (prefix >> 32) | (uniqueID & 0xffffffff); 

	//for now ignore the prefix time part. only use uniqueID.
	return uniqueID; 
	
}

/**
* @brief Create the body of the CAX event and push it into priority queue.
* @param ev The event whose body is to be created.
* @param xmlElem The information in xmlElem is extracted to create the event 
*				 body.
* @author Mingsheng Hong (mshong@cs.cornell.edu)
* @date December 9, 2005
* @version 1.0
*/
void createInternalEvent(Event* ev, const CaxEventString& xmlElem) {
	StreamSchemaMgr* pStreamSchemaMgr = StreamSchemaMgr::getInstance();
	StreamID streamID = pStreamSchemaMgr->getStreamID("XML");
	const StreamSchema* pSchema = StreamSchemaMgr::getInstance()->
		getStreamSchema(streamID);

	char* evArray = ev->getBody();
	size_t offset = 0;

	//attribute Eid is a 4-byte unsigned integer
	string attrName = "Eid";
	AttributeID attrID = pSchema->getID(attrName);
	assert (attrID == 0);
	assert (pSchema->getInternalAttributeSize(attrID) == sizeof(unsigned));
	*(unsigned*)(evArray) = xmlElem.elementID;
	offset += sizeof(unsigned);
	
	//attribute elementID is a 4-byte SAXEventType instance (an SDT)
	attrName = "Type";
	attrID = pSchema->getID(attrName);
	assert (attrID == 1);
	assert (pSchema->getInternalAttributeSize(attrID) == sizeof(SAXEventType));
	AttrTypeID attrTypeID = pSchema->getAttributeType(attrID);
	string attrTypeName = DataTypeMgr::getInstance()->getName(attrTypeID);
	convertConstToByteArray(attrTypeName,
		xmlElem.type, 
		sizeof(SAXEventType), 
		evArray+offset,
		false,		//do not ping string constants from events
		true		//put strings into external space
		);
	offset += sizeof(SAXEventType);

	//attribute Tag is a variable length string.
	attrName = "Tag";
	attrID = pSchema->getID(attrName);
	assert (attrID == 2);
	assert (pSchema->getInternalAttributeSize(attrID) 
		== sizeof(IntStringHandle));
	IntStringHandle hTag = lookup(xmlElem.tag.c_str(), 1);
	*(IntStringHandle*)(evArray+offset) = hTag;
	offset += sizeof(IntStringHandle);

	//attribute Level is a 4-byte unsigned integer
	attrName = "Level";
	attrID = pSchema->getID(attrName);
	assert (attrID == 3);
	assert (pSchema->getInternalAttributeSize(attrID) == sizeof(unsigned));
	*(unsigned*)(evArray+offset) = xmlElem.level;
	offset += sizeof(unsigned);

	//attribute Data is a 4-byte integer
	///@warning This should later be changed into a UDT (or SDT) that is an
	///         an associate array.
	attrName = "Data";
	attrID = pSchema->getID(attrName);
	assert (attrID == 4);
	assert (pSchema->getInternalAttributeSize(attrID) == sizeof(int));
	attrTypeID = pSchema->getAttributeType(attrID);
	attrTypeName = DataTypeMgr::getInstance()->getName(attrTypeID);
	convertConstToByteArray(attrTypeName,
		xmlElem.data, 
		sizeof(int), 
		evArray+offset,
		false,
		true);
	offset += sizeof(int);

	//luoluo 5-25
/*	Cayuga::PriorityQueue::PriorityQueue::getInstance()->put(ev);*/
	inputWindow ::getInstance()->put(ev);
}

////////////////////////////////////////////////////////////////////////////////
// CaxParser member function definitions
////////////////////////////////////////////////////////////////////////////////

void CaxParser::reset() {
	assert (_elIDStack.empty());
	assert (_level == 0);

	///@bug The current CAX parser only works with one stream of documents,
	///		so the counters are not reset.
	//_caxIDCount = 0;
	//_elIDCount = 0;
}


CaxParser::CaxParser() : _elIDCount(0), _caxIDCount(0), _level(0) {
	reset();
}

Event* CaxParser::createCaxEventWithHeader(const CaxEventString& xmlElem) {
	size_t szSchema = 2*sizeof(CY_Heap::GHandle)+3*sizeof(unsigned);
	Event* ev = Event::allocEvent(szSchema);

	ev->setStartTime(mixTime(_docReceivedTime, _caxIDCount));
	++_caxIDCount;
	ev->setEndTime(ev->getStartTime());
	ev->setStreamID(StreamSchemaMgr::getInstance()->getStreamID("XML"));

	createInternalEvent(ev, xmlElem);
	return ev;
}

void CaxParser::startDocument()
{
	//std::cout << "startDocument" << std::endl;
	_docReceivedTime = time (NULL);
} // startDocument

void CaxParser::endDocument()
{
	//std::cout << "endDocument" << std::endl;
	reset();
} // endDocument

void CaxParser::startElement(const std::string& namespaceURI, 
							   const std::string& localName,
							   const std::string& qName, 
							   const AttributesString& atts)
{
	try {
		CaxEventString xmlElem;

		xmlElem.type = "START_ELEM";
		xmlElem.tag = localName;
		xmlElem.level = ++_level;

		xmlElem.elementID = _elIDCount++;
		_elIDStack.push(xmlElem.elementID);

		//SHORT CUT
		///@warning For now assume each element has at most one
		///attribute, and that attribute is named Data
		if (atts.getLength() > 0) {
			assert (atts.getLocalName(0) == "Data");
			xmlElem.data = atts.getValue(0);
		}

		createCaxEventWithHeader(xmlElem);
	}
	catch (XMLException& e) {
		cerr<<e.getMessage();
	}
} // startElement

void CaxParser::endElement(const std::string& namespaceURI, 
							 const std::string& localName,
							 const std::string& qName)
{
	//std::cout << "endElement: " << namespaceURI << ":" << localName
	//<< std::endl;

	CaxEventString xmlElem;

	xmlElem.type = "END_ELEM";
	xmlElem.tag = localName;
	xmlElem.level = _level--;

	xmlElem.elementID = _elIDStack.top();
	_elIDStack.pop();

	createCaxEventWithHeader(xmlElem);
} // endElement

void CaxParser::characters(const std::string& ch)
{
	//std::cout << "Characters: " << ch << std::endl;
	CaxEventString xmlElem;

	xmlElem.type = "CHUNK";
	xmlElem.tag = "";
	xmlElem.level = _level;
	
	//ignore carrage return character 0x10
	///@warning This is only relevant on Windows. May need to be revisited.
	char cCR = 0x10;
	if (!ch.find(cCR)) {
		xmlElem.data = ch;
	}
	else {
		size_t strSize = ch.size();
		if (strSize != 1) {
			cerr<<"unexpected situation"<<endl;
			cerr<<"characters is "<<ch<<endl;
			cerr<<"size is "<<strSize<<endl;
			assert (false);
		}
	}

	xmlElem.elementID = _elIDStack.top();

	createCaxEventWithHeader(xmlElem);
} // characters

// end of file
