/**
 * @file SAXEventType.h
 * @date November 22, 2005
 * @brief Definition of the SAXEventType.
		  
		  Put in a separate file to make it easy for other files to include it.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _SAX_EVENT_TYPE_H
#define _SAX_EVENT_TYPE_H

namespace Cayuga {
namespace SAXHandler {
/**
 * @brief A SAXEventType object represents the type of a SAX event.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 22, 2005
 * @version 1.0
 */
enum SAXEventType {
	START_ELEM,
	END_ELEM,
	CHUNK,
}; //enum SAXEventType
}  //namespace SAXHandler
}  //namespace Cayuga

#endif //_SAX_EVENT_TYPE_H
