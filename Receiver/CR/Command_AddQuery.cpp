/**
 * @file Command_AddQuery.cpp
 * @date March 23, 2007
 * @brief Implementation file of Command_AddQuery class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "../Receiver/CR/Command_AddQuery.h"
#include "../Receiver/CR/Command.h"
using namespace Cayuga::Receiver;

#include "../ICDSMSCE/QueryProcessor/QueryProcessorImpl.h"
using namespace Cayuga::QueryProcessor;

//#include "../ICDSMSCE/SAXHandler/ConfigMgr.h"
//using namespace Cayuga::SAXHandler;


#include <cassert>
#include <vector>
using namespace std;

Command_AddQuery::Command_AddQuery(bool bAIR, const string& query) 
: _bAIR(bAIR), _strQuery(query) {
}

const string Command_AddQuery::type() const {
	return "Query";
}

void Command_AddQuery::exec(QueryProcessorImpl* qpImpl,
							ostream& debugOut) const {
	const string filename_extension = (_bAIR)? ".xml" : ".txt";
	const string queryFileName = logCommand(filename_extension, _strQuery);
	qpImpl->addQueries(vector<string>(1, queryFileName), _bAIR);
}
