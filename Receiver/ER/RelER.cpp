/**
* @file RelER.cpp
* @date April 4, 2007
* @brief This file explicitly instantiates a few uses of RelER.
* @author Mingsheng Hong (mshong@cs.cornell.edu)
*/

#include "../Receiver/ER/RelER.h"
//#include "../Receiver/ER/RelERDef.h"
#include "../Receiver/Options/FileReceiverOptions.h"
#include "../Receiver/Options/NetworkEROptions.h"
using namespace Cayuga::Receiver;

template class RelER<FileReceiverOptions>;

#include "../ICDSMSCE/BasicDataStructure/Event/Event.h"
#include "../ICDSMSCE/BasicDataStructure/StreamID.h"
using namespace Cayuga::BasicDataStructure::EventNS;
using Cayuga::BasicDataStructure::StreamID;
#include "../ICDSMSCE/AutomatonCompiler/convertConstToByteArray.h"

/*#include "../ICDSMSCE/PriorityQueue/PriorityQueue.h"*/
#include "../ICDSMSCE/BasicDataStructure/Window/inputWindow.h"
#include "../ICDSMSCE/BasicDataStructure/Automaton/Query.h"

#include "../ICDSMSCE/SystemMgr/DataTypeMgr.h"
#include "../ICDSMSCE/SystemMgr/StreamSchemaMgr.h"
using namespace Cayuga::SystemMgr;

#include "../ICDSMSCE/Utils/AttrMap.h"
#include "../ICDSMSCE/Utils/split.h"
using namespace Cayuga::Utils;

#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

namespace Cayuga {
namespace Receiver {
void parseEvent(const string& line, char charDelim, bool bStrict) {
	vector<string> fields;
	split(line, fields, charDelim);
	//the last three fields are respectively stream name, start and
	//end timestamps
	Event* ev;
	try {
		if (fields.size() < 3) {
			//cerr<<"The stream file is ill-formed."<<endl;    
			throw string("cannot parse event "+line);
		}

		//now consult inputStreamName to see how to convert fields
		StreamID streamID = StreamSchemaMgr::getInstance()->
			getStreamID(fields[fields.size()-3]);
		const StreamSchema* pSchema = StreamSchemaMgr::getInstance()->
			getStreamSchema(streamID);

		const size_t szSchema = pSchema->getInternalSchemaSize();
		ev = Event::allocEvent(szSchema);

		ev->setStartTime(atoi(fields[fields.size()-2].c_str()));
		ev->setEndTime(atoi(fields[fields.size()-1].c_str()));
		ev->setStreamID(streamID);

		char* evArray = ev->getBody();
		size_t offset = 0;
		size_t numAttributes = pSchema->getNumAttributes();

		//the last two attributes are start and end timestamps
		assert (numAttributes+3 == fields.size());
		for (AttributeID attrID = 0; attrID < numAttributes; ++attrID) {
			size_t attrLen = pSchema->getInternalAttributeSize(attrID);

			AttrTypeID attrType = pSchema->getAttributeType(attrID);
			string attrTypeName 
				= DataTypeMgr::getInstance()->getName(attrType);
			Cayuga::AutomatonCompiler::convertConstToByteArray(
				attrTypeName,
				fields[attrID],
				attrLen,
				evArray+offset,
				false,	//do not ping string constants from events
				true	//put in external space
				); 
			offset += attrLen;
		}
	}
	catch (const string& s) {
		//cerr<<"event from stream "<<fields[fields.size()-3]
		//	<<" cannot be internalized."<<endl;
		cerr<<s<<endl;
		if (bStrict) {
			throw s;
		}
		else return;
	}
    
// 	 	//luoluo 5-25

// // 	Cayuga::PriorityQueue::PriorityQueue::getInstance()->put(ev);
// 	inputWindow::getInstance()->put(ev);
		//luoluo 5-26
// 	Query * pQuery = new Query();
// 	(*(pQuery->_inputwindows.begin()))->put(ev);
		//luoluo 5-28
/*	inputWindow * ptr = inputWindow ::getInstance();*/
	inputWindow ::getInstance()->put(ev);
}
} //namespace Receiver
} //namespace Cayuga 
