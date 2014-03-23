	/**
 * @file NetworkSenderWorker.cpp
 * @date March 23, 2007
 * @brief This file defines the class NetworkSenderWorker.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "NetworkSenderWorker.h"
using namespace Cayuga::Sender;

#include "network/TCPClientSocket.h"
#include "network/MessageChannel.h"

#include <cassert>
using namespace std;

NetworkSenderWorker::NetworkSenderWorker(TCPClientSocket* clntSock) 
: _clntSock(clntSock), _messageChannel(NULL) {
	_messageChannel = new MessageChannel(_clntSock);
}

NetworkSenderWorker::~NetworkSenderWorker() {
	cerr<<"Network Sender Worker destructed."<<endl;
	delete _messageChannel;
	delete _clntSock;
}

void NetworkSenderWorker::send(const string* str) {
	try {
		Message sendMsg(str->c_str(), str->size()); 		
		_messageChannel->send(&sendMsg);
	}
	catch (cudb::network::exceptions::SocketException& e) {
		throw e; //rethrow it to be caught by its caller
	}

}
