/**
 * @file NetworkEROptions.h
 * @date March 23, 2007
 * @brief The file declares the interface for NetworkEROptions.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _NETWORK_ER_OPTIONS_H
#define _NETWORK_ER_OPTIONS_H

namespace Cayuga {
namespace Receiver {
/**
  * @brief A NetworkEROptions object stores the options for a network 
			event receiver.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date March 23, 2007
 * @version 1.0
 */
class NetworkEROptions {
public:
	NetworkEROptions(const string& ipPort, const char charDelim = '`')
		: _ipPort(ipPort), _charDelim(charDelim) {
	}
	
	const string& _ipPort;
	char _charDelim;

}; //class NetworkEROptions
}  //namespace Receiver
}  //namespace Cayuga

#endif //_NETWORK_ER_OPTIONS_H
