/**
 * @file inputWindow.cpp
 * @brief Implementation file of Window class.
 * @author leixiaoluo
 * @date  2011-5-21
 */
#include "BasicDataStructure/Window/inputWindow.h"
#include "BasicDataStructure/Event/EventFinder.h"
#include "BasicDataStructure/Event/Event.h"
using namespace Cayuga::BasicDataStructure::EventNS;

#include "BasicDataStructure/Timestamp.h"
using Cayuga::BasicDataStructure::TimeStamp;

#include "SystemMgr/DataTypeMgr.h"
#include "SystemMgr/StreamSchemaMgr.h"
using namespace Cayuga::SystemMgr;
#include "MemoryMgr/CY_HeapFace.h"
#include "MemoryMgr/CY_IntStringFace.h"
using namespace CY_IntString;
#include "AutomatonCompiler/convertConstToByteArray.h"

#include "SystemMgr/DataTypeMgr.h"
#include "SystemMgr/StreamSchemaMgr.h"
using namespace Cayuga::SystemMgr;

#include "Utils/AttrMap.h"
#include "Utils/split.h"
using namespace Cayuga::Utils;

#include "SAXHandler/ConfigMgr.h"
using namespace Cayuga::SAXHandler;

#include "SystemMgr/Options.h"
using namespace Cayuga::SystemMgr;
#include <set>
#include <cassert>
using namespace std;

bool inputWindow::indicateStart=true;
bool  inputWindow ::slide_over=true;
/*multiset<Event* , endTimeLess>::iterator inputWindow::_special ;*/
bool inputWindow::Allover =false;
int inputWindow :: _indicateLine = 0;

inputWindow::inputWindow() {
	CY_Heap::registerRootFinder( new EventFinder(_windowevents) );
	_special = _windowevents.begin();
}

//luoluo 4-8
inputWindow ::inputWindow(WindowID winid , 
				WindowSize winsize, 
				WindowType wintype, 
				SlideStep winstep, 
				Start winstart) :_windowid(winid),_windowsize(winsize),
				_windowtype(wintype),_windowstart(winstart),
				_windowstep(winstep){
					CY_Heap::registerRootFinder( new EventFinder(_windowevents) );
}


inputWindow * inputWindow::operator = (inputWindow * other)
{
	multiset<Event* ,endTimeLess>::iterator copybegin = other->_windowevents.begin();
	for (;copybegin != other->_windowevents.end();++copybegin)
	{
		// 		Event newEvent(** copybegin) ;
		// 		_windowevents.insert(&newEvent);

		const StreamSchema* pSchema = StreamSchemaMgr::getInstance()->
			getStreamSchema((*copybegin)->getStreamID());
		const size_t szSchema = pSchema->getInternalSchemaSize();
		//  		Event * newEvent = (Event*)malloc(sizeof(Event)+szSchema);
		// 		(*newEvent) = ** copybegin;
		// 		_windowevents.insert(newEvent);
		// /*		delete newEvent;*/
		Event * newEvent = Event::allocEvent(szSchema);
		Event * ev = * copybegin;
		newEvent->setStartTime(ev->getStartTime());
		newEvent->setEndTime(ev->getEndTime());
		newEvent->setStreamID(ev->getStreamID());
		newEvent->setID(ev->getID());

		char * evbody = ev->getBody();
		char* copystring=NULL;
		size_t offset1=0;

		char* evArray = newEvent->getBody();
		size_t offset = 0;
		size_t numAttributes = pSchema->getNumAttributes();

		for (AttributeID attrID = 0; attrID < numAttributes; ++attrID) {
			size_t attrLen = pSchema->getInternalAttributeSize(attrID);

			AttrTypeID attrType = pSchema->getAttributeType(attrID);
			string attrTypeName 
				= DataTypeMgr::getInstance()->getName(attrType);

			if (attrTypeName == "int")copystring = (evbody+offset1);
			else if(attrTypeName == "float")copystring = (evbody+offset1);
			else if(attrTypeName == "string")
			{
				IntStringHandle h = *(IntStringHandle*)(ev+offset1);
				copystring = const_cast<char*>(h->getBody());
			}
			offset1 +=attrLen;
			string str(copystring);

			Cayuga::AutomatonCompiler::convertConstToByteArray(
				attrTypeName,
				str,
				attrLen,
				evArray+offset,
				false,	//do not ping string constants from events
				true	//put in external space
				); 
			offset += attrLen;
		}
		_windowevents.insert(newEvent);
	}
	return this;
}

Event* inputWindow::_peekEvent() {
	//This function is called by the public APIs peek and get above, 
	//so mutex is used in that level, not here.
// 	if (_windowevents.empty()) {
// 		return NULL;
// 	}

// 	multiset<Event*, endTimeLess>::iterator it = _windowevents.begin();
// 	Event* pThisEvent = *it;
// 	return pThisEvent;
	//luoluo 8-12
	if (_special == _windowevents.end())return NULL;
// 	multiset<Event*, endTimeLess>::iterator it = _special;
// 	Event* pThisEvent = *it;
	return *_special;
}

Event* inputWindow::peek() {
	Lock lock = _mutex.scopedLock();
	return _peekEvent();
}

Event* inputWindow::get() {
	Lock lock = _mutex.scopedLock();
	Event* pThisEvent = _peekEvent();

	assert (pThisEvent);
	++_special;
/*	_windowevents.erase(_windowevents.begin());	*/
	return pThisEvent;
}

