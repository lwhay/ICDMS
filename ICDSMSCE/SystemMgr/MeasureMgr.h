/**
 * @file MeasureMgr.h
 * @date March 9, 2007
 * @brief This file defines a measurement manager for producing continuous 
		measures of Cayuga (e.g. throughput, memory consumption, etc) during 
		event processing.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _MEASURE_MGR_H
#define _MEASURE_MGR_H

#include "BasicDataStructure/Event/EventID.h"
using Cayuga::BasicDataStructure::EventNS::EventID;

#include "BasicDataStructure/Timestamp.h"
using Cayuga::BasicDataStructure::TimeStamp;

#include "Utils/Singleton.h"
#include "Utils/Timer.h"

#include <fstream>
#include <vector>
using namespace std;

namespace Cayuga {

namespace Sender {
class NetworkSender;
}
using Cayuga::Sender::NetworkSender;

namespace SystemMgr {
using Cayuga::Utils::Singleton;
using Cayuga::Utils::crTime;

enum MeasureType {
		EP_TIME,
		GC_TIME,
		USED_HEAP_SIZE,
};

/**
 * @brief A MeasureMgr is responsible for producing measures of Cayuga engine.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date March 9, 2007
 * @version 1.0
 */
class MeasureMgr;
class MeasureMgr : public Singleton<MeasureMgr>
{
	friend class Cayuga::Utils::Singleton<MeasureMgr>;
public:
	/**
	* @brief Constructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date March 9, 2007
	* @version 1.0
	*/
	MeasureMgr();

	/**
	* @brief Destructor.Write the measurements out to disk files
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date March 9, 2007
	* @version 1.0
	*/
	~MeasureMgr();

	/**
	* @brief Start the time measurement for the input event ID
	* @param eventID The current event ID being processed
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date March 9, 2007
	* @version 1.0
	*/
	void startTimeMeasure(EventID eventID);

	/**
	* @brief stop the time measurement
	* @param m The measure type
	* @param bStoreCost If bStoreCost is true, dump it to a disk file.
	* @invariant startTimeMeasure must have been called before this invocation
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date March 9, 2007
	* @version 1.0
	*/
	void stopTimeMeasure(MeasureType m, bool bStoreCost);

	/**
	* @brief dump the measurement to one or multiple disk files
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date March 9, 2007
	* @version 1.0
	*/
	//void dumpMeasure();

	/**
	* @brief Send out a status message if MeasurePort config param is non-0.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 23, 2007
	* @version 1.0
	*/
	void sendMeasure();

	/**
	* @brief record the measurement to a corresponding disk file
	* @param eventID The current event ID being processed
	* @param totalNumInstances The total number of instances 
	* @param numInstancesPerState The number of instances per intermediate 
			 NFA state
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date March 13, 2007
	* @version 1.0
	*/
	void recordInstanceMeasure(EventID eventID, 
		size_t totalNumInstances, 
		const vector<size_t>& numInstancesPerState);

private:

	/** each vector element is a pair of eventID and time cost in ms */
	//vector<pair<long long, long long> > _timeCosts;

	/** timer to be started at the invocation of MeasureMgr::startTimeMeasure */
	crTime _timer;

	/** the current event ID */
	EventID _eventID;

	/** the output handle to event processing costs */
	ofstream _ep_out;

	/** the output handle to gc costs */
	ofstream _gc_out;

	/** the output handle to number of NFA instances */
	ofstream _inst_out;

	/** This variable indicates whether to send measures via TCP socket */
	bool _bSendMeasure;

	/** This variable storeds the event window length. */
	//unsigned _eventWindowLen;
	
	/** This variable stores the number of events processed so far. When 
	*	it reaches _eventWindowLen, it is reset to 0, and an output status msg
	*   for time cost will be sent out
	*/
	//unsigned _numEventsProcessed;

	/** This variable stores the number of eopoches processed so far. When 
	*	it reaches _eventWindowLen, it is reset to 0, and an output status msg
	*   for number of instances will be sent out
	*/
	//unsigned _numEpochesProcessed;

	/** This variable storeds the sum of event processing costs in the 
	*   current event window.
	*/
	TimeStamp _epCostInWindow;
	
	/** 
	* This variable a pointer to the network output event sender.
	*/
	NetworkSender* _pStatusSender;

}; //class MeasureMgr
}  //namespace SystemMgr
}  //namespace Cayuga

#endif //_MEASURE_MGR_H
