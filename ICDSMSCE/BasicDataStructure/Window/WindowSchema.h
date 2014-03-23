#ifndef _WINDOW_SCHEMA_H
#define _WINDOW_SCHEMA_H

#include <cstdio>

namespace Cayuga {
	namespace BasicDataStructure {
		namespace WindowSchema {
			enum WinType{
				TIMEWINDOW,
				COUNTWINDOW

			};
			typedef float WinLength;
			typedef float WinStep;
			typedef float WinTime;
			typedef long WinID;


		}  //namespace Automaton
	}  //namespace BasicDataStructure
}  //namespace Cayuga


#endif //_NODE_ID_H