void inputWindow::put(Event* ev) {
	Lock lock = _mutex.scopedLock();

	multiset<Event * ,endTimeLess >::iterator expr;
	expr = _windowevents.insert(ev);
/*	if (_special== _windowevents.end())_special = expr;*/
	if (indicateStart)
	{
		_special = expr;
		indicateStart = false;
	}

	
}

bool inputWindow::empty() {
	Lock lock = _mutex.scopedLock();
	return _windowevents.empty();
}

size_t inputWindow::size() {
	Lock lock = _mutex.scopedLock();
	return _windowevents.size();
}

void inputWindow::write(ostream& out) const {
	out<<"<Window  WindowID=\""<<_windowid<<"\" "<<"WindowType=\"";
	switch(_windowtype){
		case 0 : out<<"TIMEWINDOW";break;
		case  1: out<<"COUNTWINDOW";break;
	}
	out<<"\" "<<"WindowSize=\""<<_windowsize<<"\" "<<"SlideStep=\""<<_windowstep<<"\" "
		<<"Start=\""<<_windowstart<<"\" >"<<endl;
	out<<"</Window>"<<endl;
}

EventID inputWindow ::SlidePoint(multiset<Event * , endTimeLess>::iterator wq_ptr){
	float wsize =this->getWindowSize();
	float slidesize = this->getWindowStep();
	EventID eventid;
	switch(this->getWindowType()){
		case  0		:
			//the following add when the sender can process timewindow.
			break;
		case  1 :
		while(slidesize--)++wq_ptr;
			eventid = (*wq_ptr)->getID();
			break;
	}
	return eventid;
}


bool inputWindow::queue_over()
{
	bool i = (_special == _windowevents.end()) ? 1 : 0;
	return i;
}

void  inputWindow::erase_queue(multiset<Event* , endTimeLess>::iterator first, 
				  multiset<Event* ,endTimeLess>::iterator second)
{
	Lock lock = _mutex.scopedLock();
	for (;first !=second;)
	{
		multiset<Event* ,endTimeLess>::iterator ptr = first;
		++first;
		Event::freeEvent(*ptr);
		_windowevents.erase(ptr);
	}
	Event::freeEvent(*second);
	_windowevents.erase(second);

	//luoluo 8-16
	indicateStart = true;
	_special = _windowevents.end();
}

multiset<Event*,endTimeLess>::iterator inputWindow::getEraseLine(multiset<Event * , endTimeLess>::iterator wq_ptr)
{
	float slidesize = this->getWindowStep();
	while(--slidesize)++wq_ptr;
	return wq_ptr;
}

void inputWindow::copyTwoWinEv(vector<string> righ)
{
	assert(&righ);
	char _delim	 = ConfigMgr::getInstance()->getOptions()->_attrDelimiter;
	bool _bStrict = ConfigMgr::getInstance()->getOptions()->_bStrict;
	for (vector<string>::iterator iterat = righ.begin(); 
									iterat != righ.end(); ++iterat)
	{
		 vector<string> fields;
		string line = *iterat;

// 		//for test.
// 		cout<<line<<"---------"<<endl;

		split(line, fields, _delim);
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

			if (!(numAttributes+3 == fields.size()))cout<<")()()()()()()("<<"the receive size are not equal!"<<endl;

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
			if (_bStrict) {
				throw s;
			}
			else return;
		}

		this->put(ev);
	}

// 	multiset<Event*, endTimeLess>::iterator loop = righ->_windowevents.begin();
// 	for (;loop != righ->_windowevents.end();)
// 	{
// 		const StreamSchema* pSchema = StreamSchemaMgr::getInstance()->
// 			getStreamSchema((*loop)->getStreamID());
// 		const size_t szSchema = pSchema->getInternalSchemaSize();
// 		Event * newEvent = Event::allocEvent(szSchema);
// 		Event * ev = * loop;
// 		newEvent->setStartTime(ev->getStartTime());
// 		newEvent->setEndTime(ev->getEndTime());
// 		newEvent->setStreamID(ev->getStreamID());
// 		newEvent->setID(ev->getID());
// 
// 		char * evbody = ev->getBody();
// 		char* copystring=NULL;
// 		size_t offset1=0;
// 
// 		char* evArray = newEvent->getBody();
// 		size_t offset = 0;
// 		size_t numAttributes = pSchema->getNumAttributes();
// 
// 		for (AttributeID attrID = 0; attrID < numAttributes; ++attrID) {
// 			size_t attrLen = pSchema->getInternalAttributeSize(attrID);
// 
// 			AttrTypeID attrType = pSchema->getAttributeType(attrID);
// 			string attrTypeName 
// 				= DataTypeMgr::getInstance()->getName(attrType);
// 
// 			if (attrTypeName == "int")copystring = (evbody+offset1);
// 			else if(attrTypeName == "float")copystring = (evbody+offset1);
// 			else if(attrTypeName == "string")
// 			{
// 				IntStringHandle h = *(IntStringHandle*)(ev+offset1);
// 				copystring = const_cast<char*>(h->getBody());
// 			}
// 			offset1 +=attrLen;
// 			string str(copystring);
// 
// // 			//for test.
// // 			cout<<str<<"   +++++++"<<endl;
// 
// 			Cayuga::AutomatonCompiler::convertConstToByteArray(
// 				attrTypeName,
// 				str,
// 				attrLen,
// 				evArray+offset,
// 				false,	//do not ping string constants from events
// 				true	//put in external space
// 				); 
// 			offset += attrLen;
// 		}
// 		
// // 		//for test.
// // 		cout<<(char*)(newEvent->getBody())<<"   +++++++"<<endl;
// 
// /*		_windowevents.insert(newEvent);*/
// 		this->put(newEvent);
// 
// 		++loop;
// 	}
}

