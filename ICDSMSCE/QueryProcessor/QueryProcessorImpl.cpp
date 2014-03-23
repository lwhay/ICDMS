    /**
     * @file QueryProcessorImpl.cpp
     * @date November 30, 2005
     * @brief Implementation file of QueryProcessorImpl class.
     * @author Mingsheng Hong (mshong@cs.cornell.edu)
     */
    
    #include "AutomatonCompiler/Context.h"
    using namespace Cayuga::AutomatonCompiler;
/*#include "BasicDataStructure/Window/Window.h"*/
	#include "BasicDataStructure/Window/inputWindow.h"
	#include "BasicDataStructure/Window/WindowThread.h"

    
    #include "BasicDataStructure/Automaton/Instance.h"
    #include "BasicDataStructure/Automaton/node.h"
    #include "BasicDataStructure/Automaton/IntermediateNode.h"
    #include "BasicDataStructure/Automaton/InstanceFinder.h"
    #include "BasicDataStructure/Automaton/Query.h"
    using namespace Cayuga::BasicDataStructure::Automaton;
    
    #include "BasicDataStructure/Event/Event.h"
    using namespace Cayuga::BasicDataStructure::EventNS;
    
    #include "BasicDataStructure/CMessage.h"
    using Cayuga::BasicDataStructure::CMessage;
    
    #include "MemoryMgr/CY_GCFace.h"
    using namespace CY_Heap;
    
    #include "PriorityQueue/PriorityQueue.h"
    using namespace Cayuga::PriorityQueue;
    
    #include "QueryProcessor/QueryProcessorImpl.h"
    #include "QueryProcessor/FilterEvaluator.h"
    #include "QueryProcessor/FREvaluator.h"
    using namespace Cayuga::QueryProcessor;
    
    #include "SAXHandler/ConfigMgr.h"
    #include "SAXHandler/AIR_Handler.h"
    using namespace Cayuga::SAXHandler;
    
    #include "SystemMgr/MeasureMgr.h"
    #include "SystemMgr/StreamSchema.h"
    #include "SystemMgr/StreamSchemaMgr.h"
    #include "SystemMgr/TraceMgr.h"
    #include "SystemMgr/Options.h"
    using namespace Cayuga::SystemMgr;
    
    #include "../ICDSMSQL/CayugaQLCompiler.h"
    using namespace Cayuga::QLCompiler;
    
    #include "network/Message.h"
    #include "corelib/os/threads/Thread.h"
    using namespace cudb::corelib::os::threads;
    
    #include "../Receiver/CR/CommandQueue.h"
    #include "../Receiver/CR/Command.h"
    #include "../Receiver/CR/Command_AddQuery.h"
    using namespace Cayuga::Receiver;
    
    #include "../Sender/NetworkSender.h"
    using namespace Cayuga::Sender;

	#include "../Receiver/NetworkReceiverWorker.h"
	#include "../Receiver/ER/NetworkRelER.h"
	using namespace Cayuga::Receiver;
    #include "network/TCPClientSocket.h"

    
    #include <sstream>
	#include <map>
	#include <utility>
    using namespace std;
    
    namespace Cayuga {
    
    ////////////////////////////////////////////////////////////////////////////////
    // Definition of utility functions
    ////////////////////////////////////////////////////////////////////////////////
    
    
    namespace QueryProcessor {
    void insertIndexedPred(const Context& context, 
    					   const PredIndexSpecString* pSpec, 
    					   const vector<NormalAtomPredString*>& indexedPreds) {
    		vector<AttributeID>* pIndexSpec;
    		vector<AttributeID> indexSpec;
    		if (pSpec) { 
    			indexSpec = pSpec->spec2InternalIndexSpec(context);
    			pIndexSpec = &indexSpec;
    		}
    		else {
    			pIndexSpec = NULL;
    		}
    
    		if (context._edgeType == FILTER) {
    			FilterEvaluator::getInstance()->insertPred(context, 
    				pIndexSpec, 
    				indexedPreds);
    		}
    		else {
    			FREvaluator::getInstance()->insertPred(context, 
    				pIndexSpec, 
    				indexedPreds);
    		}
    }
    
    void insertIndexSpecs(const Context& context,
    				 const vector<PredIndexSpecString>& filterPredIndices, 
    				 const vector<PredIndexSpecString>& frPredIndices) {
    for (vector<PredIndexSpecString>::const_iterator it = filterPredIndices.begin();
    		it != filterPredIndices.end();
    		++it) {
    		FilterEvaluator::getInstance()->addIndexSpec(*it, context);
    	}
    
    	//insert FR index spec
    	for (vector<PredIndexSpecString>::const_iterator it = frPredIndices.begin();
    		it != frPredIndices.end();
    		++it) {
    		FREvaluator::getInstance()->addIndexSpec(*it, context);
    	}
    }
    
    void insertDynIndexSpecs(const Context& context,
    						 const PredIndexSpecString&
    						 dynFilterPredIndex) {
    	FilterEvaluator::getInstance()->addDynIndexSpec(dynFilterPredIndex,
    										  context);						
    }
    
    }
    }
    
    
    
////////////////////////////////////////////////////////////////////////////////
// QueryProcessorImpl member function definitions
////////////////////////////////////////////////////////////////////////////////

