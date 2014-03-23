/**
 * @file ConfigMgr.h
 * @date December 8, 2005
 * @brief ConfigMgr reads system configuration options from an XML file.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _CONFIG_MGR_H
#define _CONFIG_MGR_H

#include "SimpleHandler.h"

#include "Utils/Singleton.h"
#include "Utils/StringMap.h"

namespace Cayuga {

namespace SystemMgr {
class Options;
}

using Cayuga::SystemMgr::Options;

namespace SAXHandler {
using Cayuga::Utils::StringMap;
using Cayuga::Utils::Singleton;

class ConfigMgr;
/**
 * @brief A ConfigMgr object reads system configuration options in XML format.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date December 8, 2005
 * @version 1.0
 */
class ConfigMgr : public SimpleHandler, 
	public Singleton<ConfigMgr>
{
public:
	/**
	* @brief Default constructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date December 8, 2005
	* @version 1.0
	*/
	ConfigMgr();
	
	/**
	* @brief Destructor.
			 We destroy Options member field here.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date Octorber 20, 2006
	* @version 1.0
	*/
	~ConfigMgr();

	/**
	* @brief Call back function for SAX parser when a start document event is 
	         produced.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date December 8, 2005
	* @version 1.0
	*/
	void startDocument();
	
	/**
	* @brief Call back function for SAX parser when a start document event is 
	         produced.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date December 8, 2005
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
	* @date December 8, 2005
	* @version 1.0
	*/
	void startElement(const std::string& namespaceURI, 
		const std::string& localName,
		const std::string& qName, 
		const AttributesString& atts);

	/**
	* @brief Return a pointer to the option object the ConfigMgr stores. The
			returned object cannot be modified.
	* @return As above.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 8, 2007
	* @version 1.0
	*/
	const Options* getOptions();

private:
	/**
	* This variable stores the option name, value pairs read from XML.
	*/
	StringMap _optionMap;

	/**
	* This variable stores all the system options.
	*/
	Options* _options;

}; //class ConfigMgr
}  //namespace SAXHandler
}  //namespace Cayuga

#endif //_CONFIG_MGR_H

