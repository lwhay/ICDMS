/**
 * @file PriorityQueue.h
 * @date November 17, 2005
 * @brief Re-order the output of events fed into Cayuga engine based on their 
		  end timestamps.

		  There should be one instance of this class in the system, which
		  means we should apply Singleton pattern.

 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _PRIORITY_QUEUE_H
#define _PRIORITY_QUEUE_H

#include "PriorityQueue/GenericQueue.h"
/*#include "BasicDataStructure/Window/Window.h"*/

#include "BasicDataStructure/Event/Event.h"
using namespace Cayuga::BasicDataStructure::EventNS;

#include "Utils/Singleton.h"
using Cayuga::Utils::Singleton;

#include "corelib/os/threads/Mutex.h"
using namespace cudb::corelib::os::threads;


#include <set>
#include <cassert>
using namespace std;

namespace Cayuga {
namespace PriorityQueue {
class PriorityQueue;
/**
 * @brief An PriorityQueue object re-orders input events based on their 
		  end timestamps.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 17, 2005
 * @version 1.0
 */
class PriorityQueue : public GenericQueue<Event>, 
	public Singleton<PriorityQueue> {
	friend class Cayuga::Utils::Singleton<PriorityQueue>;
// 	//luoluo 4-8
// 	friend class Window;

// 	//luoluo 4.2
// 	friend class Window;
public:
	/**
	* @brief Default constructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 17, 2005
	* @version 1.0
	*/
	PriorityQueue();

	/**
	* @brief Return the pointer to the next event to process.
			 Return NULL if the queue is empty.
	* @return As above. 
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date May 11, 2006
	* @version 1.0
	*/
	Event* peek();

	/**
	* @brief Return the pointer to the next event to process, and consume it.
	* @return As above. 
	* @invariant The queue cannot be empty at this invocation. 
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 17, 2005
	* @version 1.0
	*/
	Event* get();

   /**
	* @brief Put event into the queue.
	* @param ev Pointer to the input event
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 17, 2005
	* @version 1.0
	*/
	void put(Event* ev);

	/**
	* @brief Return true if there is no event in the queue.
	* @return As above.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 20, 2006
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

	//this variable is defined under the limit of "private" ,but 
	//in order to get its iterator point to beginning and ending.
	multiset<Event*, endTimeLess> _events;

private:
	/**
	* @brief This is the implementation of peeking event. 
	* @return Same as peek. 
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date May 11, 2006
	* @version 1.0
	*/
	Event* _peekEvent();

// 	/**
// 	* This variable stores all the events that have been fed into Cayuga system 
// 	* but have not been processed.
// 	*/
// 	multiset<Event*, endTimeLess> _events;

// 	/**
// 	*@ This variable stores the same events from _events which used for get 
// 	*the any events from a given time districts.
// 	*@luoluo 2-28
// 	*/
// 	multiset<Event* , startTimeLess> _reevents;

	/**
	* This variable is used to control access to the queue.
	 */
	Mutex _mutex;

// 	/**
// 	*@brief To build the index among the created instance 
// 	*@author leixiaoluo
// 	*@date 2011-4-4
// 	*/
// 	hashtable<Event ,
// 					 Event , 
// 					 hash<Event> ,
// 					 identity<Event> ,
// 					equal_to<Event> ,
// 					alloc>iht ;

}; //class PriorityQueue
}  //namespace PriorityQueue
}  //namespace Cayuga

#endif //_PRIORITY_QUEUE_H
