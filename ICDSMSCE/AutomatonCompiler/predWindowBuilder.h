 #ifndef _PREDWINDOW_BUILDER_H
 #define _PREDWINDOW_BUILDER_H
 
 #include "SAXHandler/SimpleHandler.h"
#include "AutomatonCompiler/predWindowString.h"
using Cayuga::AutomatonCompiler::predWindowString;

#include "Utils/AttrMap.h"

  #include <string>
 
namespace Cayuga {
 	namespace AutomatonCompiler {
		class predWindowString;

 		/**
 		* @brief A predWindowBuilder object builds an predwindow from SAX events.	
		*/
		class predWindowBuilder : public SimpleHandler {

 		public:
			/**
			* @brief Default constructor
 			*/
 			predWindowBuilder();

 			void startElement(const std::string& namespaceURI, 
 				const std::string& localName,
				const std::string& qName, 
 				const AttributesString& atts);
 
			void endElement(const std::string& namespaceURI, 
				const std::string& localName,
				const std::string& qName);
			//get the window created by the windowBuilder
			predWindowString* getPredWindow();
 		
 		private:            
 			predWindowString * _predWindow ;
 
 
 
 		}; 
 	}  //namespace AutomatonCompiler
 }  //namespace Cayuga
#endif 
