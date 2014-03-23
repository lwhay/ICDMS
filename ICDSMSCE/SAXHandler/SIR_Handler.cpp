/**
 * @file SIR_Handler.cpp
 * @date November 17, 2005
 * @brief Implementation file of SIR_Handler class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifdef _MSC_VER
#  pragma warning(disable:4786)
#endif

#include "SAXHandler/SIR_Handler.h"
using namespace Cayuga::SAXHandler;

#include "CayugaSystem.h"
using Cayuga::CayugaSystem;

#include "SystemMgr/StreamSchemaMgr.h"

#include <iostream>


SIR_Handler::SIR_Handler() {
	_pStreamSchemaMgr = StreamSchemaMgr::getInstance();
}


////////////////////////////////////////////////////
// ContentHandler
void SIR_Handler::startDocument()
{
	//std::cout << "startDocument" << std::endl;
} // startDocument

void SIR_Handler::endDocument()
{
	//std::cout << "endDocument" << std::endl;

	CayugaSystem::getInstance()->debugOstream()<<"new stream schema "
		<<_streamName<<" inserted"<<endl;
	_pStreamSchemaMgr->insertNewSchema(_streamName, _tmpSchema);

	_pStreamSchemaMgr->write(CayugaSystem::getInstance()->debugOstream());
	_tmpSchema.clear();
} // endDocument

void SIR_Handler::startElement(const std::string& namespaceURI, 
							   const std::string& localName,
							   const std::string& qName, 
							   const AttributesString& atts)
{
	try {
		assert(localName.length());
		if (localName == "StreamType") {
			//assert (atts.getLocalName(0) == "Name");
			if (atts.getLocalName(0) == "Name") {
				_streamName = atts.getValue(0);
			}
			else if (atts.getLocalName(1) == "Name") {
				_streamName = atts.getValue(1);
			}
			else {
				assert (false);
			}
		}
		else if (localName == "AttrNameType") {
			assert (atts.getLocalName(0) == "Name");
			string attrName = atts.getValue(0);
			assert (atts.getLocalName(1) == "Type");
			string attrType = atts.getValue(1);
			_tmpSchema.putAttributeAtEnd(attrName, attrType);
		}
	}
	catch (XMLException& e) {
		cerr<<e.getMessage();
	}
} // startElement

// end of file
