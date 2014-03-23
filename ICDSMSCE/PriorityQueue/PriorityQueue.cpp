/**
 * @file PriorityQueue.cpp
 * @date November 17, 2005
 * @brief Implementation file of PriorityQueue class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "BasicDataStructure/Event/EventFinder.h"
using namespace Cayuga::BasicDataStructure::EventNS;

#include "PriorityQueue/PriorityQueue.h"
using namespace Cayuga::PriorityQueue;

PriorityQueue::PriorityQueue() {
	CY_Heap::registerRootFinder( new EventFinder(_events) );
}

Event* PriorityQueue::_peekEvent() {
	//This function is called by the public APIs peek and get above, 
	//so mutex is used in that level, not here.
	if (_events.empty()) {
		return NULL;
	}

	multiset<Event*, endTimeLess>::iterator it = _events.begin();
	Event* pThisEvent = *it;
	return pThisEvent;
}

Event* PriorityQueue::peek() {
	Lock lock = _mutex.scopedLock();
	return _peekEvent();
}
Event* PriorityQueue::get() {
	Lock lock = _mutex.scopedLock();
	Event* pThisEvent = _peekEvent();

	assert (pThisEvent);
	
	_events.erase(_events.begin());	
	return pThisEvent;
}

void PriorityQueue::put(Event* ev) {
	Lock lock = _mutex.scopedLock();
	_events.insert(ev);
// 	//luoluo 3-28
// 	_reevents.insert(ev);

}

bool PriorityQueue::empty() {
	Lock lock = _mutex.scopedLock();
	return _events.empty();
}

size_t PriorityQueue::size() {
	Lock lock = _mutex.scopedLock();
	return _events.size();
}
