/**
* @file MeasureMgr.cpp
* @date March 9, 2007
* @brief This implementation file of class MeasureMgr.
* @author Mingsheng Hong (mshong@cs.cornell.edu)
*/

#include <cassert>
#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

#include "SystemMgr/MeasureMgr.h"
#include "SystemMgr/Options.h"
using namespace Cayuga::SystemMgr;

#include "BasicDataStructure/CMessage.h"
using Cayuga::BasicDataStructure::CMessage;

#include "CayugaSystem.h"
using Cayuga::CayugaSystem;

#include "PriorityQueue/PriorityQueue.h"
using namespace Cayuga::PriorityQueue;

#include "MemoryMgr/CY_IntStringFace.h"
#include "MemoryMgr/CY_HeapFace.h"

#include "SAXHandler/ConfigMgr.h"
using namespace Cayuga::SAXHandler;

#include "Utils/AttrMap.h"
using namespace Cayuga::Utils;

#include "../Sender/NetworkSender.h"
using namespace Cayuga::Sender;

MeasureMgr::MeasureMgr() : _timer("ep_time_cost"), _eventID(-1),
	/*_numEventsProcessed(0), _numEpochesProcessed(0),*/
	_epCostInWindow(0), _pStatusSender(NULL) {

		if (!ConfigMgr::getInstance()->getOptions()->_bMeasure) return;

		_ep_out.open("ep_cost.txt");
		_gc_out.open("gc_cost.txt");
		_inst_out.open("inst.txt");

		_bSendMeasure 
			= (ConfigMgr::getInstance()->getOptions()->_nMeasurePort != 0);
		//_eventWindowLen 
		//	= ConfigMgr::getInstance()->getOptions()->_eventWindowLen;

		if (_bSendMeasure) {
			_pStatusSender = new NetworkSender(
				ConfigMgr::getInstance()->getOptions()->_nMeasurePort,
				"Measure Sender",
				CayugaSystem::getInstance()->debugOstream());
		}
}

MeasureMgr::~MeasureMgr() {
	delete _pStatusSender;
}

void MeasureMgr::startTimeMeasure(EventID eventID) {
	_timer.resetAndStart();
	_eventID = eventID;
}

void MeasureMgr::stopTimeMeasure(MeasureType m, bool bStoreCost) {
	_timer.stop();
	TimeStamp thisCost = _timer.returnTimerInMicroSeconds();
	
	assert (_eventID >= 0);
	if (bStoreCost) {
		//_timeCosts.push_back(make_pair(_eventID, thisCost));

		switch (m) {
			case EP_TIME:
				_ep_out<<_eventID<<"\t"<<thisCost<<endl;
				//++_numEventsProcessed;
				_epCostInWindow += thisCost;
				break;
			case GC_TIME:
				_gc_out<<_eventID<<"\t"<<thisCost<<endl;
				break;
			default:
				cerr<<"not implemented"<<endl;
				break;
		}
	}
}

void MeasureMgr::sendMeasure() {
	if (!_bSendMeasure) return;
		
	CMessage statusMsg("STATUS", 
		ConfigMgr::getInstance()->getOptions()->_attrDelimiter);
	
	//the current Cayuga time
	TimeStamp curTime = time (NULL);
	statusMsg.appendField("CTIME");
	statusMsg.appendField(toString(curTime));
	
	//the sum of EP cost
	statusMsg.appendField("EP_COST");
	statusMsg.appendField(toString(_epCostInWindow));
	
	size_t heapConsumption = CY_Heap::getNumLiveManualBytes();

	//add the usage by IntSringTbl
	unsigned numIntSrings, numIntStringBytes, numLookups, numFails, numInstalls;
	CY_IntString::getStatistics(&numIntSrings, &numIntStringBytes, &numLookups, 
		&numFails, &numInstalls);
	heapConsumption += numIntStringBytes;

	statusMsg.appendField("HEAP_SIZE");
	statusMsg.appendField(toString(heapConsumption));

	size_t numUnprocessedEvents 
		= PriorityQueue::PriorityQueue::getInstance()->size();
	size_t numInstances = CY_Heap::getNumLiveManualObjects() - numUnprocessedEvents;
	statusMsg.appendField("NUM_INST");
	statusMsg.appendField(toString(numInstances));

	statusMsg.appendField("EQ_LEN");
	statusMsg.appendField(toString(numUnprocessedEvents));

	//send it out
	ostringstream ostr;
	statusMsg.write(ostr);
	_pStatusSender->send(new string(ostr.str()));

	//reset
	_epCostInWindow = 0;
}

//void MeasureMgr::dumpMeasure() {
//	for (vector<pair<long long, long long> >::const_iterator it 
//		= _timeCosts.begin(); it != _timeCosts.end(); ++it) {
//			long long eventID = it->first;
//			long long cost = it->second;
//			out<<eventID<<"\t"<<cost<<endl;
//	}
//}

void MeasureMgr::recordInstanceMeasure(EventID eventID, 
							   size_t totalNumInstances, 
							   const vector<size_t>& numInstancesPerState) {
	_inst_out<<eventID<<"\t"<<totalNumInstances;
	for(vector<size_t>::const_iterator it = numInstancesPerState.begin();
		it != numInstancesPerState.end(); ++it) {
			_inst_out<<"\t"<<*it;
	}
	_inst_out<<endl;
}
