/**
 * @file QueryProcessor.cpp
 * @date November 17, 2005
 * @brief Implementation file of QueryProcessor class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "QueryProcessor/QueryProcessor.h"
#include "QueryProcessor/QueryProcessorImpl.h"
using namespace Cayuga::QueryProcessor;
using Cayuga::SystemMgr::Options;


Cayuga::QueryProcessor::QueryProcessor::QueryProcessor(ostream& debugOut) {
	_qpImpl = new QueryProcessorImpl(debugOut);
}

int Cayuga::QueryProcessor::QueryProcessor::addQueries(const vector<string>& queryFileNames,
							   bool bIsAIR) {
	return _qpImpl->addQueries(queryFileNames, bIsAIR);
}

void Cayuga::QueryProcessor::QueryProcessor::startEventProcessing() {
	_qpImpl->startEventProcessing();
}

void Cayuga::QueryProcessor::QueryProcessor::sendOutputEvent(const IntermediateNode* pEndNode, 
									 const Instance* pWitnessInstance) {
	_qpImpl->sendOutputEvent(pEndNode, pWitnessInstance);
}


void Cayuga::QueryProcessor::QueryProcessor::printStats(ostream& out) {
	_qpImpl->printStats(out);
}

//luoluo 3.13
long Cayuga::QueryProcessor::QueryProcessor ::return_out_number(){
	return _qpImpl->return_out_event();
}

long long Cayuga::QueryProcessor::QueryProcessor:: returnProcessTime(){
	return _qpImpl->_process_time.returnTimerInMilliSeconds("process_time");
}

float Cayuga::QueryProcessor::QueryProcessor ::returnTotalProcessEvent(){
	return _qpImpl->_total_events;
}