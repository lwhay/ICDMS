/**
 * @file QueryProcessorImpl.h
 * @date November 17, 2005
 * @brief QueryProcessorImpl is the implementation class of a query processor.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _QUERY_PROCESSOR_IMPL_H
#define _QUERY_PROCESSOR_IMPL_H

#include "arch-dep.h"

#include "BasicDataStructure/Automaton/NodeID.h"
using Cayuga::BasicDataStructure::Automaton::NodeID;

#include "BasicDataStructure/Event/EventID.h"
using Cayuga::BasicDataStructure::EventNS::EventID;

#include "Utils/Timer.h"
using namespace Cayuga::Utils;

#include "BasicDataStructure/Window/predWindow.h" 

#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <map>
using namespace std;


namespace Cayuga {
namespace BasicDataStructure {
namespace Automaton {
class Node;
class IntermediateNode;
class Query;
class Instance;
}

namespace EventNS {
class Event;
}
}

namespace SystemMgr {
class MeasureMgr;
class Options;
}

using Cayuga::SystemMgr::MeasureMgr;
using Cayuga::SystemMgr::Options;

namespace Sender {
class NetworkSender;
}
using Cayuga::Sender::NetworkSender;

namespace QueryProcessor {
using Cayuga::BasicDataStructure::EventNS::Event;
using namespace Cayuga::BasicDataStructure::Automaton;

class FilterEvaluator;
class FREvaluator;
/**
 * @brief A QueryProcessorImpl object implements query processor
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 17, 2005
 * @version 1.0
 */
class QueryProcessorImpl {
public:
	friend class FilterEvaluator;
	friend class FilterIndex;
	friend class FREvaluator;


	/**
	* @brief Default constructor
	* @param debugOut The debug message output stream
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 17, 2005
	* @version 1.0
	*/
	QueryProcessorImpl(ostream& debugOut = cout);
	
	/**
	* @brief Destructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 4, 2007
	* @version 1.0
	*/
	~QueryProcessorImpl();

	/**
	* @brief Read the queries from input query files and add them into the 
			 engine.
	* @param queryFileNames The names of the files storing queries to be loaded
	* @param bIsAIR True iff the input set of queries in in AIR form.
				Otherwise they are in CEL form.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 4, 2007
	* @version 1.0
	*/
	int addQueries(const vector<string>& queryFileNames,
		bool bIsAIR);

	/**
	* @brief Start event processing
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 17, 2005
	* @version 1.0
	*/
	void startEventProcessing();

	/**
	* @brief Send the output event to appropriate places, such as trace, 
			TCP socket, and console screen.
	* @param pEndNode The end node from which the output event is generated
	* @param pWitnessInstance The output event to send
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 14, 2007
	* @version 1.0
	*/
	void sendOutputEvent(const IntermediateNode* pEndNode, 
		const Instance* pWitnessInstance);
	
	/**
	* @brief Print statistics of the QP.
	* @param out The output stream handle
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 25, 2007
	* @version 1.0
	*/
	void printStats(ostream& out);

//luoluo3.13
	long return_out_event();
	/**
	*@brief		This variable stores the relation between the event and instance .
	Although maybe the link is chaos , if treated carefully it is good for the index 
	between the two elements.
	*@luoluo 4-7
	*/
	multimap<EventID ,Instance*> _linkEvIns;
	/**
	*@This variable stores the time during the processing .
	*@Just for testing .
	*@luoluo 4-18
	*/
	Timer _process_time;
	/**
	*@This variable stores the total numbers of the processing events 
		because of the window operation .
		@@@There is some different . The sliding have also being calculated.
	*@Just for testing .
	*/
	 float _total_events;

	 /**
	 *@brief	The struct wad used to store the information  on sending .
	 *@date	2011-6-17
	 *@author luoluo
	 */
	 typedef struct sendBuffer {
		 bool pred;
		 float swise;
	 }MDATA ;
	 MDATA * pSendInfo;

	 /**
	 *@brief	Erase the relation between the event and instacne.
	 *@author	leixiaoluo
	 *@date	2011-6-20
	 */
	 void eraseRelationEvInst(
		 multimap<EventID , Instance *> ::iterator &first,
		 multimap<EventID , Instance *> ::iterator &last);

private:
	/**
	* @brief Process the input event
	* @param ev The current event content to process
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 17, 2005
	* @version 1.0
	*/
	void _procOneEvent(/*const*/ Event* ev);
	
	/**
	* @brief Install new instances created during this event processing epoch.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 17, 2005
	* @version 1.0
	*/
	void _installNewInstances();
	
	/**
	* @brief Process incoming commands in CommandQueue.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date March 25, 2007
	* @version 1.0
	*/
	void _processCommands();

	/**
	* This variable stores the current continous query DAG.
    */
	Query* _nfa_dag;

	/**
	* This variable stores the pointer to the filter evaluator component.
    */
	FilterEvaluator* _pFilterEvaluator;
	
	/**
	* This variable stores the pointer to the FR evaluator component.
    */
	FREvaluator* _pFrEvaluator;
	
	/** 
	* This variable stores the set of affected nodes for the current event.
	* This is set by FilterEvaluator. 
	*/
	hash_set<NodeID> _affectedNodes;

	/** 
	* This variable stores the set of nodes with instances in the system. 
	*/
	hash_set<NodeID> _nodesWithInstance;

	/** 
	* This variable stores the set of nodes touched during this event processing
	* epoch.
	*/
	hash_set<NodeID> _nodesTouchedInEpoch;

	/** 
	* This variable stores the config options of Cayuga system.
	*/
	//Options* _options;

	/** 
	* This variable stores the pointer to the measure manager.
	*/
	MeasureMgr* _measureMgr;

	/** 
	* This variable stores the number of events we have processed so far.
	*/
	EventID _numProcessedEvents;

	/** 
	* This variable a pointer to the network output event sender.
	*/
	NetworkSender* _pOutputSender;

	/** 
	* This variable stores the output stream of witnesses.
	*/
	ofstream _outWitnesses;

	/** This variable stores the debug message output stream. */
	ostream& _debugOut;

	/** This variable stores the total number of output events produced by 
		all Cayuga automata, regardless of whether they are resubscription
		NFAs are not. */
	long _numNFAOutputEvents;

	/** This variable stores the total number of output events produced by 
		all Cayuga queries. Note a query can be compiled to multiple NFAs.
		So this number is always no more than _numNFAOutputEvents. */
	long _numQueryOutputEvents;

}; //class QueryProcessorImpl
}  //namespace QueryProcessor
}  //namespace Cayuga

#endif //_QUERY_PROCESSOR_IMPL_H
