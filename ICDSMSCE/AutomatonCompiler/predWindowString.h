#ifndef _PREDWINDOWE_STRING_H
#define _PREDWINDOWE_STRING_H

#include <iostream>
#include "BasicDataStructure/Window/predWindowSchema.h"
using namespace Cayuga::BasicDataStructure::predWindowSchema;
#include "AutomatonCompiler/Context.h"
using namespace Cayuga::AutomatonCompiler;


#include "BasicDataStructure/Window/predWindow.h"

#include "Utils/StringMap.h"
using namespace Cayuga::Utils;
using namespace std;

namespace Cayuga{
	namespace AutomatonCompiler{
		class predWindowString{
		public:
			//@brief Default constructor
			predWindowString();
			predWindowString(const StringMap& attrmap);
			predWindowString(long tem,long num);

			
			//@brief Default destructor
			~predWindowString(){};

			//to get the schema value and set the schema value.
			void setPredWinID(predWinID id);
			predWinID getPredWinID();
			void setPredWinTemporal(predWinTemporal predTemp);
			predWinTemporal getPredWinTemporal();
			void setPredWinCount(predWinCount predCoun);
			predWinCount getPredWinCount();

			void write(ostream& out=cout);
			predWindow * recompile(Context context);

		private:
			static long predWindIdCount;
			predWinID  predID;
			predWinCount predCount;
			predWinTemporal predTemporal;

		};
	}
}
#endif 