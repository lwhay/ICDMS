/**
 * @file ConfigMgr.cpp
 * @date December 8, 2005
 * @brief Implementation file of ConfigMgr class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifdef _MSC_VER
#  pragma warning(disable:4786)
#endif

#include <iostream>
#include <string>
using namespace std;

#include "SAXHandler/ConfigMgr.h"
using namespace Cayuga::SAXHandler;

#include "SystemMgr/Options.h"
using namespace Cayuga::SystemMgr;

#include "Utils/AttrMap.h"
using namespace Cayuga::Utils;

ConfigMgr::ConfigMgr() {
	_options = new Options();
}

ConfigMgr::~ConfigMgr() {
	delete _options;
}

////////////////////////////////////////////////////
// ContentHandler
void ConfigMgr::startDocument()
{
	//std::cout << "startDocument" << std::endl;
} // startDocument

void ConfigMgr::endDocument()
{
	//std::cout << "endDocument" << std::endl;
	_options->setOptions(_optionMap);

} // endDocument

void ConfigMgr::startElement(const std::string& namespaceURI, 
							 const std::string& localName,
							 const std::string& qName, 
							 const AttributesString& atts)
{
	try {
		assert(localName.length());

		//root element
		if (localName == "Config") {
			return;
		}

		assert (localName == "Option");
		StringMap attrMap;
		createAttrMap(atts, attrMap);

		string optionName = getParameterValue<string>(attrMap, "Name");
		string optionValue = getParameterValue<string>(attrMap, "Value");

		_optionMap.insert(make_pair(optionName, optionValue));
	}
	catch (XMLException& e) {
		cerr<<e.getMessage();
	}
} // startElement

const Options* ConfigMgr::getOptions() {
	return _options;
}


// end of file

