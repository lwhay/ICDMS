 /**
 * @file WindowBuiler.cpp
 */
 
 #include "AutomatonCompiler/WindowBuilder.h"
 #include "Utils/AttrMap.h"
#include "Utils/StringMap.h"
using namespace Cayuga::Utils;
 
 #include <string>
 using namespace std;
 using namespace Cayuga::AutomatonCompiler;

 WindowBuilder::WindowBuilder() {
 
 }
 WindowString* WindowBuilder::getWindow()
 {
	 return pWindow;
 }
 
 void WindowBuilder::startElement(const std::string& namespaceURI, 
				const std::string& localName,
 				const std::string& qName, 
				const AttributesString& atts) {
				try {
				assert(localName.length());
 				StringMap attrMap;
                createAttrMap(atts,attrMap);
				pWindow=new WindowString(attrMap);
				
/*
 				switch(_state) {
 			case INIT:
 				assert (localName == "Window");
				_state = IN_WINDOW;
 				createAttrMap(atts, attrMap);
                 string windowid = getParameterValue(attrMap , "WindowID");
				string windowtype = getParameterValue(attrMap , "WindowType");
				addPrimitiveAttr(windowid , windowtype);
				break;
 			case IN_WINDOW:
 				if (localName == "WindowClause") {
				createAttrMap(atts, attrMap);
 				string windowsize = getParameterValue<string>(attrMap, "WindowSize");
				string slidestep = getParameterValue<string>(attrMap, "SlideStep");
                 addWindowAttr(windowsize , slidestep);
				break;
 				}
 			    }
*/
 			   }
 			 catch (XMLException& e) {
 			 cerr<<e.getMessage();
 			 }
 
 }
 
 void WindowBuilder::endElement(const std::string& namespaceURI, 
 			 const std::string& localName,
 			 const std::string& qName) {
 				 assert(localName.length());
				 cout<<"the window is :"<<endl;
				 pWindow->write();
 
 }
 /*
 void WindowBuilder :: addPrimitiveAttr(const string& winID, const string& winType){
 			   pWindow->addPrimerAttr(winID , winType);
 }
 
 void WindowBuilder :: addWindowAttr(const string& winSize , const string& winStep){
 			  pWindow->addWindowAttr(winSize , winStep);
 }
 */