QueryProcessorImpl::QueryProcessorImpl(ostream& _debugOut) 
: _numProcessedEvents(0), _pOutputSender(NULL) , _debugOut(_debugOut),
_numNFAOutputEvents(0), _numQueryOutputEvents(0) ,_total_events(0){
	_pFilterEvaluator = FilterEvaluator::getInstance();
	_pFilterEvaluator->setQP(this);
	_pFrEvaluator = FREvaluator::getInstance();
	_pFrEvaluator->setQP(this);

	//luoluo 6-17
	pSendInfo = new MDATA;

	CY_Heap::registerRootFinder( new InstanceFinder(_nodesWithInstance) );

	_measureMgr = MeasureMgr::getInstance();

	_nfa_dag = new Query();

	short nEventPort 
		= ConfigMgr::getInstance()->getOptions()->_nOutputEventPort;
	if (nEventPort != 0)
		_pOutputSender = new NetworkSender(nEventPort,
		"Witness Sender", _debugOut,
	ConfigMgr::getInstance()->getOptions()->_OutputEventHost);

	const string& dirName 
		= ConfigMgr::getInstance()->getOptions()->_witnessLogDir;
	const string witnessFileName = dirName + "witnesses.txt";

	if (ConfigMgr::getInstance()->getOptions()->_bLogOutput) {
		//write the query into a file
		_outWitnesses.open(witnessFileName.c_str());
	}
}

QueryProcessorImpl::~QueryProcessorImpl() {
	delete _nfa_dag;

	delete _pOutputSender;
	//need to delete other member variables. However, when the destructor
	//is called, the system goes down, so it does not matter.
}


int QueryProcessorImpl::addQueries(const vector<string>& queryFileNames,
								   bool bIsAIR) {
	int ret = 0;
	if (bIsAIR) {
		AIR_Handler air_handler(*_nfa_dag);
		pluginHandler (&air_handler, 
			queryFileNames, 
			ConfigMgr::getInstance()->getOptions()->_printFrequency,
			_debugOut);
	}
	else {
		CayugaQLCompiler compiler(*_nfa_dag);
		ret = compiler.compile(queryFileNames, _debugOut);
	}

	_debugOut<<"finished loading "
		<<queryFileNames.size()<<" queries."<<endl;
	if (ConfigMgr::getInstance()->getOptions()->_bVerbose) {
		_nfa_dag->write(_debugOut);
	}

	string logFileName = "queries_loaded.xml";
	const string& dirName 
		= ConfigMgr::getInstance()->getOptions()->_commandLogDir;
	logFileName = dirName + logFileName;

	if (!dirName.empty()) {
		ofstream out(logFileName.c_str());
		if (!out) {
			cerr<<"error opening "<<logFileName<<endl;
			exit(-1);
		}
		_nfa_dag->write(out);
	}

	if (ConfigMgr::getInstance()->getOptions()->_bRecordTrace) {
		TraceMgr::getInstance()->generateAIRMsg(*_nfa_dag);
	}

	return ret;
}


