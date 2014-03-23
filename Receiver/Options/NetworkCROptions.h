/**
 * @file NetworkCROptions.h
 * @date March 23, 2007
 * @brief The file declares the interface for NetworkCROptions.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _NETWORK_CR_OPTIONS_H
#define _NETWORK_CR_OPTIONS_H

namespace cudb {
namespace network {
class TCPClientSocket;
}
}
using namespace cudb::network;

namespace Cayuga {
namespace Receiver {
/**
  * @brief A NetworkCROptions object stores the options for a network 
			command receiver.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date March 23, 2007
 * @version 1.0
 */
class NetworkCROptions {
public:
	NetworkCROptions(TCPClientSocket* clntSock, char delim = '`')
		: _clntSock(clntSock), _delim(delim) {
	}
	
	TCPClientSocket* _clntSock;
	char _delim;

}; //class NetworkCROptions
}  //namespace Receiver
}  //namespace Cayuga

#endif //_NETWORK_CR_OPTIONS_H
