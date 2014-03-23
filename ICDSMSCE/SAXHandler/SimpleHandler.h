/**
 * @file SimpleHandler.h
 * @date May 5, 2006
 * @brief Definition of SimpleHandler. This is a wrapper around 
          SAX2PrintHandlers for interface compatibility with Arabica SAX parser
		  we used previously. Specially, it is the two functions startElement
		  and endElement that should be adapted.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef __Simple_Handler_H
#define __Simple_Handler_H

#include "arch-dep.h"

#include    "SAX2PrintHandlers.hpp"

//#include    <xercesc/sax2/Attributes.hpp>
#include "../BASELIB/xercesc/sax2/Attributes.hpp"

#include <iostream>	
#include <string>	
#include <vector>
using namespace std;

XERCES_CPP_NAMESPACE_USE

namespace Cayuga {
namespace SAXHandler {
class AttributesString
{
public:
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    /** @name Constructors and Destructor */
    //@{
    /** Default constructor */
	AttributesString (const Attributes& atts) : _atts(atts)
    {
    }

    /** Destructor */
    virtual ~AttributesString()
    {
    }
    //@}

    /** @name The virtual attribute list interface */
    //@{
  /**
    * Return the number of attributes in this list.
    *
    * The SAX parser may provide attributes in any
    * arbitrary order, regardless of the order in which they were
    * declared or specified.  The number of attributes may be
    * zero.
    *
    * @return The number of attributes in the list.
    */
	virtual unsigned int getLength() const {
		return _atts.getLength();
	}

  /**
    * Return the local name of an attribute in this list (by position).
    *
    * The QNames must be unique: the SAX parser shall not include the
    * same attribute twice.  Attributes without values (those declared
    * #IMPLIED without a value specified in the start tag) will be
    * omitted from the list.
    *
    * @param index The index of the attribute in the list (starting at 0).
    * @return The local name of the indexed attribute, or null
    *         if the index is out of range.
    * @see #getLength
    */
	virtual const string getLocalName(const unsigned int index) const {
		const XMLCh* attrName = _atts.getLocalName(index);
		const string ret(XMLString::transcode(attrName));
		return ret;
	}

	 /**
    * Return the value of an attribute in the list (by position).
    *
    * If the attribute value is a list of tokens (IDREFS,
    * ENTITIES, or NMTOKENS), the tokens will be concatenated
    * into a single string separated by whitespace.
    *
    * @param index The index of the attribute in the list (starting at 0).
    * @return The attribute value as a string, or
    *         null if the index is out of range.
    * @see #getLength
    * @see #getValue(XMLCh*)
    */
	virtual const string getValue(const unsigned int index) const {
		const XMLCh* attrValue = _atts.getValue(index);
		const string ret(XMLString::transcode(attrValue));
		return ret;
	}

private:
	const Attributes& _atts;

}; //AttributesString

class SimpleHandler : public SAX2PrintHandlers 
{
public:
   SimpleHandler();

   virtual ~SimpleHandler();

	virtual void startElement(const std::string& namespaceURI, 
		const std::string& localName,
		const std::string& qName, 
		const AttributesString& atts);

	virtual void endElement(const std::string& namespaceURI, 
		const std::string& localName,
		const std::string& qName);


    void endElement( const XMLCh* const uri,
					 const XMLCh* const localname,
					 const XMLCh* const qname);

    void startElement(	const   XMLCh* const    uri,
						const   XMLCh* const    localname,
						const   XMLCh* const    qname,
					    const   Attributes&		attributes);

protected:
	/**
	* @brief Create an attribute map from the input attribute name, value pairs
	* @param atts The input attribute name, value pairs given by the SAX parser
	* @param attrMap The attribute map to be created
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date May 5, 2006
	* @version 1.0
	*/
	void createAttrMap(const AttributesString& atts, 
		hash_map<std::string, std::string>& attrMap);
}; //class SimpleHandler

//forward declaration	
void pluginHandler (SimpleHandler* pHandler, 
					const std::vector<std::string>& inputFileNames,
					const size_t printFrequency = 1000,
					ostream& debugOut = cout);

}  //namespace SAXHandler
}  //namespace Cayuga

#endif
