/**
 * @file GenericQueue.h
 * @date March 19, 2007
 * @brief A generic queue interface for Cayuga queues, including event queue
		 (a priority queue based on timestamp) and command queue 
		 (a regular FIFO queue).
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _GENERIC_QUEUE_H
#define _GENERIC_QUEUE_H

#include <cstddef>

namespace Cayuga {

/**
 * @brief An PriorityQueue object re-orders input events based on their 
		  end timestamps.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 17, 2005
 * @version 1.0
 */
template <typename T>
class GenericQueue {
public:
	/**
	* @brief Return the pointer to the next element in the queue.
			 Return NULL if the queue is empty.
	* @return As above. 
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date March 19, 2007
	* @version 1.0
	*/
	T* peek();

	/**
	* @brief Return the pointer to the next element in the queue and consume it.
	* @return As above. 
	* @invariant The queue cannot be empty at this invocation. 
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date March 19, 2007
	* @version 1.0
	*/
	T* get();

   /**
	* @brief Put the element into the queue.
	* @param elem Pointer to the input element
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date March 19, 2007
	* @version 1.0
	*/
	void put(T* elem);

	/**
	* @brief Return true if there is no element in the queue.
	* @return As above.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date March 19, 2007
	* @version 1.0
	*/
	bool empty();

	/**
	* @brief Return the number of elements in the queue.
	* @return As above.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 23, 2007
	* @version 1.0
	*/
	size_t size();

}; //class GenericQueue
}  //namespace Cayuga

#endif //_GENERIC_QUEUE_H
