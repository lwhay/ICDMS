 /**
 * @file WindowBuilder.h
 * @date 2011-4-8
 * @brief Responsible of building an window from SAX events.
 * @author Rainbow
 */
 

 #ifndef _WINDOW_BUILDER_H
 #define _WINDOW_BUILDER_H
 
 #include "SAXHandler/SimpleHandler.h"
#include "AutomatonCompiler/WindowString.h"
#include "Utils/AttrMap.h"

  #include <string>
 
namespace Cayuga {
 	namespace AutomatonCompiler {
		class WindowString;

 		/**
 		* @brief A WindowBuilder object builds an window from SAX events.	
		* @see	EdgeBuilder
 		* @see QueryBuilder
		* @author Rainbow
 		* @date 2011-4-8
		*/
		class WindowBuilder : public SimpleHandler {

 		public:
			/**
			* @brief Default constructor
 			* @author Rainbow
			* @date 2011-4-8
 			*/
 			WindowBuilder();

 			void startElement(const std::string& namespaceURI, 
 				const std::string& localName,
				const std::string& qName, 
 				const AttributesString& atts);
 
			void endElement(const std::string& namespaceURI, 
				const std::string& localName,
				const std::string& qName);
			//get the window created by the windowBuilder
			WindowString* getWindow();
 
 	/**
 	* This variable stores the state of this window builder which XML 
 	* element are we use when reading the AIR xml file.
 	*/
   /*
 			enum {
				INIT,
				IN_WINDOW, 
		} _state;
		       
	*/    
 	//		void addPrimitiveAttr(const string& winID, const string& winType);
 	//		void addWindowAttr(const string& winSize, const string& winStep);
 		
 		private:
             
 			WindowString * pWindow ;
 
 
 
 		}; //class WindowBuilder
 	}  //namespace AutomatonCompiler
 }  //namespace Cayuga
 
 #endif //_WINDOW_BUILDER_H
