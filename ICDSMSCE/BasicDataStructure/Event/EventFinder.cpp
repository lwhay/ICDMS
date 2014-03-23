/**
 * @file EventFinder.cpp
 * @date December 9, 2005
 * @brief Implementation file of EventFinder class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include <cassert>

#include "BasicDataStructure/Event/EventFinder.h"
#include "BasicDataStructure/Event/Event.h"
using namespace Cayuga::BasicDataStructure::EventNS;

EventFinder::EventFinder(multiset<Event*, endTimeLess>& unprocessedEvents) 
: RootFinder(), _unprocessedEvents(unprocessedEvents) {
	reset();
}

EventFinder::~EventFinder() {
}

void EventFinder::reset() {
	_event_it = _unprocessedEvents.begin();
}

int EventFinder::fill(unsigned bufsz, GHandle * buf) {
	/* assume bufsz always big enough for at least one instance */
	GHandle *p = buf;
	while(_event_it != _unprocessedEvents.end()) {
		unsigned nh = (unsigned)(*_event_it)->getHandleBytes();
		nh = nh / sizeof(GHandle);  // number of handles, not bytes
		if( nh > bufsz ) break;
		(*_event_it)->copyHandles(p);
		p += nh;  bufsz -= nh;  ++_event_it;
	}
	return (p - buf);
}
