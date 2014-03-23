/**
 * @file Command_AddSchema.cpp
 * @date April 22, 2007
 * @brief Implementation file of Command_AddSchema class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "../Receiver/CR/Command_AddSchema.h"
#include "../Receiver/CR/Command.h"
using namespace Cayuga::Receiver;

#include "../ICDSMSCE/QueryProcessor/QueryProcessorImpl.h"
using namespace Cayuga::QueryProcessor;

#include "../ICDSMSCE/SAXHandler/SIR_Handler.h"
using namespace Cayuga::SAXHandler;
//
//#include "../ICDSMSCE/SystemMgr/Options.h"
//using namespace Cayuga::SystemMgr;

#include "../ICDSMSCE/Utils/AttrMap.h"
using Cayuga::Utils::toString;

#include <cassert>
#include <vector>
using namespace std;

Command_AddSchema::Command_AddSchema(const string& streamSchema) 
: _streamSchema(streamSchema) {
}

const string Command_AddSchema::type() const {
	return "Schema";
}

void Command_AddSchema::exec(QueryProcessorImpl* qpImpl,
							 ostream& debugOut) const {	
	//cout<<"reading input stream schema(s)."<<endl;
	const string schemaFileName = logCommand(".xml", _streamSchema);

	SIR_Handler mySIRHandler;
	pluginHandler (&mySIRHandler, vector<string>(1,schemaFileName), 
		1000, //print frequency
		debugOut);
}