void QueryProcessorImpl::startEventProcessing() {
	_debugOut<<"number of automata nodes "
		<<this->_nfa_dag->getSize()<<endl;

	//index start nodes based on input stream ID in filter evaluator
	//FilterEvaluator::getInstance()->indexStartNodes(_nfa_dag->_startNodes);


	Event* ev;
	TimeStamp lastSeenEndTimeStamp = -1;

	//the checkpoint frequency, denoted as W, can be read from in configMgr.
	//call each time period of length W a check point window, starting from
	//window 0. Let the current window ID be k.
	EventID checkpointWindowID = -1;
	long checkPointFrequency 
		= ConfigMgr::getInstance()->getOptions()->_checkPointFrequency;
//luoluo 5-25
/*	inputWindow * pWindowQueue = inputWindow ::getInstance();*/
// 	Cayuga::PriorityQueue::PriorityQueue* pPriorityQueue = 
// 		Cayuga::PriorityQueue::PriorityQueue::getInstance();

 	//luoluo 4-8
	// it must be sure that the information got from the AIR have 
	//been wrote into the window queue . it means we have create
	//a window queue at that time .
/* 	Window * pinWQ =Window ::getInstance();*/
	//because we have just create one window in the input window ,
	// so we can use the begin() function.
/*	Window * pinWQ =*(_nfa_dag->_windows.begin());*/
//	inputWindow * pinWQ = _nfa_dag->_inputwindows;
	inputWindow * pinWQ = inputWindow::getInstance();


	//luoluo 4-18
	while(pinWQ->size()==0){
		if (ConfigMgr::getInstance()->getOptions()->_docInputMode 
			== Cayuga::SAXHandler::NETWORK) {
				Thread::sleep(100);
				continue;
		}
	}

//     //the following two methods are all ok.
// 	NetworkReceiverWorker * pNetworkReceiverWorker 
// 		= NetworkReceiverWorker ::getInstance();
// 	TCPClientSocket * _client
// 				= pNetworkReceiverWorker->socket_exp;
// 
// 	
// 	//set the sending information.
// 	pSendInfo->pred = true;
// 	pSendInfo->swise =pinWQ->getWindowStep();
// 	int len = sizeof(sendBuffer);


	_total_events = pinWQ->size();

	unsigned printFrequency 
		= ConfigMgr::getInstance()->getOptions()->_printFrequency;

	bool one_process = true;
	bool one_slide = true;
	EventID PointID = pinWQ->SlidePoint(pinWQ->_windowevents.begin());
	while(true) {
		ev = pinWQ->peek();
        
		//to get the next begging of the slide operation. 
		if (ev && !one_slide){
			PointID=pinWQ->SlidePoint(pinWQ->_windowevents.begin());
			one_process = true;
		}

		if (pinWQ->Allover)
			break;
		
		

	  //luoluo 4-8
	 //Use this if function to do a sliding processing , before the sliding ,
	 //we should delete those correlated instances exceed the window size.
       if (pinWQ->queue_over())
       {
			if (one_process)
			{
			
 		  //luoluo 5-4
 		 // at the end of the sliding action ,  some new instance should also be process.
 		  _installNewInstances();
					
// 					//luoluo 8-12
// 		   		  //for printing  the information of the _linkEvIns.
// 		   		  for (multimap<EventID , Instance *> ::iterator test_start = _linkEvIns.begin();
// 		   			    test_start != _linkEvIns.end() ; ++ test_start)
// 		   		  {
// 		   			  cout <<"-----------------------------------------"<<endl;
// 		   			  cout <<"the event id :"<<test_start->first <<"the instance id :"
// 		   				  << test_start->second->getID()<<"\t"<<test_start->second->getStartTime()<<"\t"<<
// 		   				  test_start->second->getEndTime()<<endl;
// // 		   			  for (hash_set <EventID> :: iterator for_test = test_start->second->ptr_event.begin();
// // 		   				  for_test != test_start ->second ->ptr_event.end() ; ++for_test)
// // 		   			  {
// // 		   // 				 if ((*for_test) = NULL)cout<<"why this event have been survive"<<endl;
// // 		   // 				else 
// // 		   					cout << "the rational event among the instance is :\t"<<(*for_test)<<endl;
// // 		   			  }
// 		   			  cout <<"------------------------------------------"<<endl;
// 		   		  }


 			multimap<EventID , Instance *> ::iterator end  =
 				_linkEvIns.lower_bound(PointID);
 			//to let the end iterator point the next one of the last being processed eventID
 			//in the multimap .
 			//the following loop also need to be optimized .
			multimap<EventID , Instance *> ::iterator  beg = _linkEvIns.begin();

// 				cout <<"the slide point :   "<<PointID<<endl;
// 				if (_linkEvIns.empty())cout<<"the _linkEvIns is empty!"<<endl;
// 				else
// 				{
// 					cout <<"To erase---the begin event id:  "<<beg->first<<endl;
// 					cout <<"To erase---the end event id:  "<<end->first<<endl;
// 				}

			eraseRelationEvInst(beg , end);
          
/*			cout <<"befor sliding operation:   "<<_linkEvIns.size()<<endl;*/
 			//luoluo 4-11
 			for (int i =_linkEvIns.size();i >0; --i){
 				multimap<EventID , Instance *>:: iterator beg_multi = _linkEvIns.begin();
 				while(beg_multi != _linkEvIns.end()){
 				Instance * beg_ins = beg_multi->second;
 				multimap<EventID , Instance *>:: iterator erase_itera = beg_multi;
 				++ beg_multi;
 				if (beg_ins->iterator_del == true){
 					_linkEvIns.erase(erase_itera);	
 					/*break;*/
 				}
 				}
 			}
/*		cout <<"after sliding operation:   "<<_linkEvIns.size()<<endl;*/
 		ev = pinWQ->peek();
 		_total_events +=pinWQ->size();
/*		one_process =true;*/
		one_slide = false;
		//erase events beyond the window queue.
		multiset<Event * , endTimeLess>::iterator instead = pinWQ->_windowevents.begin();

// 		inputWindow* fortest= pinWQ;
// 		multiset<Event*, endTimeLess>::iterator  fortestWin = fortest->_windowevents.begin();
// 		for (;fortestWin !=fortest->_windowevents.end();++fortestWin) 
// 		{
// 			cout<<"%%%"<<(*fortestWin)->getStartTime()<<"%%%"<<(*fortestWin)->getEndTime()<<endl;
// 		}

		pinWQ->erase_queue(instead,pinWQ->getEraseLine(instead));

// 		inputWindow* fortest1= pinWQ;
// 		multiset<Event*, endTimeLess>::iterator  fortestWin1 = fortest1->_windowevents.begin();
// 		for (;fortestWin1 !=fortest1->_windowevents.end();++fortestWin1) 
// 		{
// 			cout<<"@@@"<<(*fortestWin1)->getStartTime()<<"@@@"<<(*fortestWin1)->getEndTime()<<endl;
// 		}

		{
			//luoluo 8-25
			pinWQ->_indicateLine +=1;
			//luoluo 8-16
			pinWQ->slide_over = true;
		}
			}
			
       }
        //luoluo 4-17 
		//in order to accord with the TCP experiment.
		if (ev == NULL) {
			if (ConfigMgr::getInstance()->getOptions()->_docInputMode 
				== Cayuga::SAXHandler::NETWORK) {
/*				_client->send(pSendInfo , len);*/
//				Sleep(200);
				one_process = false;
				continue;
			}
		}

	   //set the varibles to be processed once.
	   one_process = true;


		//at this point ev cannot be NULL
		assert (ev);
		
		if(ev->getEndTime() > lastSeenEndTimeStamp) {
			//this epoch has ended, do administrative tasks
			//Install all new instances created in this epoch
			_installNewInstances();

			//garbage collection
			if (ConfigMgr::getInstance()->getOptions()->_bMeasure) {								
				_measureMgr->startTimeMeasure(_numProcessedEvents);
				bool bGCInvoked = runGC(false);
				_measureMgr->stopTimeMeasure(GC_TIME, bGCInvoked);
			}
			else {
			bool bGCInvoked = runGC();
			if (bGCInvoked) {
				_debugOut<<"GC invoked."<<endl;
			}
			}

			//If there are pending requests to insert/remove
			//queries in the request queue, process them now.
			_processCommands();

			//the logic below for peeking the queue again is only useful
			//when beta operator is implemented,
			//which may cause InstallNewInstances() to put new events ending at 
			//lastSeenEndTimeStamp onto the queue.
            //In this case it is guaranteed that queue is non-empty. 
			//Since beta has not been implemented, will comment it out for now.
			//ev = pPriorityQueue->peek();
			//assert (ev);

			//if beta is not implemented, this is definitely starting 
			//a new epoch
			if (pinWQ->size()==1)lastSeenEndTimeStamp = 0;
			else{
				lastSeenEndTimeStamp = ev->getEndTime(); }

			if (ConfigMgr::getInstance()->getOptions()->_bRecordTrace) {
				TraceMgr::getInstance()->generateEndOfEpochMsg();
			}

// 			before processing the next event, will see whether to take a 
// 						//checkpoint of the system. 
// 						if (checkPointFrequency != 0) {
// 							//invariant: the current check point window k contains things 
// 							//happening during time [k*W, (k+1)*W)
// 							assert (lastSeenEndTimeStamp 
// 							> checkpointWindowID*checkPointFrequency);
// 							if (lastSeenEndTimeStamp 
// 								>= (checkpointWindowID+1)*checkPointFrequency) {
// 								//the first time lastSeenEndTimeStamp reaches 
// 								//(k+1)*W, start a new check point window k+1
// 								
// 								//simply incrementing checkpointWindowID is incorrect
// 								//++checkpointWindowID;
// 								//instead should advance checkpointWindowID to the largest
// 								//one that is smaller than 
// 								//lastSeenEndTimeStamp/checkPointFrequency
// 								checkpointWindowID 
// 									= lastSeenEndTimeStamp/checkPointFrequency;
// 								TraceMgr::getInstance()->
// 									checkPoint(*_nfa_dag, _nodesWithInstance,
// 									checkpointWindowID*checkPointFrequency);
// 							}
// 						}
		}  

// 		//for test
// 		cout<<"^^^^^^"<<*pinWQ->_special<<endl;

// 		//luoluo 6-20
// 		//the get() function are used to erase the event in the queue.
 		ev = pinWQ->get();

		if (_numProcessedEvents % printFrequency == 0) 
		{
			_debugOut
				<<"processing event "<<_numProcessedEvents<<endl;
		}

		if (ConfigMgr::getInstance()->getOptions()->_bMeasure)
		{
			_measureMgr->startTimeMeasure(_numProcessedEvents);
		}

			if (ConfigMgr::getInstance()->getOptions()->_bMeasure) 
			{
				_measureMgr->startTimeMeasure(_numProcessedEvents);
				_procOneEvent(ev);
				_measureMgr->stopTimeMeasure(EP_TIME, true);

				//send out measures
				if (_numProcessedEvents %
					ConfigMgr::getInstance()->getOptions()->_eventWindowLen
					== 0) {
						_measureMgr->sendMeasure();
				}
			}
			else {
				_procOneEvent(ev);
			}





// 		//luoluo 4-11
// 			EventID eid = ev->getID();
// 			multimap<EventID , Instance*> ::iterator test =_linkEvIns.find(eid);
// 			if (ev->getProcessOrnot()){
// 				if (ConfigMgr::getInstance()->getOptions()->_bVerbose) {
// 					_debugOut
// 						<<"processing event from stream "
// 						<<ev->getStreamID()<<endl;
// 					StreamSchemaMgr::getInstance()->getStreamSchema(ev->getStreamID())
// 						->printEvent(ev, _debugOut);
// /*					_debugOut<<"------******-----"<<"just for testing !"<<endl;*/
// 					_debugOut<<endl;
// 				}
// 			} 
//             else if (test == _linkEvIns.end())_procOneEvent(ev);
// /*			if (test == _linkEvIns.end() || !(ev->getProcessOrnot()))_procOneEvent(ev);*/
// 			else{
// 				if (ConfigMgr::getInstance()->getOptions()->_bVerbose) {
// 					_debugOut
// 						<<"processing event from stream "
// 						<<ev->getStreamID()<<endl;
// 					StreamSchemaMgr::getInstance()->getStreamSchema(ev->getStreamID())
// 						->printEvent(ev, _debugOut);
// /*					_debugOut<<"------******-----"<<"just for testing !"<<endl;*/
// 					_debugOut<<endl;
// 				}
// 			}
// 
// /*			_procOneEvent(ev);*/
// 			_measureMgr->stopTimeMeasure(EP_TIME, true);
// 
// 			//send out measures
// 			if (_numProcessedEvents %
// 				ConfigMgr::getInstance()->getOptions()->_eventWindowLen
// 				== 0) {
// 					_measureMgr->sendMeasure();
// 			}
// 		}
// 		else {
// 			_procOneEvent(ev);
// 		}



		++_numProcessedEvents;
		
		//delete ev;
/*		Event::freeEvent(ev);*/


		// Explicit destructor call
		// ONLY use with placement new
		//ev->Event::~Event();
		//size_t szEvent = sizeof(Event) + ev->getBodySize();
		//CY_Heap::freeMGeneric(
		//	ev, 
		//	ev, 
		//	szEvent
		//	);
	}  

	//Install all new instances created in this epoch
	_installNewInstances();
   
/*	_process_time.stopTimer(time_string);*/

#ifdef _DEBUG
	_pFilterEvaluator->outputStats(_debugOut);
	_pFrEvaluator->outputStats(_debugOut);
#endif //_DEBUG
}

