/**
 * @file inputWindow.h
 * @date 2011-5-18
 * @brief Be heritaged from the Window.h , and it's function is to replace
	the PQ as a input window .

		  There should be one instance of this class in the system, which
		  means we should apply Singleton pattern.

 * @author leixiaoluo
 */

#ifndef  _OUTPUTWINDOW_H
#define _OUTPUTWINDOW_H

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

class outputWindow : 
    public Singleton<outputWindow>,
	public Window
{



};

#endif