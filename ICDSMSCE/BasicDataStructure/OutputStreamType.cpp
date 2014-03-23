/**
 * @file OutputStreamType.cpp
 * @date November 27, 2005
 * @brief Implementation file of OutputStreamType class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "BasicDataStructure/OutputStreamType.h"
using namespace Cayuga::BasicDataStructure;

#include "SystemMgr/StreamSchemaMgr.h"
using namespace Cayuga::SystemMgr;

OutputStreamType::OutputStreamType(StreamID thisSid, 
				 bool thisbResubscribed, 
				 bool thisbOutput) 
				 : sid(thisSid), 
				 bResubscribed(thisbResubscribed),
				 bOutput(thisbOutput) {
}

void OutputStreamType::write(ostream& out) const {
	out<<"<OutputStreamName Name=\"";
	out<<StreamSchemaMgr::getInstance()->getStreamName(sid)<<"\"";
	if (bOutput) {
		out<<" "<<"Type=\"OUT\"";
	}
	else {
		out<<" "<<"Type=\"RESUB\"";
	}
	out<<"/>"<<endl;
}
