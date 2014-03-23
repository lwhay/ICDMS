/**
 * @file Event.h
 * @date November 22, 2005
 * @brief Base class of events.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _EVENT_H
#define _EVENT_H

#include "arch-dep.h"

#include "BasicDataStructure/Event/EventID.h"
using Cayuga::BasicDataStructure::EventNS::EventID;

#include "BasicDataStructure/Timestamp.h"
using Cayuga::BasicDataStructure::TimeStamp;

#include "BasicDataStructure/StreamID.h"
using Cayuga::BasicDataStructure::StreamID;

#include "MemoryMgr/CY_HeapFace.h"

#include <cassert>
//#include <ctime>
#include <functional>
#include <iostream>
#include <set>
using namespace std;

using namespace CY_Heap;

namespace Cayuga {
namespace BasicDataStructure {

namespace EventNS {


/**
 * @brief An EventHeader object represents the header information of an event.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 22, 2005
 * @version 1.0
 */
class EventHeader {
public:	
	/** 
	* This variable stores the ID of the input event stream.
	*/
	StreamID _streamID;
	
	/** 
	* This variable stores the start timestamp of this event.
	*/
	TimeStamp _start;
	
	/** 
	* This variable stores the end timestamp of this event.
	*/
	TimeStamp _end;
};

/**
 * @brief An Event object represents an event.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 22, 2005
 * @version 1.0
 */
class Event {
public:
	/**
	* @brief Default constructor
	* @param bodyBytes Number of bytes in the event body
	* @param eid eid of this event
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 22, 2005
	* @version 1.0
	*/
	Event(size_t bodyBytes, EventID eid);

	/**
	* @brief Virtual destructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 22, 2005
	* @version 1.0
	*/
	virtual ~Event();

// 	//luoluo 8-25
// 	Event( Event & other);
// 	Event * operator = (Event* other);
	//luoluo8-26
	void setID (EventID eid);
	

	

	/**
	* @brief Allocate a new event with allocMGeneric
	* @param szSchema Size of the event body
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 28, 2007
	* @version 1.0
	*/
	static Event* allocEvent(size_t szSchema);

	/**
	* @brief Free the input event with freeMGeneric.
	* @param ev The input event to free.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 28, 2007
	* @version 1.0
	*/
	static void freeEvent(Event* ev);

	/**
	* @brief Output member variable information in XML format.
	* @param out The output stream handle.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 22, 2005
	* @version 1.0
	*/
	virtual void write(ostream& out = cout);

	/**
	* @brief Return the start timestamp of this event.
	* @return As above.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 30, 2005
	* @version 1.0
	*/
	//luoluo 4-10
/*	 virtual TimeStamp getStartTime() const;*/
	 TimeStamp getStartTime() const;
	
	/**
	* @brief Set the start timestamp of this event.
	* @param t The timestamp to set to.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 30, 2005
	* @version 1.0
	*/
	virtual void setStartTime(const TimeStamp& t);
	
	/**
	* @brief Return the end timestamp of this event.
	* @return As above.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 30, 2005
	* @version 1.0
	*/
	//luoluo 4-10
/*	 virtual TimeStamp getEndTime() const;*/
	TimeStamp getEndTime() const;
	
	/**
	* @brief Set the end timestamp of this event.
	* @param t The timestamp to set to.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 30, 2005
	* @version 1.0
	*/
	virtual void setEndTime(const TimeStamp& t);

	/**
	* @brief Return the input stream to which this event belongs.
	* @return As above.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 30, 2005
	* @version 1.0
	*/
	//luoluo 4-10
/*	virtual StreamID getStreamID() const;*/
	StreamID getStreamID() const;
	
	/**
	* @brief Set the stream ID of this event.
	* @param sid The stream ID to set to.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 30, 2005
	* @version 1.0
	*/
	virtual void setStreamID(const StreamID sid);

	/**
	* @brief Return the content of this instance.
	* @return As above.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 30, 2005
	* @version 1.0
	*/
	//luoluo 4-10
/*	virtual char* getBody();*/
	char* getBody();
	
	/**
	* @brief Return the content of this instance.
	* @return As above.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 30, 2005
	* @version 1.0
	*/
	//luoluo 4-10
	/*virtual const char* getBody() const;*/
	const char* getBody() const;

	/**
	* @brief Return the number of handles bytes in this event.
	* @return As above.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date December 9, 2005
	* @version 1.0
	*/
	virtual size_t getHandleBytes() const;
	
	/**
	* @brief Copy handles from this event to the handle buffer.
	* @param buf The handle buffer to copy to
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date December 9, 2005
	* @version 1.0
	*/
	virtual void copyHandles(CY_Heap::GHandle* buf);

	/**
	* @brief Overload the new operator to control the behavior of memory
	*        allocation for Event class.
	*        Specifically, we use our own memory pool to manage CAX events.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date Feburary 22, 2006
	* @version 1.0
	*/
	void* operator new(size_t, void* loc) /*throw(bad_alloc)*/;
   
	//luoluo 4-10
	/*virtual size_t getBodySize();*/
	size_t getBodySize();

	/**
	* @brief Get the unique ID of this event.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 28, 2007
	* @version 1.0
	*/
	EventID getID() const;

	/**
	* @brief Get the event with the input ID.
	* @param eid The input event ID
	* @return As above.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 28, 2007
	* @version 1.0
	*/
	//static Event* getEvent(EventID eid);

	//luoluo 4-10
		/** 
	* This variable stores the header information of this event.
	*/
	EventHeader _header;



	/**
	*@Get the information whether the event have been process.
	*@luoluo 5-4
	*/
	bool getProcessOrnot();

	/**
	*@Set the information whether the event have been process.
	*@luoluo 5-4
	*/
	void setProcessOrnot();
			/**
	*@This varible indicate whether the event have been taken into 
	*@the procession.
	*@luoluo 5-4
	*/
	 bool  _processOrnot ;

protected:
// 	/** 
// 	* This variable stores the header information of this event.
// 	*/
// 	EventHeader _header;

	/** 
	* This variable stores the number of bytes in the event body.
	*/
	size_t _numBytesBody;

	
	/** 
	* This variable stores the start of the event body.
	*/
	void* _body;
	//char _body[1];


private:
	EventID _id;
	//static EventID _idCnt;
	//static hash_map<EventID, Event*> _events;
}; //class Event


/**
 * @brief An endTimeLess object is a boolean function object that compares the
		  end time stamp values of two events
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 17, 2005
 * @version 1.0
 */
struct endTimeLess : public binary_function <Event*, Event*, bool> 
{
	bool operator()(
	const Event* _Left, 
	const Event* _Right
		) const {
			return (_Left->getEndTime() < _Right->getEndTime());
	}
};

// 	/**
// 	*@brief Another sort function  is used to compare the start stamp 
// 	values of two events 
// 	*@author leixiaoluo
// 	*@date 2011-3-28
// 	*/
// struct startTimeLess : public binary_function <Event * , Event * ,bool>
// {
// 	bool operator()(
// 		const Event * lp,
// 		const Event * rp
// 	)const {
// 		return ( lp->getStartTime()< rp->getStartTime());
// 	}
// };

}  //namespace EventNS
}  //namespace BasicDataStructure
}  //namespace Cayuga

#endif //_EVENT_H
