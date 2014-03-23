/**
 * @file Event.cpp
 * @date November 22, 2005
 * @brief Implementation file of Event class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "BasicDataStructure/Event/Event.h"
using namespace Cayuga::BasicDataStructure::EventNS;

using Cayuga::BasicDataStructure::StreamID;

#include "corelib/os/threads/Mutex.h"
using namespace cudb::corelib::os::threads;

//EventID Event::_idCnt = 0;
//hash_map<EventID, Event*> Event::_events;
/*bool Event::_processOrnot = false;*/
Event::Event(size_t bodyBytes, EventID eid) 
: _numBytesBody(bodyBytes), _id(eid) ,_processOrnot(false){
	//cout<<"cons called"<<endl;
	_body = (char*)this + sizeof(Event);

	;
	//Mutex mutex;
	//Lock lock = mutex.scopedLock();
	//_id = _idCnt++; 
	//_events.insert(make_pair(_id, this));
}

Event::~Event() {
	//cout<<"des called"<<endl;
	//Mutex mutex;
	//Lock lock = mutex.scopedLock();
	//_events.erase(_id);
}

// Event::Event( Event& other)
// {
// 	_id = other.getID();
// 	_numBytesBody = other.getBodySize();
// 	_processOrnot = other.getProcessOrnot();
// 	_header._start = other.getStartTime();
// 	_header._end = other.getEndTime();
// 	_header._streamID = other.getStreamID();
// 	_body = new char[_numBytesBody+1];
// 	strcpy((char*)_body,other.getBody());
// }
// 
// Event* Event :: operator = (Event* other)
// {
// 	_id = other->getID();
// 	_numBytesBody = other->getBodySize();
// 	_processOrnot = other->getProcessOrnot();
// 	_header._start = other->getStartTime();
// 	_header._end = other->getEndTime();
// 	_header._streamID = other->getStreamID();
// 	_body = new char[_numBytesBody+1];
// 	strcpy((char*)_body,other->getBody());
// 	return this;
// }
void Event ::setID (EventID eid)
	{
		_id = eid;
	}

Event* Event::allocEvent(size_t szSchema) {
	EventID id;
	Event* ev = (Event*)CY_Heap::allocMGeneric
		(sizeof(Event) + szSchema, &id);
	//placement new
	return new(ev) Event(szSchema, id);
}

void Event::freeEvent(Event* ev) {
	ev->Event::~Event();
	size_t szEvent = sizeof(Event) + ev->getBodySize();
	CY_Heap::freeMGeneric(
		ev, 
		ev, 
		szEvent
		);
}

char* Event::getBody() {
	return (char*) _body;
}

const char* Event::getBody() const {
	return (const char*) _body;
}

void* Event::operator new(size_t, void* loc) /*throw(bad_alloc)*/ {
	return loc;
}

//void Event::operator delete(void* obj, size_t extraBytes) {
//	// Check for null pointer
//	if(!obj) return; 
//		//CY_Heap::freeM<Event>((Event*)obj);
//		CY_Heap::freeMGeneric( ((void *)(obj)), 
//			((void *)(obj)), 
//			sizeof(Event)+extraBytes );
//
//}

size_t Event::getHandleBytes() const {
	return _numBytesBody;
}

void Event::copyHandles(CY_Heap::GHandle* buf) {
	memcpy(buf, _body, _numBytesBody);
}

///@todo Implement this write function.
void Event::write(ostream& out) {
//	out<<_header._streamID<<" ";
//	out<<_header._start<<" "<<_header._end<<" ";
//	out<<_numBytesBody<<" ";
//
//	//out.write((const char*)_body, (streamsize)_numBytesBody)<<endl;
}

TimeStamp Event::getStartTime() const  {
	return _header._start;
}

void Event::setStartTime(const TimeStamp& t) {
	_header._start = t;
}

TimeStamp Event::getEndTime() const  {
	return _header._end;
}

void Event::setEndTime(const TimeStamp& t) {
	_header._end = t;
}

StreamID Event::getStreamID() const {
	return _header._streamID;
}

void Event::setStreamID(const StreamID sid) {
	_header._streamID = sid;
}

size_t Event::getBodySize() {
	return _numBytesBody;
}

EventID Event::getID() const {
	return _id;
}

bool Event ::getProcessOrnot(){
	return _processOrnot;
}

void Event ::setProcessOrnot(){
	_processOrnot = true;
}

//Event* Event::getEvent(EventID eid) {
//	//Mutex mutex;
//	//Lock lock = mutex.scopedLock();
//	hash_map<EventID, Event*>::iterator find_it = _events.find(eid);
//	assert (find_it != _events.end());
//	return find_it->second;
//}
