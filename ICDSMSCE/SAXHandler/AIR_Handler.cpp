/**
 * @file AIR_Handler.cpp
 * @date November 17, 2005
 * @brief Implementation file of AIR_Handler class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifdef _MSC_VER
#  pragma warning(disable:4786)
#endif

#include <iostream>
#include <fstream>

#include "SAXHandler/AIR_Handler.h"
#include "SAXHandler/ConfigMgr.h"
using namespace Cayuga::SAXHandler;

#include "AutomatonCompiler/QueryString.h"
#include "AutomatonCompiler/QueryBuilder.h"

#include "BasicDataStructure/Automaton/Query.h"
using namespace Cayuga::BasicDataStructure::Automaton;

AIR_Handler::AIR_Handler(Query& existingQs) 
: _existingQs(existingQs) {
	_qBuilder = new QueryBuilder();
}

AIR_Handler::~AIR_Handler() {
	delete _qBuilder;
}

////////////////////////////////////////////////////
// ContentHandler
void AIR_Handler::startDocument()
{
	//std::cout << "startDocument" << std::endl;
} // startDocument

void AIR_Handler::endDocument()
{
	//std::cout << "endDocument" << std::endl;
	QueryString* thisQString = _qBuilder->getQueryString();
	
	//thisQString->write();
	Query * thisQ;
	try {
		thisQ = thisQString->compile();
		delete thisQString;
		_qBuilder->reset();
	}
	catch (string &s){
		cout << "Query compilation failed!" <<endl;
		delete thisQString;  // not sure these two lines are necessary...
		_qBuilder->reset();
		throw (s);
	}
	
//#ifdef _DEBUG
	//ostream& out = cout;
	//if (ConfigMgr::getInstance()->getOptions()->_bVerbose) {
	//	//ofstream out("log.txt", ios::app);
	//	//out<<"trying to merge query "<<endl;
	//	thisQ->write(out);
	//	//out<<endl;
	//}
//#endif //_DEBUG

	//merge _existingQs with thisQ
	_existingQs.mergeQuery(thisQ);

//#ifdef _DEBUG
	//if (ConfigMgr::getInstance()->getOptions()->_bVerbose) {
	//	out<<endl<<"the result is "<<endl;
	//	_existingQs.write(out);
	//	out<<endl;
	//}
//#endif //_DEBUG
} // endDocument

void AIR_Handler::startElement(const std::string& namespaceURI, 
							   const std::string& localName,
							   const std::string& qName, 
							   const AttributesString& atts)
{
	_qBuilder->startElement(namespaceURI, localName, qName, atts);
} // startElement

void AIR_Handler::endElement(const std::string& namespaceURI, 
							 const std::string& localName,
							 const std::string& qName)
{
	_qBuilder->endElement(namespaceURI, localName, qName);
} // endElement
