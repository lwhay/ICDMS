/**
 * @file EventFinder.h
 * @date December 9, 2005
 * @brief Finds handles in events stored in Priority Queue for GC 
          in Cayuga engine.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _EVENT_FINDER_H
#define _EVENT_FINDER_H

#include "Event.h"
#include "MemoryMgr/CY_GCFace.h"

#include <set>
using namespace std;

namespace Cayuga {
namespace BasicDataStructure {
namespace EventNS {
class Event;
/**
 * @brief An EventFinder object enumerates all events stored in Priority Queue 
		  in Cayuga engine.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date December 9, 2005
 * @version 1.0
 */
class EventFinder : public CY_Heap::RootFinder {
public:
	/**
	* @brief Default constructor
	* @param unprocessedEvents The set of events in Priority Queue at this time 
	         instance
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date Decemeber 9, 2005
	* @version 1.0
	*/
	EventFinder(multiset<Event*, endTimeLess>& unprocessedEvents);
	
	/**
	* @brief Virtual destructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date Decemeber 9, 2005
	* @version 1.0
	*/
	~EventFinder();

	/**
	* @brief Fill the buffer with Handle values, and return the
		number of values added.
	* @param bufsz The maximum number of handle values to add
	* @param buf The handle buffer to add handles to
	* @return Number of handle values added. <= 0 at end of enumeration.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date Decemeber 9, 2005
	* @version 1.0
	*/
	virtual int fill(unsigned bufsz, CY_Heap::GHandle * buf);
	
	/**
	* @brief Reset the status of this instance finder.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date Decemeber 9, 2005
	* @version 1.0
	*/
	virtual void reset();

private:
	/**
	* This variable stores the current iterator position on events in 
	* Priority Queue.
	*/
	multiset<Event*, endTimeLess>::iterator _event_it;
	
	/**
	* This variable stores a reference to the set of events in Priority Queue.
	*/
	multiset<Event*, endTimeLess>& _unprocessedEvents;
}; //class EventFinder
}  //namespace EventNS
}  //namespace BasicDataStructure
}  //namespace Cayuga


#endif //_EVENT_FINDER_H
