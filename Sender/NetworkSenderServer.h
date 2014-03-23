/**
 * @file NetworkSenderServer.h
 * @date March 23, 2007
 * @brief This file declares the class NetworkSenderServerThread.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _NETORK_SENDER_SERVER_THREAD_H
#define _NETORK_SENDER_SERVER_THREAD_H


#include "../Sender/NetworkSenderWorker.h"

#include "../ICDSMSCE/SAXHandler/ConfigMgr.h"
using namespace Cayuga::SAXHandler;

#include "../ICDSMSCE/SystemMgr/Options.h"
using namespace Cayuga::SystemMgr;

#include "network/TCPClientSocket.h"
#include "network/TCPServerSocket.h"
#include "network/MessageChannel.h"
using namespace cudb::network;

#include "corelib/os/threads/Thread.h"
using cudb::corelib::os::threads::Thread;

#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

namespace Cayuga {
namespace Sender {
/**
 * @brief The Network Sender Server. If param foreignAddr != "0" it becomes a client socket.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @author Lars Brenna (larsb@cs.uit.no)
 * @date October 3, 2007
 * @version 1.2
 */
class NetworkSenderServerThread : public Thread {
public:
	NetworkSenderServerThread(const short port,
		FifoQueue<string>& msgQueue, const string& senderName,
		ostream& debugOut = cout, const string& foreignAddr = "0") 
		: _msgQueue(msgQueue), _name(senderName), _debugOut(debugOut), 
		_fAddr(foreignAddr), _port(port) {
		//_servSock = new TCPServerSocket(port, 1);
		//_clntSock = new TCPClientSocket(foreignAddr.c_str(), port);
	}
	~NetworkSenderServerThread() {

		delete _servSock;
		delete _clntSock;
	}
protected:
	virtual void* run() {

		while (true) {
			try {
				if (_fAddr == "0"){
					_servSock = new TCPServerSocket(_port, 1);	
					_clntSock = _servSock->accept();
				}
				else {
					_servSock = NULL;
					_clntSock = new TCPClientSocket(_fAddr.c_str(), _port);
				}

				_debugOut
					<<"["<<_name<<"] client connection created."
					<<endl;

				_pNetworkSenderWork 
					= new NetworkSenderWorker(_clntSock);

				//continously obtain elements from the queue
				unsigned cnt = 0;
				unsigned printFrequency 
					= ConfigMgr::getInstance()->getOptions()->_printFrequency;

				while(true) {
					const string* pMsg = _msgQueue.peek();
					
					if (pMsg != NULL) {
						pMsg = _msgQueue.get();
						_pNetworkSenderWork->send(pMsg);
						delete pMsg;
						
						if (cnt % printFrequency == 0) {
							_debugOut
								<<"["<<_name<<"] message "
								<<cnt<<" sent"<<endl;
						}
						++cnt;
					}
					else {
						//sleep for while before retry
						//ideally do not actually poll the queue.
						//can wait on a semaphore.
						Thread::sleep (1000);
					}
				}
			}
			catch (cudb::network::exceptions::SocketException&) {
				cerr<<"["<<_name<<"[ connection disrupted."<<endl;
				
				delete _pNetworkSenderWork;
			}
		}
		return NULL;
	}
private:
	TCPServerSocket* _servSock;
	TCPClientSocket* _clntSock;

	NetworkSenderWorker* _pNetworkSenderWork;

	FifoQueue<string>& _msgQueue;

	//The name of this sender
	string _name;

	//The debug message output stream 
	ostream& _debugOut;

	//The hostname or IP for event receiver IFF in client mode
	string _fAddr;
	
	//port
	const short _port;


}; //class NetworkSenderServerThread
}  //namespace Receiver
}  //namespace Cayuga

#endif //_NETORK_SENDER_SERVER_THREAD_H