void QueryProcessorImpl::sendOutputEvent(const IntermediateNode* pEndNode, 
										 const Instance* pWitnessInstance
										 ) {
	const Options* options = ConfigMgr::getInstance()->getOptions();
	bool bDebugOrLogOrSend 
		= options->_bVerbose | options->_bLogOutput
		| options->_bRecordTrace | (options->_nOutputEventPort != 0);
	const vector<OutputStreamType>& outputStreams 
	 = pEndNode->_outputStreams;
	for (size_t i = 0; i < outputStreams.size(); ++i) {
		++_numNFAOutputEvents;
		if (outputStreams[i].bOutput) {
			//this is a query level output event.
			++_numQueryOutputEvents;
		}
		
		//now put the event back to queue if this output 
		//stream is resubscribed to by some other query
		if (outputStreams[i].bResubscribed) {
			size_t szSchema = pEndNode->getSchemaLen();
			Event* ev = Event::allocEvent(szSchema);

			ev->setStartTime(pWitnessInstance->getStartTime());
			ev->setEndTime(pWitnessInstance->getEndTime());
			StreamID streamID = outputStreams[i].sid;
			ev->setStreamID(streamID);
			const char* instBody = pWitnessInstance->getBody();
			char* evBody = ev->getBody();
			memcpy(evBody, instBody, szSchema);
			//luoluo 5-25
// 			PriorityQueue::PriorityQueue::getInstance()
// 				->put(ev);
			inputWindow::getInstance()->put(ev);
		}

		if (!bDebugOrLogOrSend) continue;

		//optional Trace writing
		//Will output every witness regardless of whether it corresponds
		//to an output stream for a user query or just an "intermediate"
		//output stream of a cayuga automaton.
		//if options->_bRecordTrace is set, the function call below will
		//write a message to trace file. Otherwise it will just return
		//the constructed CMessage
		const string& outputStreamName = StreamSchemaMgr::getInstance()
			->getStreamName(outputStreams[i].sid);
		CMessage witnessMsg = TraceMgr::getInstance()->generateWitnessMsg(
					pEndNode, pWitnessInstance, outputStreamName);

		//send to a disk file named witnesses.txt
		//pEndNode->_pNodeSchema->printEventInFileERFormat(pNewInstance, 
		//	pEndNode->_outputStreams, _outWitnesses);
		if (options->_bLogOutput) {
			witnessMsg.write(_outWitnesses);
		}

		//send to output screen
		if (options->_bVerbose) {
			_debugOut
				<<"found 1 witness for stream ";
			outputStreams[i].write(_debugOut);
			_debugOut<<"content: "<<endl;
			pEndNode->_pNodeSchema->printEvent(pWitnessInstance, 
				_debugOut);
			_debugOut<<endl;
		}

		//if this output stream is not an output stream for an end user query,
		//but some intermediate stream produced by some automaton, will
		//not output it network
		if (!outputStreams[i].bOutput) continue;

		//network
		if (options->_nOutputEventPort != 0) {
			ostringstream ostr;
			witnessMsg.write(ostr);
			string* pStr = new string(ostr.str());
			_pOutputSender->send(pStr);
		}
	}
	//
}

