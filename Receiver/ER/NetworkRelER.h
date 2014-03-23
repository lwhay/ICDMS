/**
 * @file NetworkRelER.h
 * @date April 19, 2006
 * @brief This file declares the class NetworkRelER.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _NETORK_REL_ER_H
#define _NETORK_REL_ER_H


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


//#include "../Receiver/ER/RelER.h"
#include "../Receiver/NetworkReceiverWorker.h"

namespace Cayuga {
namespace Receiver {
//class NetworkEROptions;
/**
 * @brief The Network Event Receiver (ER). 
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date April 19, 2006
 * @version 1.0
 */
class NetworkRelER : public NetworkReceiverWorker {
public:
	/**
	* @brief Default constructor
	* @param debugOut The debug message output stream
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 19, 2006
	* @version 1.0
	*/
	NetworkRelER(TCPClientSocket* clntSock,
		ostream& debugOut = cout);

	/**
	* @brief Return a string label for the name of this receiver instance.
	* @return As above.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 15, 2007
	* @version 1.0
	*/
	static string name();
	
protected:
	virtual void _processMessage(Message& recvMsg);

	/**
	* @brief Return a string label for the name of this receiver instance.
	* @return As above.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 15, 2007
	* @version 1.0
	*/
	virtual string _name();

private:
	char _delim;
	bool _bStrict;
}; //class NetworkRelER
}  //namespace Receiver
}  //namespace Cayuga

#endif //_NETORK_REL_ER_H
