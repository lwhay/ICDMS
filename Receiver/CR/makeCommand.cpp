/**
 * @file makeCommand.cpp
 * @date March 23, 2007
 * @brief Implementation of the factory function makeCommand.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "../Receiver/CR/Command.h"
#include "../Receiver/CR/Command_AddQuery.h"
#include "../Receiver/CR/Command_AddSchema.h"
using namespace Cayuga::Receiver;


#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <vector>
using namespace std;

#include "../ICDSMSCE/Utils/RAssert.h"
#include "../ICDSMSCE/Utils/split.h"
using namespace Cayuga::Utils;

namespace Cayuga {
namespace Receiver {
Command* makeCommand(const char* buffer, 
					 char delim) {
	vector<string> fields;
	split(string(buffer), fields, delim);

	assert (fields.size() > 2);

	//the first field is an integer denoting the number of fields following
	//(and not including) it
	int numFields = atoi(fields[0].c_str());
	assert (numFields+1 == fields.size()); //invariant

	//the second field is the type of the command, drawn from 
	//QUERY, SCHEMA, etc
	if (fields[1] == "QUERY") {
		//in this case we know there are 4 fields in total
		assert (numFields == 3);
		bool bAIR = (fields[2] == "AIR")? true : false;
		return new Command_AddQuery(bAIR, fields[3]);
	}
	else if (fields[1] == "SCHEMA") {
		assert (numFields == 2);
		return new Command_AddSchema(fields[2]);
	}
	else {
		Assert (false);
		return NULL;
	}
}
}  //namespace Receiver
}  //namespace Cayuga
