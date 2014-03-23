/**
 * @file SimpleHandler.cpp
 * @date May 5, 2006
 * @brief Definition of SimpleHandler.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "SAXHandler/SimpleHandler.h"

#include <string>
using namespace std;

using namespace Cayuga::SAXHandler;

SimpleHandler::SimpleHandler() : 
	SAX2PrintHandlers("LATIN1", XMLFormatter::UnRep_CharRef, false) {
}

SimpleHandler::~SimpleHandler() {
}

void SimpleHandler::startElement(const std::string& namespaceURI, 
								 const std::string& localName,
								 const std::string& qName, 
								 const AttributesString& atts) {
}

void SimpleHandler::endElement(const std::string& namespaceURI, 
							   const std::string& localName,
							   const std::string& qName) {
}

void SimpleHandler::startElement(const   XMLCh* const    uri,
									const   XMLCh* const    localname,
									const   XMLCh* const    qname,
                                    const   Attributes&		attributes) {
	const string namespaceURI(XMLString::transcode(uri));
	const string localName(XMLString::transcode(localname));
	const string qName(XMLString::transcode(qname));
	startElement(namespaceURI, localName, qName, AttributesString(attributes));
}


void SimpleHandler::endElement(const XMLCh* const uri,
								 const XMLCh* const localname,
								 const XMLCh* const qname) {
    const string namespaceURI(XMLString::transcode(uri));
	const string localName(XMLString::transcode(localname));
	const string qName(XMLString::transcode(qname));
	endElement(namespaceURI, localName, qName);
}

void SimpleHandler::createAttrMap(const AttributesString& atts, 
								  hash_map<std::string, std::string>& attrMap) {
	attrMap.clear();
	int numAttrs = atts.getLength();
	for (int i = 0; i < numAttrs; ++i) {
		attrMap.insert(make_pair(atts.getLocalName(i), atts.getValue(i)));
	}
}
