/**
 * @file Window.h
 * @date 2011-3-23
 * @brief By using the parameters from the XML , we creat the window to 
 replace the PQ in Window Operation .

		  There should be one instance of this class in the system, which
		  means we should apply Singleton pattern.

 * @author leixiaoluo
 */

#ifndef _WINDOW_H
#define _WINDOW_H

#include "PriorityQueue/GenericQueue.h"

#include "PriorityQueue/PriorityQueue.h"
using namespace Cayuga::PriorityQueue;

#include "BasicDataStructure/Event/Event.h"
using namespace Cayuga::BasicDataStructure::EventNS;

#include "Utils/Singleton.h"
using Cayuga::Utils::Singleton;

#include "corelib/os/threads/Mutex.h"
using namespace cudb::corelib::os::threads;

#include "BasicDataStructure/Window/WindowSchema.h"
using namespace Cayuga::BasicDataStructure::WindowSchema;

//#include "AutomatonCompiler/WindowString.h"
//using namespace Cayuga::AutomatonCompiler;

/*#include "extlib/inc/stl/stl_hashtable.h"*/

#include <set>
#include <cassert>
#include <string>
using namespace std;
using namespace Cayuga;

typedef WinLength WindowSize;
typedef WinStep SlideStep;
typedef WinTime Start;
typedef WinID WindowID;
typedef WinType WindowType;

/*using Cayuga::BasicDataStructure::EventNS::Event;*/

/**
 * @brief An Window object  inherit from the PQ , which is used 
 for processing the Window Operator , On the other hand , it"s 
 a event queue.
 * @author leixiaoluo
 * @date 2011-3-23
 */

