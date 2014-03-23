/**
 * @file SAXHeader.cpp
 * @date November 27, 2005
 * @brief This file defines the utility functions for SAX parser below.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/TransService.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>

#include "SAXHandler/SimpleHandler.h"

#include <iostream>
using namespace std;

// ---------------------------------------------------------------------------
//  Local data
//
//  encodingName
//      The encoding we are to output in. If not set on the command line,
//      then it is defaulted to LATIN1.
//
//  xmlFile
//      The path to the file to parser. Set via command line.
//
//  valScheme
//      Indicates what validation scheme to use. It defaults to 'auto', but
//      can be set via the -v= command.
//
//	expandNamespaces
//		Indicates if the output should expand the namespaces Alias with
//		their URI's, defaults to false, can be set via the command line -e
// ---------------------------------------------------------------------------
static const char*              encodingName    = "LATIN1";
static XMLFormatter::UnRepFlags unRepFlags      = XMLFormatter::UnRep_CharRef;
static SAX2XMLReader::ValSchemes valScheme      = SAX2XMLReader::Val_Auto;
static bool					        expandNamespaces= false ;
static bool                     doNamespaces    = true;
static bool                     doSchema        = true;
static bool                     schemaFullChecking = false;
static bool                     namespacePrefixes = false;

/*
// ---------------------------------------------------------------------------
//  Local helper methods
// ---------------------------------------------------------------------------
static void usage()
{
    XERCES_STD_QUALIFIER cout << "\nUsage:\n"
            "    SAX2Print [options] <XML file>\n\n"
            "This program invokes the SAX2XMLReader, and then prints the\n"
            "data returned by the various SAX2 handlers for the specified\n"
            "XML file.\n\n"
            "Options:\n"
             "    -u=xxx      Handle unrepresentable chars [fail | rep | ref*].\n"
             "    -v=xxx      Validation scheme [always | never | auto*].\n"
             "    -e          Expand Namespace Alias with URI's. Defaults to off.\n"
             "    -x=XXX      Use a particular encoding for output (LATIN1*).\n"
             "    -f          Enable full schema constraint checking processing. Defaults to off.\n"
             "    -p          Enable namespace-prefixes feature. Defaults to off.\n"
             "    -n          Disable namespace processing. Defaults to on.\n"
             "                NOTE: THIS IS OPPOSITE FROM OTHER SAMPLES.\n"
             "    -s          Disable schema processing. Defaults to on.\n"
             "                NOTE: THIS IS OPPOSITE FROM OTHER SAMPLES.\n"
             "    -sa         Print the attributes in alfabetic order. Defaults to off.\n"
             "    -?          Show this help.\n\n"
             "  * = Default if not provided explicitly.\n\n"
             "The parser has intrinsic support for the following encodings:\n"
             "    UTF-8, USASCII, ISO8859-1, UTF-16[BL]E, UCS-4[BL]E,\n"
             "    WINDOWS-1252, IBM1140, IBM037, IBM1047.\n"
         <<  XERCES_STD_QUALIFIER endl;
}
*/

namespace Cayuga {
namespace SAXHandler {
/**
 * @brief Plug the handler into the handler driver
 * @param pHandler Pointer to the SAX handler to plug in
 * @param inputFileNames Vector of input XML files to parse
* @param debugOut The debug message output stream 
 * @date November 27, 2005
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @version 1.0
 */
void pluginHandler (SimpleHandler* pHandler, 
					const vector<string>& inputFileNames,
					const size_t printFrequency,
					ostream& debugOut) {
	// Initialize the XML4C2 system
	//set default values as follows
	valScheme = SAX2XMLReader::Val_Never;
    doNamespaces = false;
	doSchema = false;

    //
    //  Create a SAX parser object. Then, according to what we were told on
    //  the command line, set it to validate or not.
    //
    SAX2XMLReader* parser;
    SAX2XMLReader* reader = XMLReaderFactory::createXMLReader();
    parser=reader;

    //
    //  Then, according to what we were told on
    //  the command line, set it to validate or not.
    //
    if (valScheme == SAX2XMLReader::Val_Auto) {
        parser->setFeature(XMLUni::fgSAX2CoreValidation, true);
        parser->setFeature(XMLUni::fgXercesDynamic, true);
    }

    if (valScheme == SAX2XMLReader::Val_Never) {
        parser->setFeature(XMLUni::fgSAX2CoreValidation, false);
    }

    if (valScheme == SAX2XMLReader::Val_Always) {
        parser->setFeature(XMLUni::fgSAX2CoreValidation, true);
        parser->setFeature(XMLUni::fgXercesDynamic, false);
    }

    parser->setFeature(XMLUni::fgSAX2CoreNameSpaces, doNamespaces);
    parser->setFeature(XMLUni::fgXercesSchema, doSchema);
    parser->setFeature(XMLUni::fgXercesSchemaFullChecking, schemaFullChecking);
    parser->setFeature(XMLUni::fgSAX2CoreNameSpacePrefixes, namespacePrefixes);

    //
    //  Create the handler object and install it as the document and error
    //  handler for the parser. Then parse the file and catch any exceptions
    //  that propogate out
    //

    int errorCount = 0;
    int errorCode = 0;
    try {
        parser->setContentHandler(pHandler);
        parser->setErrorHandler(pHandler);

		for (size_t fileIdx = 0; fileIdx < inputFileNames.size(); ++fileIdx) {
			const char* xmlFile = inputFileNames[fileIdx].c_str();
			if (fileIdx % printFrequency == 0) {
				debugOut
					<<"opening XML file "<<xmlFile<<endl;
			}
			try { 
				parser->parse(xmlFile);  
			}
			catch (string &s) {  
				//Ideally, this exception which stems from ValueIDMap OR AIR_Handler
				//should be a more "sophisticated" type, such as a CayugaException.
				cerr << "Cayuga could not use input file " << xmlFile 
					<<" "<< s <<endl;
				continue;
			}
		}

        errorCount = parser->getErrorCount();
    }
    catch (const OutOfMemoryException&) {
        XERCES_STD_QUALIFIER cerr << "OutOfMemoryException" 
			<< XERCES_STD_QUALIFIER endl;
        errorCode = 5;          
    }
    catch (const XMLException& toCatch) {
        XERCES_STD_QUALIFIER cerr << "\nAn error occurred\n  Error: "
             << toCatch.getMessage()
             << "\n" << XERCES_STD_QUALIFIER endl;
        errorCode = 4;
    }

    if(errorCode) {
        XMLPlatformUtils::Terminate();
		cerr<<"SAX parser has error."<<endl;
		exit(-1);
    }

    //
    // Delete the parser itself. Must be done prior to calling Terminate, below.
    //
    delete reader;
}
} //namespace SAXHandler 
} //namespace Cayuga
