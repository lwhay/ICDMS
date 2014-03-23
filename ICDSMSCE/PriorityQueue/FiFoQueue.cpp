/**
 * @file FifoQueue.cpp
 * @date April 15, 2007
 * @brief Explit instantiations of FifoQueue class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "PriorityQueue/FiFoQueue.h"
#include "PriorityQueue/FiFoQueueDef.h"
using namespace Cayuga;

namespace Cayuga {
	namespace Receiver {
		class Command;
	}
}

#include <string>

template class FifoQueue<Cayuga::Receiver::Command>;
template class FifoQueue<std::string>;
