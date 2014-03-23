// /**
// * @file WindowString.h
// * @date 2011-3-22
// * @brief Base class of window read from AIR.
// * @author leixiaoluo
// */
// 
// #ifndef _WINDOWE_STRING_H
// #define _WINDOWE_STRING_H
// 
// 
// #include <iostream>
// #include <vector>
// using namespace std;
// 
// namespace Cayuga {
// namespace AutomatonCompiler {
// 		class Context;
// 
// 		/**
// 		* @brief A WindowString object represents an window constructed 
// 		from SAX events.	
// 		* @see	EdgeString
// 		* @see QueryString
// 		* @author leixiaoluo
// 		* @date 2011-3-22
// 		*/
// 		class WindowString {
// 		public:
// 
// 			/**
// 			* @brief Default constructor
// 			* @param windowType Type of the window read from AIR
// 			* @author leixiaoluo
// 			* @date 2011-3-22
// 			*/
// 			WindowString(const string& windowType);
// 
// 			/**
// 			* @brief Virtual destructor
// 			* @author leixiaoluo
// 			* @date 2011-3-22
// 			*/
// 			 ~WindowString();
// 
// 			/**
// 			* @brief 
// 			* @param context The compilation context
// 			* @return Pointer to the compiled window.
// 			* @author leixiaoluo
// 			* @date 2011-3-22
// 			*/
// 			 Window * compile(Context& context);
// 
// 			/**
// 			* @brief Output member variable information in XML format, aiming
// 			at reconstructing the XML piece in AIR which encodes this node
// 			* @param out The output stream handle.
// 			* @author leixiaoluo
// 			* @date 2011-3-22
// 			*/
// 			 void write(ostream& out = cout) = 0;
// 
//          
//           /************************************************************************/
//           /* 
//           *@brief  The function is used for stroing the first row attribute read from 
// 		  the XML
// 		  *@param windid The id of this window
// 		  *@param windtype  The type of this window
// 		  *@author leixiaoluo
// 		  *@date  2011-3-23
// 		  */
//           /************************************************************************/
//           void addPrimerAttr(const string& windid, const string& windtype);
// 
// 		   /************************************************************************/
//           /* 
//           *@brief  The function is used for stroing the second row attribute read
// 		  from the XML
// 		  *@param windsize The size of this window , if the type of this window is
// 		  time , it mean the time span is identified by time , or the type is count	
// 		  then it identify the span is number
// 		  *@param windstep  The slidestep of this window , it is different from the 
// 		  type of  the window
// 		  *@author leixiaoluo
// 		  *@date  2011-3-23
// 		  */
//           /************************************************************************/
// 		  void addWindowAttr(const string& windsize, const string& windstep);
// 
// 
// 
// 		}; //class WindowString.h
// 	}  //namespace AutomatonCompiler
// }  //namespace Cayuga
// 
// #endif //_WINDOWE_STRING_H


/**
* @file WindowString.h
* @date March 23, 2011
* @brief Base class of window read from AIR.
* @author Rainbow (rainbowsworld@163.com)
*/

#ifndef _WINDOWE_STRING_H
#define _WINDOWE_STRING_H

#include <iostream>
#include "BasicDataStructure/Window/WindowSchema.h"
using namespace Cayuga::BasicDataStructure::WindowSchema;
#include "AutomatonCompiler/Context.h"
using namespace Cayuga::AutomatonCompiler;


/*#include "BasicDataStructure/Window/Window.h"*/

#include "BasicDataStructure/Window/inputWindow.h"

#include "Utils/StringMap.h"
using namespace Cayuga::Utils;
using namespace std;
/*
namespace Cayuga {
	namespace BasicDataStructure {
		namespace Automaton {
			class Window;
		}
	*/

namespace Cayuga{
	namespace AutomatonCompiler{
	//	using  Cayuga::BasicDataStructure::Automaton::Window;
	//	using namespace AutomatonCompiler::Context;
		/*
		enum WinType{
			TIMEWINDOW,
			COUNTWINDOW

		};
		typedef float WinLength;
		typedef float WinStep;
		typedef float WinTime;
		typedef long WinID;
		*/
		class WindowString{
		public:
			//@brief Default constructor
			WindowString();
			WindowString(const StringMap& attrmap);
			//If the length equals 0 ,the length of the window will be infinite;
			//If the step equals 0,the window0 won't be sliding.

			WindowString(WinType _winType,WinLength _length=0,WinStep _step=0,WinTime _startTime=0);
			
			//@brief Default destructor
			~WindowString();
			WinType getWinType();
			WinLength getWinLength();
			WinStep getWinStep();
			WinTime getWinStartTime();
			WinID getWindowID();
			void setWinType(WinType _winType);
			void setWinLength(WinLength _length);
			void setWinStep(WinStep _step);
			void setWinStartTime(WinTime _startTime);
			void setWindowID(WinID id);

			void write(ostream& out=cout);
/*		    Window *compile(Context context);*/

			//luoluo 5-20
			inputWindow * recompile(Context context);

            //luoluo 2012-5-23
            void setWinType(const string& setwintype);
            void setWinLength(const string& setwinlength);
            void setWinStep(const string& setwinstep);
            void setWinStartTime(const string& setwinstarttime);

		private:
			WinType winType;
			WinLength winLength;
			WinID winID;
			WinStep winStep;
			WinTime startTime;
			static WinID winIDCount;

		};
	}
}
#endif 
