/**
 * @file inputWindow.h
 * @date 2011-5-18
 * @brief Be heritaged from the Window.h , and it's function is to replace
	the PQ as a input window .

		  There should be one instance of this class in the system, which
		  means we should apply Singleton pattern.

 * @author leixiaoluo
 */

#ifndef  _INPUTWINDOW_H
#define _INPUTWINDOW_H

#include "PriorityQueue/GenericQueue.h"

#include "BasicDataStructure/Event/Event.h"
using namespace Cayuga::BasicDataStructure::EventNS;

#include "Utils/Singleton.h"
using Cayuga::Utils::Singleton;

#include "corelib/os/threads/Mutex.h"
using namespace cudb::corelib::os::threads;

#include "BasicDataStructure/Window/WindowSchema.h"
using namespace Cayuga::BasicDataStructure::WindowSchema;

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



/**
 * @brief An Window object  inherit from the PQ , which is used 
 for processing the Window Operator , On the other hand , it"s 
 a event queue.
 * @author leixiaoluo
 * @date 2011-3-23
 */

class inputWindow : 
    public Singleton<inputWindow>,
	public GenericQueue<Event>
{
	friend class Cayuga::Utils::Singleton<inputWindow>;
public:
	/**
	*@brief		The following two constructor functions are used to 
		form a window queue without the PQ.
	*@date		2011-5-18
	*@author  leixiaoluo
	*/
	inputWindow();

	inputWindow(WindowSize wsize);
	/**
	*@brief		To build the window queue from the AIR information.
	*@author		leixiaoluo
	*@date		2011-5-19
	*/
	inputWindow(WindowID winid ,
				 WindowSize winsize,
				 WindowType wintype,
				 SlideStep winstep,
				 Start winstart);
	
	~inputWindow()
	{
/*		delete _special;*/
	};
    
	//luoluo 8-25
/*	inputWindow  ( inputWindow &other);*/
	inputWindow * operator = (inputWindow * other);
//    
// /**
// 	*@brief Set the window queue .
// 	*@param The there parameters define the new window queue.
// 	*@author leixiaoluo
// 	*@date  2011-5-19
// 	*/
// 	 void SetinputWindowQueue( const WindowSize& winsize , 
// 		        const Start& winstart ,
// 				const WindowType& wintype 
// 				);
// 	/**
// 	*@brief		The initial of the the window queue.
// 	*@author		leixiaoluo
// 	*@date		2011-5-19
// 	*/
// 	void SetinputWindowQueue();
// 	 /**
// 	 *@brief	Reset the WQ with the operation of sliding .
// 	 *@param
// 	 *@author	leixiaoluo
// 	 *@date	2011-5-19
// 	 */
// 	void SetWQwithSlide(const WindowSize& winsize,
// 				const SlideStep& winstep,
// 				const Start& winstart,
// 				const WindowType& wintype
// 			 );


	 /**
	 *@brief  The  next eight functions could be used for getting the window 
	 parameters.
	 *@param The parameters could be found on the above .
	 *@author leixiaoluo
	 *@date 2011-5-19
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
	* @date 2011-5-19
	*/
	Event* peek();

	/**
	* @brief Return the pointer to the next event to process, and consume it.
	* @return As above. 
	* @invariant The queue cannot be empty at this invocation. 
	* @author leixiaoluo
	* @date 2011-5-19
	*/
	Event* get();

   /**
	* @brief Put event into the window queue.
	* @param ev Pointer to the input event
	* @author leixiaoluo
	* @date 2011-5-19
	*/
	void put(Event* ev);

	/**
	* @brief Return true if there is no event in the queue.
	* @return As above.
	* @author leixiaoluo
	* @date 2011-5-19
	*/
	bool empty();

	/**
	* @brief Return the number of elements in the window queue.
	* @return As above.
	* @author leixiaoluo
	* @date 2011-5-19
	*/
	size_t size();
	/**
	*@brief Output member variable information in XML format.
	*@param out The output stream handle.
	*@author leixiaoluo
	*@date 2011-5-19
	*/
	virtual void write(ostream& out = cout) const;

	/**
	*@To point that whether the sliding is over .
	*@luoluo 5-19
	*/
	static bool slide_over;

	//luoluo 8-18
	static bool Allover;

	/**
	* This variable stores all the events that have been fed into Cayuga system 
	* but have not been processed.
	*/
	multiset<Event*, endTimeLess> _windowevents;

	/**
	*@brief		Before the slide operation , the part which
					exceed the window should be delete .
	*@author	leixiaoluo
	*@date		2011-6-21
	*/
	EventID SlidePoint(multiset<Event * , endTimeLess>::iterator wq_ptr);

	/**
	*@brief		Using the mutex to let the communication between client and 
					server on queue more rightly.
	*@author luoluo 7-13
	*/
	void  lock_unlock();

	//luoluo 8-12
	//the only pointer point to the queue.
	 multiset<Event* , endTimeLess> ::iterator _special;
	//to check whether all the events of this queue have being processed.
	bool  queue_over();
	//to erase the even exceed the window.
	void  erase_queue(multiset<Event* , endTimeLess>::iterator first, 
		multiset<Event* ,endTimeLess>::iterator second);
	//the cut-off line
	multiset<Event*,endTimeLess>::iterator getEraseLine(multiset<Event * , endTimeLess>::iterator wq_ptr);

	//luoluo 8-13
	//copy the righ inputwindow's _windowevents into this window.
	void copyTwoWinEv(vector<string> righ);

	/**
	* This variable is used to control access to the queue.
	 */
	Mutex _mutex;

	//luoluo 8-25
	static int  _indicateLine;



private:
	/**
	* @brief This is the implementation of peeking event. 
	* @return Same as peek. 
	* @author leixiaoluo
	* @date 2011-5-19
	*/
	Event* _peekEvent();
		

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

	//luoluo 8-16
	static bool indicateStart;

};

#endif
