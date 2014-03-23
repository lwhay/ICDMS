/**
 * @file EventID.h
 * @date April 28, 2007
 * @brief Definition of an event ID.
		  
		  Put in a separate file to make it easy for other files to include it.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _EVENT_ID_H
#define _EVENT_ID_H

namespace Cayuga {
namespace BasicDataStructure {
namespace EventNS {
typedef long long EventID;

#define BAD_EVENT_ID -1
}  //namespace EventNS
}  //namespace BasicDataStructure
}  //namespace Cayuga


#endif //_EVENT_ID_H
