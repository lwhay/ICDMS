/**
 * @file Command.cpp
 * @date March 23, 2007
 * @brief Implementation file of Command class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "../Receiver/CR/Command.h"
using namespace Cayuga::Receiver;

#include "../ICDSMSCE/SAXHandler/ConfigMgr.h"
using namespace Cayuga::SAXHandler;

#include "../ICDSMSCE/SystemMgr/Options.h"
using namespace Cayuga::SystemMgr;

#include "../ICDSMSCE/Utils/AttrMap.h"
using Cayuga::Utils::toString;

#include <cassert>
#include <fstream>
#include <vector>
using namespace std;

long Command::_numCommands = 0;

const string Command::logCommand(
		const string& filename_extension,
		const string& cmd) const {

	string cmdFileName = "CMD_";
	string strNum = toString(_numCommands++);
	cmdFileName += strNum;
	cmdFileName += "_";

	cmdFileName += type();
	
	//cmdFileName += "_";

	cmdFileName += filename_extension;

	const string& dirName 
		= ConfigMgr::getInstance()->getOptions()->_commandLogDir;
	cmdFileName = dirName + cmdFileName;

	//write the command into a file
	ofstream out(cmdFileName.c_str());
	out<<cmd;
	out.close();

	return cmdFileName;
}
