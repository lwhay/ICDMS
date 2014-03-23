/**
 * @file NetworkSender.cpp
 * @date April 14, 2007
 * @brief This file defines the class NetworkSender.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "../Sender/NetworkSender.h"
#include "../Sender/NetworkSenderServer.h"
using namespace Cayuga::Sender;

NetworkSender::NetworkSender(short nPort, const string& senderName,
							 ostream& debugOut, const string& _fAddr) {
	_pSenderServerThread = new NetworkSenderServerThread(
		nPort, _pMessageQueue, senderName, debugOut, _fAddr);

	_pSenderServerThread->start();
}


NetworkSender::~NetworkSender() {
	delete _pSenderServerThread;
}

void NetworkSender::send(const string* str) {
	_pMessageQueue.put(str);
}
