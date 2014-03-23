#include "AutomatonCompiler/predWindowBuilder.h"
 #include "Utils/AttrMap.h"
#include "Utils/StringMap.h"
using namespace Cayuga::Utils;
 
 #include <string>
 using namespace std;
 using namespace Cayuga::AutomatonCompiler;

 predWindowBuilder::predWindowBuilder() {
 }

 predWindowString* predWindowBuilder::getPredWindow()
 {
	 return _predWindow;
 }
 
 void predWindowBuilder::startElement(const std::string& namespaceURI, 
				const std::string& localName,
 				const std::string& qName, 
				const AttributesString& atts) {
				try {
				assert(localName.length());
 				StringMap attrMap;
                createAttrMap(atts,attrMap);
				_predWindow=new predWindowString(attrMap);
 			   }
 			 catch (XMLException& e) {
 			 cerr<<e.getMessage();
 			 }
 
 }
 
 void predWindowBuilder::endElement(const std::string& namespaceURI, 
 			 const std::string& localName,
 			 const std::string& qName) {
 				 assert(localName.length());
				 cout<<"the predicate window is :"<<endl;
				 _predWindow->write();
 }
 