void QueryProcessorImpl::_procOneEvent(/*const*/ Event* ev) {
//#ifdef _DEBUG
	if (ConfigMgr::getInstance()->getOptions()->_bVerbose) {
		_debugOut
			<<"processing event from stream "
			<<ev->getStreamID()<<endl;
		StreamSchemaMgr::getInstance()->getStreamSchema(ev->getStreamID())
			->printEvent(ev, _debugOut);
		_debugOut<<endl;
	}
	if (ConfigMgr::getInstance()->getOptions()->_bRecordTrace) {
		TraceMgr::getInstance()->generateEventMsg(ev);
	}
//#endif //_DEBUG

	//Filter Evaluator puts affects nodes for this event into _affectedNodes,
	//and for each affected node, it marks affected instances.
	_pFilterEvaluator->eval(ev);

	//for each affected node, FR Evaluator is invoked to do state update for
	//each affected instance, satisfied FR edge pair.
	//Also, nodes touched in this epoch are collected in _nodesTouchedInEpoch,
	//so that instance installment can be performed at the end of each epoch
	for (hash_set<NodeID>::iterator node_it = _affectedNodes.begin();
		node_it != _affectedNodes.end(); ++node_it) {
			Node* pNode = Node::_nodes[*node_it];
			if (pNode->getNodeType() != typeid(StartNode)) {
				_nodesTouchedInEpoch.insert(pNode->getID());
			}

			//cout<<"FR evaluator is  invoked for node  "<<pNode<<endl;
			_pFrEvaluator->eval(ev, pNode);
	}
	_affectedNodes.clear();
}

