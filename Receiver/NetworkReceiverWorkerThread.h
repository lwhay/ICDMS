/**
 * @file NetworkReceiverWorkerThread.h
 * @date April 15, 2007
 * @brief This file declares the class NetworkReceiverWorkerThread.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _NETORK_RECEIVER_WORKER_THREAD_H
#define _NETORK_RECEIVER_WORKER_THREAD_H

#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

#include "corelib/os/threads/Thread.h"
using cudb::corelib::os::threads::Thread;

namespace Cayuga {
namespace Receiver {
/**
 * @brief The Network Receiver Worker Thread. 
 * @param debugOut The debug message output stream
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date April 15, 2007
 * @version 1.0
 */
template <typename WorkerT>
class NetworkReceiverWorkerThread : public Thread {
public:
	NetworkReceiverWorkerThread(TCPClientSocket* clntSock, 
		ostream& debugOut = cout) 
		: _clntSock(clntSock), _debugOut(debugOut) {
	}
	~NetworkReceiverWorkerThread() {
		//_clntSock will be deleted in the new thread created in run
	}
protected:
	virtual void* run() {
		WorkerT* pNetworkWorker = new WorkerT(_clntSock, _debugOut);

// 		NetworkReceiverWorker * ptr = NetworkReceiverWorker::getInstance();
// 		*ptr = *(NetworkReceiverWorker*)pNetworkWorker;

		pNetworkWorker->open();
		return NULL;
	}
private:
	TCPClientSocket* _clntSock;

	ostream& _debugOut;
}; //class NetworkReceiverWorkerThread
}  //namespace Receiver
}  //namespace Cayuga

#endif //_NETORK_RECEIVER_WORKER_THREAD_H
