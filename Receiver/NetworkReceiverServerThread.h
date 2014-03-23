/**
 * @file NetworkReceiverServerThread.h
 * @date April 15, 2007
 * @brief This file declares the class NetworkReceiverServerThread.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _NETORK_RECEIVER_SERVER_THREAD_H
#define _NETORK_RECEIVER_SERVER_THREAD_H


#include "NetworkReceiverWorkerThread.h"

#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

#include "network/TCPClientSocket.h"
#include "network/TCPServerSocket.h"
using namespace cudb::network;

#include "corelib/os/threads/Thread.h"
using cudb::corelib::os::threads::Thread;

namespace Cayuga {
namespace Receiver {
/**
 * @brief The Network Receiver Server. 
 * @param debugOut The debug message output stream
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date April 15, 2007
 * @version 1.0
 */template <typename WorkerT>
class NetworkReceiverServerThread : public Thread {
public:
	NetworkReceiverServerThread(const short port, ostream& debugOut = cout) :
	_debugOut(debugOut) {
		_servSock = new TCPServerSocket(port, 1);
	}
	~NetworkReceiverServerThread() {
		delete _servSock;
	}
	//luoluo 6-18
	TCPClientSocket * getTCPclientSocket(){
		return _clntSock;
	}
protected:
	virtual void* run() {

		while (true) {
			try {
				_clntSock = _servSock->accept();

				_debugOut<<"["<<WorkerT::name()
					<<"] client connection created."<<endl;

				NetworkReceiverWorkerThread<WorkerT>* newThread 
					= new NetworkReceiverWorkerThread<WorkerT>(_clntSock,
					_debugOut);
				newThread->start();
			}
			catch (cudb::network::exceptions::SocketException&) {
				//cerr<<"connection broken in "<<WorkerT::name()<<endl;
				Thread::sleep (100);
			}
		}
		return NULL;
	}


private:
	TCPServerSocket* _servSock;
	TCPClientSocket* _clntSock;

	ostream& _debugOut;
}; //class NetworkReceiverServerThread
}  //namespace Receiver
}  //namespace Cayuga

#endif //_NETORK_RECEIVER_SERVER_THREAD_H