void QueryProcessorImpl::_installNewInstances() {
	if (ConfigMgr::getInstance()->getOptions()->_bMeasure) {
		//go through all NFA intermdiate states, regardless of whether 
		//they are toched or not during this epoch. 
		//Calculuate the number of instances under
		//each state, by summing up the number of unmarked (for deletion) 
		//instances in pNode->_instances, and the the number of instances in
		//pNode->_newInstances.
		size_t totalNumInstances = 0;
		vector<size_t> numInstancesPerState;
		for (vector<Node*>::const_iterator node_it = 
			_nfa_dag->_intermediateNodes.begin(); 
			node_it != _nfa_dag->_intermediateNodes.end(); ++node_it) {
			IntermediateNode* pNode = (IntermediateNode*)*node_it;
			assert (typeid(*pNode) == typeid(IntermediateNode));
			size_t numInstances = 0;
			for (list<Instance*>::iterator inst_it = pNode->_instances.begin();
			inst_it != pNode->_instances.end(); ++inst_it) {
				if ((*inst_it)->isTouched()) {
				}
				else {
					//only sum up untouched/unmarked instances
					++numInstances;
				}
			}

			//also sum up new instances
			numInstances += pNode->_newInstances.size();
			numInstancesPerState.push_back(numInstances);
			totalNumInstances += numInstances;
		}
		_measureMgr->recordInstanceMeasure(_numProcessedEvents, 
			totalNumInstances, numInstancesPerState);
	}

	//visit all nodes in _nodesTouchedInEpoch
	for (hash_set<NodeID>::iterator node_it 
		= _nodesTouchedInEpoch.begin();
		node_it != _nodesTouchedInEpoch.end(); ++node_it) {
			IntermediateNode* pNode 
				= dynamic_cast<IntermediateNode*>(Node::_nodes[*node_it]);
			assert (pNode);
			for (list<Instance*>::iterator inst_it = pNode->_instances.begin();
				inst_it != pNode->_instances.end(); ) {
					Instance* pThisInstance = *inst_it;
					if (pThisInstance->isTouched()) {
						list<Instance*>::iterator erase_it = inst_it;
						++inst_it;
						//Instance* pThisInstance = *erase_it;
						 
						assert (typeid(*pNode) == typeid(IntermediateNode));
						assert ((pNode)->getSchemaLen() 
							== pThisInstance->getBodySize());

						//luoluo 4-7 4-9
						hash_set</*const*/ EventID>itera;
						hash_set</*const*/ EventID>::iterator ite_s =pThisInstance->ptr_event.begin();
						hash_set</*const*/ EventID>::iterator ite_e =pThisInstance->ptr_event.end();
						while( ite_s !=ite_e){
							itera.insert(* ite_s);
// 														cout<<"***********-----------*************"<<pThisInstance->getID()<<"------"
// 															<<(*ite_s)<<endl;
							++ ite_s;
						}
						for (hash_set</*const*/ EventID>::iterator eve_ite=itera.begin() ;
							eve_ite !=itera.end();eve_ite++ ){
								EventID eveid = (* eve_ite);
								typedef multimap<EventID , Instance *> ::iterator mulmap;
								pair<mulmap , mulmap> pos =
									_linkEvIns.equal_range(eveid);
								while(pos.first != pos.second){
									if (pos.first->second->getID()== pThisInstance->getID()){
/*									cout<<"AAAA for testing :"<<pThisInstance->getID() <<"  delete now!"<<endl;*/
										_linkEvIns.erase(pos.first);
										
										break;
									}
									++ pos.first;						
								}
						}

						Instance::freeInstance(pThisInstance);

                        //luoluo  3.19
						//to look which  instances  will be eraser.
/*						cout<<"***********-----------*************"<<pThisInstance->getID()<<endl;*/

						// Explicit destructor call
						// ONLY use with placement new
						//assert ((pNode)->getSchemaLen() 
						//	== pThisInstance->getBodySize());
						//pThisInstance->Instance::~Instance();
						//assert (typeid(*pNode) == typeid(IntermediateNode));
						//size_t szInstance = sizeof(Instance) +
						//	(pNode)->getSchemaLen();
						//CY_Heap::freeMGeneric(
						//	pThisInstance, 
						//	pThisInstance, 
						//	szInstance
						//	);
						FilterEvaluator::getInstance()->deleteInstance(pNode,
														pThisInstance);				
						pNode->_instances.erase(erase_it);
					}
					else {
						//luoluo 8-5
						list<Instance*>::iterator clear_instacne = inst_it;
						predWindow * ptr = pNode->returnPredWindow();
						if (ptr != NULL)
						{
						long temp = ptr->getPredWinTemporal();
						long cnt	 = ptr->getPredWinCount();
						//luoluo 8-6
						//the problem is that when the instance is transformed one node to another node,it 
						//copy all the ptr_event and add the current event.This should be changed .
						
						TimeStamp start = (*clear_instacne)->getStartTime();
						TimeStamp end   = (*clear_instacne)->getEndTime();
						long  calcu_tempral = (end - start) - (TimeStamp)temp;
						int  nums = (*clear_instacne)->num;
						long  calcu_count	 = nums - cnt;
						if (calcu_tempral > 0 || calcu_count >0)
						{
							assert (typeid(*pNode) == typeid(IntermediateNode));
							assert ((pNode)->getSchemaLen() 
								== pThisInstance->getBodySize());

							hash_set< EventID>itera;
							hash_set< EventID>::iterator ite_s =pThisInstance->ptr_event.begin();
							hash_set< EventID>::iterator ite_e =pThisInstance->ptr_event.end();
							while( ite_s !=ite_e){
								itera.insert(* ite_s);
								++ ite_s;
							}
							for (hash_set< EventID>::iterator eve_ite=itera.begin() ;
								eve_ite !=itera.end();eve_ite++ ){
									EventID eveid = (* eve_ite);
									typedef multimap<EventID , Instance *> ::iterator mulmap;
									pair<mulmap , mulmap> pos =
										_linkEvIns.equal_range(eveid);
									while(pos.first != pos.second){
										if (pos.first->second->getID()== pThisInstance->getID()){
/*											cout<<"BBBB for testing :"<<pThisInstance->getID() <<"  delete now!"<<endl;*/
											_linkEvIns.erase(pos.first);
											break;
										}
										++ pos.first;						
									}
							}

							Instance::freeInstance(pThisInstance);
							FilterEvaluator::getInstance()->deleteInstance(pNode,
								pThisInstance);				
							pNode->_instances.erase(clear_instacne);
						}
						}
						
						++inst_it;
					}
			}

			//if this is end node, clear the new instances since it has been 
			//output; otherwise merge it with existing instances
			if (pNode->getInputStream() == BAD_STREAM_ID) {
				for (list<Instance*>::iterator inst_it 
					= pNode->_newInstances.begin();
					inst_it != pNode->_newInstances.end(); ++inst_it) {
						Instance* pThisInstance = *inst_it;
						//delete pThisInstance;

						assert (typeid(*pNode) == typeid(IntermediateNode));
						assert ((pNode)->getSchemaLen() 
							== pThisInstance->getBodySize());

						//luoluo 4-9
						hash_set</*const*/ EventID>itera;
						hash_set</*const*/ EventID>::iterator ite_s =pThisInstance->ptr_event.begin();
						hash_set</*const*/ EventID>::iterator ite_e =pThisInstance->ptr_event.end();
						while( ite_s !=ite_e){
							itera.insert(* ite_s);
/*														cout<<"-----------*************---------"<<(*ite_s)<<endl;*/
							++ ite_s;
						}
						for (hash_set</*const*/ EventID>::iterator eve_ite=itera.begin() ;
							eve_ite !=itera.end();eve_ite++ ){
								EventID eveid = (* eve_ite);
								typedef multimap<EventID , Instance *> ::iterator mulmap;
								pair<mulmap , mulmap> pos =
									_linkEvIns.equal_range(eveid);
								while(pos.first != pos.second){
									if (pos.first->second->getID() == pThisInstance->getID()){
/*										cout<<"CCCC for testing :"<<pThisInstance->getID() <<"  delete now!"<<endl;*/
										_linkEvIns.erase(pos.first);
										break;
									}
									++ pos.first;						
								}
						}


						Instance::freeInstance(pThisInstance);

//                         //luoluo 3.19
// 					cout<<"-----------*************---------"<<pThisInstance->getID()<<endl;

				}
				pNode->_newInstances.clear();

			}
			else {
				//luoluo 8-7
				//to check the uncomfortable instances in the list.
				predWindow * ptr = pNode->returnPredWindow();
				if(ptr != NULL)
				{
					long temp = ptr->getPredWinTemporal();
				long cnt	 = ptr->getPredWinCount();

			for (list<Instance*>::iterator  loop = pNode->_newInstances.begin();
					loop != pNode->_newInstances.end();)
			{
// 				assert((*loop)->num ==1);
				TimeStamp start = (*loop)->getStartTime();
				TimeStamp end   = (*loop)->getEndTime();
				list<Instance*>::iterator  instan_loop = loop;
				loop ++;
				long  calcu_tempral = (end - start) - (TimeStamp)temp;
				int  nums = (*instan_loop)->num;
				long  calcu_count	 = nums - cnt;

				if (calcu_tempral > 0 || calcu_count >0)
				{
					assert (typeid(*pNode) == typeid(IntermediateNode));
					assert ((pNode)->getSchemaLen() 
						== (*instan_loop)->getBodySize());

					hash_set< EventID>itera;
					hash_set< EventID>::iterator ite_s =(*instan_loop)->ptr_event.begin();
					hash_set< EventID>::iterator ite_e =(*instan_loop)->ptr_event.end();
					while( ite_s !=ite_e){
						itera.insert(* ite_s);
						++ ite_s;
					}
					for (hash_set< EventID>::iterator eve_ite=itera.begin() ;
						eve_ite !=itera.end();eve_ite++ )
					{
							EventID eveid = (* eve_ite);
							typedef multimap<EventID , Instance *> ::iterator mulmap;
							pair<mulmap , mulmap> pos =
								_linkEvIns.equal_range(eveid);
							while(pos.first != pos.second)
							{
								if (pos.first->second->getID()== (*instan_loop)->getID())
								{
/*									cout<<"DDDD  for testing :"<<(*instan_loop)->getID() <<" delete now!"<<endl;*/
									_linkEvIns.erase(pos.first);
									break;
								}
								++ pos.first;						
							}

					}
					Instance::freeInstance((*instan_loop));
					pNode->_newInstances.erase(instan_loop);
				}
			}
			}

				//annoying semantics of merge member function: it requires
				//input to be sorted. Will optimize it later if this code is
				//bottleneck.			
				//pNode->_instances.sort();
				//pNode->_newInstances.sort();
				//pNode->_instances.merge(pNode->_newInstances);
				
				//the new logic for merging instances below is implemented
				//by Varun
				FilterEvaluator::getInstance()->
						indexInstance(pNode, pNode->_newInstances);

				copy(pNode->_newInstances.begin(), pNode->_newInstances.end(),
					back_inserter(pNode->_instances));
				//for(list<Instance*>::const_iterator it = 
				//	pNode->_newInstances.begin();
				//	it != pNode->_newInstances.end(); ++it)
				//	pNode->_instances.push_back(*it);
				pNode->_newInstances.clear();

				if (pNode->hasInstance()) {
					_nodesWithInstance.insert(pNode->getID());
				}
				else {
					_nodesWithInstance.erase(pNode->getID());
				}
			}
	}


	//for (vector<Node*>::iterator node_it 
	//	= (_nfa_dag->_intermediateNodes).begin();
	//	node_it != (_nfa_dag->_intermediateNodes).end(); ++node_it) {
	//	IntermediateNode* pNode = (IntermediateNode*) *node_it;
	//	pNode->updateCounter();
	//}
	_nodesTouchedInEpoch.clear();
}

