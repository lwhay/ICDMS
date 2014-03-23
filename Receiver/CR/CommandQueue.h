	/**
 * @file CommandQueue.h
 * @date March 23, 2007
 * @brief A FIFO queue for storing the commmands to be processed by the engine.

		  There should be one instance of this class in the system, which
		  means we should apply Singleton pattern.

 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _COMMAND_QUEUE_H
#define _COMMAND_QUEUE_H

#include "../ICDSMSCE/PriorityQueue/FiFoQueue.h"
#include "../ICDSMSCE/PriorityQueue/FiFoQueueDef.h"

#include "../ICDSMSCE/Utils/Singleton.h"
using Cayuga::Utils::Singleton;

namespace Cayuga {
namespace Receiver {
class Command;
class CommandQueue;

/**
 * @brief An CommandQueue object stores the Cayuga commands in FIFO order.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date March 23, 2007
 * @version 1.0
 */
class CommandQueue : public FifoQueue<Command>, 
	public Singleton<CommandQueue> {
	friend class Cayuga::Utils::Singleton<CommandQueue>;
}; //class CommandQueue
}  //namespace Receiver
}  //namespace Cayuga

#endif //_COMMAND_QUEUE_H
