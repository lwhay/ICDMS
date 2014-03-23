/**
 * @file OutputStreamType.h
 * @date April 22, 2007
 * @brief This file declares and defines OutputStreamType.		  
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _OUTPUT_STREAM_STREAM_H
#define _OUTPUT_STREAM_STREAM_H

#include "BasicDataStructure/StreamID.h"
using Cayuga::BasicDataStructure::StreamID;

#include <iostream>
using namespace std;

namespace Cayuga {
namespace BasicDataStructure {
/**
* @brief An OutputStream object represents the status of an output stream 
in the final state of an automaton. 
* @author Mingsheng Hong (mshong@cs.cornell.edu)
* @date March 12, 2007
* @version 1.0
*/
struct OutputStreamType {
	/**
	* @brief Default constructor
	* @param thisSid Output stream ID.
	* @param thisbResubscribed Whether this output stream is to be 
			 resubscribed or not.
	* @param thisbOutput Whether this output stream corresponds to an exnternal
			 query output stream.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date March 12, 2007
	* @version 1.0
	*/
	OutputStreamType(StreamID thisSid, 
		bool thisbResubscribed, 
		bool thisbOutput);

	/**
	* @brief Output member variable information in XML format.
	* @param out The output stream handle.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date March 12, 2007
	* @version 1.0
	*/
	void write(ostream& out = cout) const;

	/**
	sid is the output stream ID. Multiple
	output streams can share the same output stream ID. 
	*/
	StreamID sid;

	/** bResubscribed is true iff this output stream is to be resubscrbed
	to by some other automaton in the same machine. If this is the case,
	each output event on this stream will be put back into Cayuga priority
	queue. 

	bResubscribed will be deduced by Cayuga automaton compiler as follows. 
	When an output stream s is initially added into and end state,  
	s.bResubscribed is set to false. The StreamSchemaMgr will remember 
	the set of "generating" states of this output stream. 
	Later when another query that read this output stream is inserted into
	Cayuga, for each end state that contains this output stream, 
	s.bResubscribed will be set to true.
	*/
	bool bResubscribed;

	/** bOutput is true iff this output stream is to be output to an 
	external place, such as TCP network or a disk file. 

	bOutput is set by the XML attribute Type="OUT" in the Node element
	of the AIR file corresponding to the NFA. */
	bool bOutput;
};
}  //namespace BasicDataStructure
}  //namespace Cayuga

#endif //_OUTPUT_STREAM_STREAM_H
