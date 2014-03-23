/**
 * @file NetworkSenderWorker.h
 * @date April 15, 2007
 * @brief This file declares the class NetworkSenderWorker.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _NETORK_SENDER_WORK_H
#define _NETORK_SENDER_WORK_H


#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

namespace cudb {
namespace network {
class TCPClientSocket;
class MessageChannel;
}
}
using namespace cudb::network;

namespace Cayuga {
namespace Sender {
/**
 * @brief The generic Network Sender Worker. 
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date April 15, 2007
 * @version 1.0
 */
class NetworkSenderWorker {
public:
	/**
	* @brief Default constructor
	* @param clntSock The client socket to be owned by this class
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 15, 2007
	* @version 1.0
	*/
	NetworkSenderWorker(TCPClientSocket* clntSock);
	
	/**
	* @brief Virtual destructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 15, 2007
	* @version 1.0
	*/
	~NetworkSenderWorker();

	/**
	* @brief Send the input string out.
	* @param str As above.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 15, 2007
	* @version 1.0
	*/
	void send(const string* str);

private:
	/** 
	* This variable stores the socket connection that we will maintain. 
	*/
	TCPClientSocket* _clntSock;

	/**
	* This variable stores the corresponding message channel for the
	* above socket connection. 
	* If we want to removed redundant buffer copy in reading socket buffer, 
	* MessageChannel should to be replaced by lower level functions that read 
	* socket buffer directly.
	*/
	MessageChannel* _messageChannel;
}; //class NetworkSenderWorker
}  //namespace Receiver
}  //namespace Cayuga

#endif //_NETORK_SENDER_WORK_H
