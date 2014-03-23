/**
 * @file QueryBuilder.cpp
 * @date November 22, 2005
 * @brief Implementation file of QueryBuilder class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "AutomatonCompiler/QueryBuilder.h"
#include "AutomatonCompiler/QueryString.h"
#include "AutomatonCompiler/NodeBuilder.h"
#include "AutomatonCompiler/WindowBuilder.h"
#include "AutomatonCompiler/predWindowBuilder.h"
using namespace Cayuga::AutomatonCompiler;

#include <string>
#include <fstream>
using namespace std;


QueryBuilder::QueryBuilder() {
	reset();

	_pNodeBuilder = new NodeBuilder();
	_pWindowBuilder=new WindowBuilder();
// 	//luoluo 7-28
// 	_pPredWindowBuilder = new predWindowBuilder;
}

QueryBuilder::~QueryBuilder() {
	delete _pNodeBuilder;
}

void QueryBuilder::reset() {
	_bInitialState = true;
	_pThisQ = NULL;
/*	_bInitialWindow = true;*/
}

QueryString* QueryBuilder::getQueryString() {

	QueryString* pQuery = _pThisQ;
	//reset();
	return pQuery;
}

void QueryBuilder::startElement(const std::string& namespaceURI, 
								const std::string& localName,
								const std::string& qName, 
								const AttributesString& atts) {
	try {
			//luoluo 8-3
			//print some informations about the parse of XML.
		    cout<<"start query builder---------->"<<localName<<endl;
			
			assert(localName.length());
			if (_bInitialState) {
				assert (localName == "NFA");
				_bInitialState = false;
				_pThisQ = new QueryString();
				return;
			}
			//Invoked by SAX event
			//@added by rainbow
			//@date 20111-4-8
			else if (localName=="Window")
			{
                  _pWindowBuilder->startElement(namespaceURI,localName,qName,atts);
			}
// 			//luoluo 7-28
// 			else if (localName =="PREDWINDOW")
// 			{
// 				_pPredWindowBuilder->startElement(namespaceURI,localName,qName,atts);
// 			}
			else {
				_pNodeBuilder->startElement(namespaceURI, localName, qName, atts);
			}	
	}
	catch (XMLException& e) {
		cerr<<e.getMessage();
	}
}

void QueryBuilder::endElement(const std::string& namespaceURI, 
							  const std::string& localName,
							  const std::string& qName) {
	assert(localName.length());

	//luoluo 8-3
	//print some informations about the parse of XML.
	cout<<"END QUERY BUILDER---------->"<<localName<<endl;

	if (localName == "Node") {
		NodeString* pNode = _pNodeBuilder->getNode();
		_pThisQ->addNode(pNode);
	}
	else if (localName=="Window")
	{
		  WindowString *pWindow=_pWindowBuilder->getWindow();
		  _pThisQ->_thisWindow=pWindow;
	}
// 	//luoluo 7-28
// 	else if(localName == "PREDWINDOW")
// 	{
// 		predWindowString * pPredWin = _pPredWindowBuilder->getPredWindow();
// 		_pThisQ->_thisPredWindow = pPredWin;
// 	}
	else
	{
		_pNodeBuilder->endElement(namespaceURI, localName, qName);
	}
}
