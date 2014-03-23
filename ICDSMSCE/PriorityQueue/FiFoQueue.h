/**
 * @file FifoQueue.h
 * @date April 15, 2007
 * @brief A FIFO queue that is thread safe and does not block.

 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _FIFO_QUEUE_H
#define _FIFO_QUEUE_H

#include "PriorityQueue/GenericQueue.h"

#include <queue>
using namespace std;

#include "corelib/os/threads/Mutex.h"
using namespace cudb::corelib::os::threads;

namespace Cayuga {
/**
 * @brief An FifoQueue object stores the Cayuga commands in FIFO order.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date April 15, 2007
 * @version 1.0
 */
template<typename ElemT>
class FifoQueue : public GenericQueue<ElemT> {
public:
	/**
	* @brief Default constructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 15, 2007
	* @version 1.0
	*/
	FifoQueue();

	/**
	* @brief Return the pointer to the next element to process.
			 Return NULL if the queue is empty.
	* @return As above. 
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 15, 2007
	* @version 1.0
	*/
	const ElemT* peek();

	/**
	* @brief Return the pointer to the next element to process, and consume it.
	* @return As above. 
	* @invariant The queue cannot be empty at this invocation. 
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 15, 2007
	* @version 1.0
	*/
	const ElemT* get();

   /**
	* @brief Put element into the queue.
	* @param elem Pointer to the input element
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 15, 2007
	* @version 1.0
	*/
	void put(const ElemT* elem);

	/**
	* @brief Return true if there is no element in the queue.
	* @return As above.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 15, 2007
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

private:
	/**
	* @brief This is the implementation of peeking command. 
	* @return Same as peek. 
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 15, 2007
	* @version 1.0
	*/
	const ElemT* _peekElemT();

	/**
	* This variable stores all the elements that have been fed into 
	* the queue but have not been processed.
	*/
	queue<const ElemT*> _elems;

	/**
	* This variable is used to control access to the queue.
	 */
	Mutex _mutex;
}; //class FifoQueue
}  //namespace Cayuga

#endif //_FIFO_QUEUE_H