void QueryProcessorImpl::_processCommands() {
	CommandQueue* pCommandQueue = CommandQueue::getInstance();
	//although pCommandQueue is guarded by mutex, the following code is safe
	//since there is only one reader in the system -- this engine thread.
	//Therefore whenever the queue is non-empty, it will stay non-empty 
	//continue the engine removes entries from it
	while (!pCommandQueue->empty()) {
		const Command* pCommand = pCommandQueue->get();
		pCommand->exec(this, _debugOut);
	}
}


void QueryProcessorImpl::printStats(ostream& out = cout) {
	out<<"total number of NFA level output events: "
		<<_numNFAOutputEvents<<endl;
	out<<"total number of query level output events: "
		<<_numQueryOutputEvents<<endl;
}

//luoluo 3.13
long QueryProcessorImpl ::return_out_event(){
	return _numQueryOutputEvents;
}


void QueryProcessorImpl::eraseRelationEvInst(
			   multimap<EventID , Instance *> ::iterator &first,
			   multimap<EventID , Instance *> ::iterator &last){
	for (;first!= last ;  ){
	Instance * ptrInstance = first->second ;
	if (ptrInstance->iterator_del==true){++first;continue;}
	else {
		for (vector<Node*>::const_iterator nodes = 
			_nfa_dag->_intermediateNodes.begin(); 
			nodes != _nfa_dag->_intermediateNodes.end(); ++nodes) {
				IntermediateNode* pNode = (IntermediateNode*)*nodes;
				assert (typeid(*pNode) == typeid(IntermediateNode));
				for (list<Instance*>::iterator inst_it = pNode->_instances.begin();
					inst_it != pNode->_instances.end();) {
						if (ptrInstance->getID()==(*inst_it)->getID())
						{
							list<Instance*>::iterator delete_inst = inst_it;
							++inst_it;
							//luoluo 4-11
							Instance::freeInstance(ptrInstance);
							ptrInstance->iterator_del=true;

// 							 											cout<<"---***---"<<"For testing 222"<<"---***---"<<endl;
// 																		cout<<"***"<<ptrInstance->getID()<<"***"<<ptrInstance->getStartTime()
// 							 											<<"***"<<ptrInstance->getEndTime()<<endl;


							FilterEvaluator :: getInstance()->
								deleteInstance(pNode , ptrInstance);
							pNode->_instances.erase(delete_inst);
							
							++first;
							break;
						}
						else{
							++inst_it;
						}
				}
// 				//if this is end node, then do nothing ,if is not ,some action 
// 				//shoule be taken.
// 				if (pNode->getInputStream() != BAD_STREAM_ID) {
// 					for (list<Instance*>::iterator inst_it 
// 						= pNode->_newInstances.begin();
// 						inst_it != pNode->_newInstances.end(); ) {
// 							if (ptrInstance->getID()==(*inst_it)->getID()){
// 								list<Instance*>::iterator delete_inst = inst_it;
// 								++inst_it;
// 								//luoluo 4-11
// 								Instance::freeInstance(ptrInstance);
// 								ptrInstance->iterator_del=true;
// 
// 								//luoluo 8-12
// 								cout<<"---***---"<<"For testing 333"<<"---***---"<<endl;
// 								cout<<"***"<<ptrInstance->getID()<<"***"<<ptrInstance->getStartTime()
// 									<<"***"<<ptrInstance->getEndTime()<<endl;
// 
// 
// 
// 								++first;
// 								pNode->_newInstances.erase(delete_inst);
// 								break;
// 							}
// 							else{
// 								++inst_it;
// 							}
// 					}}
		}
		if (ptrInstance->iterator_del ==false) ++first;
	}
}
}