//public GenericQueue <Event>,
class Cayuga::PriorityQueue::PriorityQueue;
class Window : 
    public Singleton<Window>
{

	friend class Cayuga::Utils::Singleton<Window>;

public:
	/**
	* @brief Default constructor
	* @author leixiaoluo
	* @date 2011-3-23
	*/	
	Window();
	/**
	*@brief		To build the window queue from the AIR information.
	*@author		leixiaoluo
	*@date		2011-4-8
	*/
	Window(WindowID winid ,
				 WindowSize winsize,
				 WindowType wintype,
				 SlideStep winstep,
				 Start winstart);


//	static Window* getInstance();
// 	/* 
// 	*@brief The type of the window which have being defined
// 	*@author leixiaoluo
// 	*@date 2011-3-23
// 	*/
// 	enum WindowType{
// 		TIMEWINDOW,
// 		COUNTWINDOW
// 	}_type ;

	/**
	*@brief Set the window queue by copy the  events from the PQ
	queue 
	*@param The four parameters define the new window queue's size
 	*@param pPriorityQue  We get the events from this PQ and put them
	into the window queue under the window operator settings.
	*@author leixiaoluo
	*@date  2011-3-24
	*/
	 void SetWindowQueue( const WindowSize& winsize , 
		        const Start& winstart ,
				const WindowType& wintype ,
				 Event * ptr_eve
				);
	/**
	*@brief		The initial of the the window queue.
	*@author		leixiaoluo
	*@date		2011-4-8
	*/
	void SetWindowQueue();
	 /**
	 *@brief	Reset the WQ with the operation of sliding .
	 *@param
	 *@author	leixiaoluo
	 *@date	2011-3-29
	 */
	void SetWQwithSlide(const WindowSize& winsize,
				const SlideStep& winstep,
				const Start& winstart,
				const WindowType& wintype,
				 Event * p_eve
			 );

	 /**
	 *@brief	To get the right event from the district during the given time slot.
	 *@param		_pqevent  The received event source 
	 *@param		t0 and t1  The time slot
	 *@author	leixiaoluo
	 *@date	2011-3-28
	 */
	void EventInDistrict( Cayuga::PriorityQueue::PriorityQueue * _pqevent, 
									const TimeStamp t0, 
									const TimeStamp t1,
									 Event * eve_p);
	 /**
	 *@brief  Using the singleton class to get the PQ and return it to its caller
	 *@author leixiaoluo 
	 *@date 2011-3-24
	 */
	Cayuga::PriorityQueue::PriorityQueue * ReturnPQtoWin(){
		 _PQevent = Cayuga ::PriorityQueue ::PriorityQueue ::getInstance();
		 return _PQevent;
	 }
	 
	 /**
	 *@brief  The  next eight functions could be used for getting the window 
	 parameters.
	 *@param The parameters could be found on the above .
	 *@author leixiaoluo
	 *@date 2011-3-24
	 */
	 void setWindowSize(const float& winSize){
		 _windowsize = winSize;
	 }
	 float getWindowSize(){
		 return _windowsize;
	 }
	 void setWindowType(const WindowType& winType){
		 _windowtype = winType;
	 }
	 WindowType getWindowType(){
		 return _windowtype;
	 }
	 void setWindowStep(const float& winStep){
		_windowstep = winStep;
	 }
	 float getWindowStep(){
		return _windowstep;
	 }
	 void setWindowStart(const float& winStart){
		_windowstart = winStart;
	 }
	 float getWindowStart(){
		 return _windowstart;
	 }
	 void setWindowId(const int& winid){
		 _windowid = winid;
	 }
	 int  getWindowId(){
		 return _windowid;
	 }

	/**
	* @brief Return the pointer to the next event to process.
			 Return NULL if the window queue is empty.
	* @return As above. 
	* @author leixiaoluo
	* @date 2011-3-23
	*/
	Event* peek();

	/**
	* @brief Return the pointer to the next event to process, and consume it.
	* @return As above. 
	* @invariant The queue cannot be empty at this invocation. 
	* @author leixiaoluo
	* @date 2011-3-23
	*/
	Event* get();

   /**
	* @brief Put event into the window queue.
	* @param ev Pointer to the input event
	* @author leixiaoluo
	* @date 2011-3-23
	*/
	void put(Event* ev);

	/**
	* @brief Return true if there is no event in the queue.
	* @return As above.
	* @author leixiaoluo
	* @date 2011-3-23
	*/
	bool empty();

	/**
	* @brief Return the number of elements in the window queue.
	* @return As above.
	* @author leixiaoluo
	* @date 2011-3-23
	*/
	size_t size();
	/**
	*@brief Output member variable information in XML format.
	*@param out The output stream handle.
	*@author leixiaoluo
	*@date 2011-4-9
	*/
	virtual void write(ostream& out = cout) const;

	/**
	*@To point that whether the sliding is over .
	*@luoluo 4-10
	*/
	static bool slide_over;

private:
	/**
	* @brief This is the implementation of peeking event. 
	* @return Same as peek. 
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date May 11, 2006
	* @version 1.0
	*/
	Event* _peekEvent();

	/**
	* This variable stores all the events that have been fed into Cayuga system 
	* but have not been processed.
	*/
	multiset<Event*, endTimeLess> _windowevents;

	/**
	*This variable is used to on behalf of the PQ.
	*/
	Cayuga::PriorityQueue::PriorityQueue * _PQevent;
		
	/**
	* This variable is used to control access to the queue.
	 */
	Mutex _mutex;

	/**
	*This variable is used to identify the id of the window
	*/
    WindowID _windowid;

	/**
	*This variable is used to store the length of the window queue
	*/
    WindowSize _windowsize;

	/**
	*This variable is used to define the type of window 
	*/
	WindowType _windowtype;

	/**
	*This variable is used to appoint the start time or the start 
	tuple
	*/
	Start  _windowstart;

	/**
	*This variable is used to define the slide step at each time
	*/
	SlideStep _windowstep;

	/**
	*@To identify whether the slide operation is beginning .
	*/
	static bool slideornot;

}; //class Window


#endif //_WINDOW_H
