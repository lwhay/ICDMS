/**
 * @file StreamID.h
 * @date November 17, 2005
 * @brief Definition of a stream ID.
		  
		  Put in a separate file to make it easy for other files to include it.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _STREAM_ID_H
#define _STREAM_ID_H

namespace Cayuga {
namespace BasicDataStructure {
typedef int StreamID;

#define BAD_STREAM_ID -1
}  //namespace BasicDataStructure
}  //namespace Cayuga


#endif //_STREAM_ID_H
