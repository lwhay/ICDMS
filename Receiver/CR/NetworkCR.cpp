/**
 * @file NetworkCR.cpp
 * @date March 23, 2007
 * @brief This file defines the class NetworkCR.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "../Receiver/CR/NetworkCR.h"
//#include "../Receiver/Options/NetworkCROptions.h"
#include "../Receiver/CR/Command.h"
#include "../Receiver/CR/CommandQueue.h"
using namespace Cayuga::Receiver;

#include "network/Message.h"
#include "corelib/os/threads/Thread.h"
using namespace cudb::corelib::os::threads;

#include "../ICDSMSCE/SAXHandler/ConfigMgr.h"
using Cayuga::SAXHandler::ConfigMgr;

#include "../ICDSMSCE/SystemMgr/Options.h"
using Cayuga::SystemMgr::Options;

#include "../ICDSMSCE/Utils/split.h"
using Cayuga::Utils::split;

#include <cassert>
using namespace std;

NetworkCR::NetworkCR(TCPClientSocket* clntSock, ostream& debugOut) 
: NetworkReceiverWorker(clntSock, debugOut) {
	_delim = ConfigMgr::getInstance()->getOptions()->_attrDelimiter;
}

void NetworkCR::_processMessage(Message& recvMsg) {
	char* strMsg = recvMsg.getData();
	//by MessageChannel::receive semantics,
	//the last byte -- NULL -- should not be recorded
	size_t szMsg = recvMsg.getSize();
	assert (strMsg[szMsg-1] == '\0');

	//assert the second-to-last character received in the message 
	//must be the character delimiter.
	//Will replace it with \0 to reuse the Utils::split logic
	assert (strMsg[szMsg-2] == _delim);
	strMsg[szMsg-2] = '\0';
	Command* pCmd = makeCommand(strMsg, _delim);
	CommandQueue::getInstance()->put(pCmd);
}

string NetworkCR::name() {
	return "network CR";
}

string NetworkCR::_name() {
	return NetworkCR::name();
}
