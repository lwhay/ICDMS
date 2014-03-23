/**
 * @file QueryProcessor.h
 * @date November 17, 2005
 * @brief Query processor is the driver of the whole engine. We Applied proxy 
          pattern here to decouple its interface from its implementation by 
		  introducing another class QueryProcessorImpl.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _QUERY_PROCESSOR_H
#define _QUERY_PROCESSOR_H

#include <iostream>
#include <string>
#include <vector>
#include <hash_set>
using namespace std;

namespace Cayuga {
namespace BasicDataStructure {
namespace Automaton {	
class Query;
class IntermediateNode;
class Instance;
}
}

namespace SystemMgr {
class Options;
}

using Cayuga::SystemMgr::Options;

namespace QueryProcessor {
class QueryProcessorImpl;
using namespace Cayuga::BasicDataStructure::Automaton;

/**
 * @brief A QueryProcessor object drives components such as FilterEvaluator
          and FREvaluator to process incoming events.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 17, 2005
 * @version 1.0
 */
class QueryProcessor {
public:	
	/**
	* @brief Default constructor
	* @param debugOut The debug message output stream
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 17, 2005
	* @version 1.0
	*/
	QueryProcessor(ostream& debugOut = cout);
	
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
	void printStats(ostream& out = cout);

	//luoluo3.13
	long return_out_number();

	long long returnProcessTime();
	float returnTotalProcessEvent();

private:
	/**
	* This variable stores the actual implementation object of this query 
	* processor.
    */
	QueryProcessorImpl* _qpImpl;
}; //class QueryProcessor
}  //namespace QueryProcessor
}  //namespace Cayuga

#endif //_QUERY_PROCESSOR_H
