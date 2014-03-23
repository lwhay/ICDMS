#pragma once

#include <string>
using namespace std;

namespace Cayuga {
namespace SystemMsg {
enum ErrorMsgID {
	NOT_IMPLEMENTED,
	INCOMPATIBLE_DATA_TYPES,
};

extern string ErrorMsg[];

enum WarningMsgID {
	FUNC_NOT_NAMED,
	DUP_ATTR_NAME_IN_TWO_STREAMS,
	DOLLAR_USED,
	TIME_UNIT_USED,
};

extern string WarningMsg[];

enum LocationID {
	SELECT,
	BINARY,
	DURATION_PRED
};

extern string Location[];

enum SolutionID {
	CREATE_NEW_ATTR_NAME_SELECT,
	CREATE_NEW_ATTR_NAME_BINARY,
};

extern string Solution[];

//function declaration
void printError(ErrorMsgID eid, string param = "");

void printWarning(LocationID locID, WarningMsgID wid,
				  string param = "");

void printSolution(SolutionID sid);

} //SystemMgr
} //Cayuga
