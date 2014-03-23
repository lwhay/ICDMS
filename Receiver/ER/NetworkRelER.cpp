/**
 * @file NetworkRelER.cpp
 * @date April 19, 2006
 * @brief This file defines the class NetworkRelER.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "../Receiver/ER/NetworkRelER.h"
#include "../Receiver/ER/RelER.h"
//#include "../Receiver/Options/NetworkEROptions.h"
using namespace Cayuga::Receiver;

#include "../ICDSMSCE/SAXHandler/ConfigMgr.h"
using namespace Cayuga::SAXHandler;

#include "../ICDSMSCE/SystemMgr/Options.h"
using Cayuga::SystemMgr::Options;

#include "network/Message.h"

NetworkRelER::NetworkRelER(TCPClientSocket* clntSock,
						   ostream& debugOut) 
: NetworkReceiverWorker(clntSock, debugOut) {
	_delim = ConfigMgr::getInstance()->getOptions()->_attrDelimiter;
	_bStrict = ConfigMgr::getInstance()->getOptions()->_bStrict;
}

void NetworkRelER::_processMessage(Message& recvMsg) {
	//unsigned cnt = 0;
	//cout<<"[client] message "<<cnt++<<" received"<<endl;

	parseEvent(recvMsg.getData(), _delim, _bStrict);
}

string NetworkRelER::name() {
	return "network ER";
}

string NetworkRelER::_name() {
	return NetworkRelER::name();
}
