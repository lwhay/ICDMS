/**
 * @file NetworkCR.h
 * @date March 23, 2007
 * @brief This file declares the class NetworkCR.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _NETORK_CR_H
#define _NETORK_CR_H

#include "../Receiver/NetworkReceiverWorker.h"

#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

namespace Cayuga {
namespace Receiver {
/**
 * @brief The Command Receiver (CR). 
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date March 23, 2007
 * @version 1.0
 */
class NetworkCR : public NetworkReceiverWorker {
public:
	/**
	* @brief Default constructor
	* @param debugOut The debug message output stream
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date March 23, 2007
	* @version 1.0
	*/
	NetworkCR(TCPClientSocket* clntSock, ostream& debugOut = cout);
	
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
}; //class NetworkCR
}  //namespace Receiver
}  //namespace Cayuga

#endif //_NETORK_CR_H
