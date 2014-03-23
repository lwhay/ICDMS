/**
 * @file Receiver.h
 * @date March 23, 2007
 * @brief The file declares the interface for receiver.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _RECEIVER_H
#define _RECEIVER_H

namespace Cayuga {
namespace Receiver {
/**
 * @brief A Receiver object reads external inputs into Cayuga, such as events
		  and commands.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date March 23, 2007
 * @version 1.0
 */
template <typename T>
class Receiver {
public:
	virtual ~Receiver() {
	}

	/**
	* @brief Start receiving inputs according to the input options.
	* @param options The options for this receiver.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date March 23, 2007
	* @version 1.0
	*/
	virtual void open(const T* options) = 0;
private:
}; //class Receiver
}  //namespace Receiver
}  //namespace Cayuga

#endif //_RECEIVER_H
