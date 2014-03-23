#include "SystemMsg.h"
using namespace Cayuga::SystemMsg;

#include <iostream>

string Cayuga::SystemMsg::ErrorMsg[] = {
	"functionality not implemented yet: ",
	"incompatible data types: ",
};

string Cayuga::SystemMsg::WarningMsg[] = {
	"a function is computed but the result is not named.",
	"two input streams contain attributes of the same name: ",
	"$ or $1 is used.",
	"time unit is used, and has not been supported by CEL compiler.",

};

string Cayuga::SystemMsg::Location[] = {
	"SELECT clause",
	"BINARY clause",
	"duration predicate",
};

string Cayuga::SystemMsg::Solution[] = {
	"Will create a new name for this attribute in SELECT clause",
	"Will create a new name for that attribute in the second input stream",
};


void Cayuga::SystemMsg::printError(Cayuga::SystemMsg::ErrorMsgID eid, 
								   string param) {
	cerr<<"Error: "<<ErrorMsg[eid]<<param<<endl;
	//throw exception();
	throw string();
}

void Cayuga::SystemMsg::printWarning(Cayuga::SystemMsg::LocationID locID, 
									 Cayuga::SystemMsg::WarningMsgID wid,
									 string param) {
	cerr<<"Warning: "<<"In a "<<Location[locID]<<", ";
	cerr<<WarningMsg[wid]<<param<<endl;

}

void Cayuga::SystemMsg::printSolution(Cayuga::SystemMsg::SolutionID sid) {
	cerr<<Solution[sid]<<endl;
}
