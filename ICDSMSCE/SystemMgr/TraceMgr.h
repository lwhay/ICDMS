/**
 * @file TraceMgr.h
 * @date April 25, 2006
 * @brief This file defines a trace manager for producing traces of internal 
		  system state during event processing.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _TRACE_MGR_H
#define _TRACE_MGR_H

#include <fstream>
using namespace std;

#include "BasicDataStructure/Automaton/NodeID.h"
using Cayuga::BasicDataStructure::Automaton::NodeID;

#include "BasicDataStructure/Event/EventID.h"
using Cayuga::BasicDataStructure::EventNS::EventID;

#include "BasicDataStructure/Timestamp.h"
using Cayuga::BasicDataStructure::TimeStamp;

#include "Utils/Singleton.h"

namespace cudb {
namespace network {
	class Message;
	class TCPServerSocket;
	class TCPClientSocket;
	class MessageChannel;
}
}

using namespace cudb::network;

namespace Cayuga {

namespace BasicDataStructure {
namespace Automaton {
class Node;
class IntermediateNode;
class Instance;
class Query;
}

class CMessage;


namespace EventNS {
class Event;
}
} //namespace BasicDataStructure

using Cayuga::BasicDataStructure::Automaton::Node;
using Cayuga::BasicDataStructure::Automaton::IntermediateNode;
using Cayuga::BasicDataStructure::Automaton::Instance;
using Cayuga::BasicDataStructure::Automaton::Query;
using Cayuga::BasicDataStructure::EventNS::Event;
using Cayuga::BasicDataStructure::CMessage;

namespace SystemMgr {
using Cayuga::Utils::Singleton;

class TraceMgr;
/**
 * @brief A TraceMgr is responsible for producing traces internal system state.
		  The trace messages are portable.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date April 25, 2006
 * @version 1.0
 */
class TraceMgr : public Singleton<TraceMgr>
{
	friend class Cayuga::Utils::Singleton<TraceMgr>;
public:
	/**
	* @brief Constructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 25, 2006
	* @version 1.0
	*/
	TraceMgr();

	/**
	* @brief Destructor.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 25, 2006
	* @version 1.0
	*/
	~TraceMgr();

	/**
	* @brief Generate an AIR message from the input automata, 
			  and store it in a trace file.
	* @param nfa The automata structure from which to generate the AIR message.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 3, 2007
	* @version 1.0
	*/
	void generateAIRMsg(const Query& nfa);

	/**
	* @brief Generate an EVENT message from the input event,
			 and store it in a trace file.
	* @param ev The input event from which to generate the EVENT message.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 3, 2007
	* @version 1.0
	*/
	void generateEventMsg(const Event* ev);

	/**
	* @brief Generate an INSTANCE message from the input instance,
			 and store it in a trace file.
	* @param srcNodeID The source node under which the source instance is stored
	* @param srcInstID The source instance that caused the new instance to be
			 created.
	* @param pDestNode The destination node under which the new instance is 
			 stored.
	* @param pDestInst The newly created instance
	* @param startTime Start time of the new instance.
	* @param endTime End time of the new instance.
	* @note If pDestNode and pDestInst are NULL, this is a message indicating
			that pSrcInst has been expired by a duration predicate on a filter
			edge, and therefore can be marked for deletion.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 3, 2007
	* @version 1.0
	*/
	void generateInstanceMsg(const NodeID& srcNodeID,
		const EventID& srcInstID,
		const IntermediateNode* pDestNode,
		const Instance* pDestInst,
		const TimeStamp& startTime,
		const TimeStamp& endTime
		);

	/**
	* @brief Generate a WITNESS message from the input instance,
			 and store it in a trace file.
	* @param pDestNode The destination node under which the new instance is 
			 stored.
	* @param pWitness The newly created instance/witness to store
	* @param outputStreamName The output stream name ofthe witness event.
	* @return The generated CMessage of this witness.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 3, 2007
	* @version 1.0
	*/
	CMessage generateWitnessMsg(const IntermediateNode* pDestNode,
		const Instance* pWitness,
		const string& outputStreamName);

	/**
	* @brief Generate an end-of-epoch message, and store it in a trace file.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 3, 2007
	* @version 1.0
	*/
	void generateEndOfEpochMsg();

	/**
	* @brief Check point the NFA and instance information.
			Store the checkpoint NFA data into checkpoint_nfa_n.xml, 
			where n takes the value of startingTime. 
			
			Similarly, store the checkpoint instance data into 
			checkpoint_instance_n.xml, where n is described above.

			Also, open a new trace file named
			trace_n.txt to write trace messages to, if traces need to be
			recorded according to the config option.
	* @param nfa The input NFA to check point.
	* @param nodesWithInstance The input NFA nodes with instances to 
			 check point.
	* @param startingTime The as_of time of this check point.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 9, 2007
	* @version 1.0
	*/
	void checkPoint(const Query& nfa, 
		const hash_set<NodeID>& nodesWithInstance,
		const TimeStamp& startingTime);

private:
	void _startTraceServer();
	

	//generate a CMessage for the input instance and store it in instMsgOut
	void _generateInstanceMsgWithoutDispatching(
		const NodeID srcNodeID,
		const EventID srcInstID,
		const IntermediateNode* pDestNode,
		const Instance* pDestInst,
		const TimeStamp& startTime,
		const TimeStamp& endTime,
		CMessage& instMsgOut);

	void _checkpointNFA(const Query& nfa, 
		const TimeStamp& startingTime);

	void _checkpointInstance(
		const hash_set<NodeID>& nodesWithInstance,
		const TimeStamp& startingTime);

	/**
	* @brief Compute and return a file name as follows. It consists of 
			 the directory name from parameter value
			 ConfigMgr::getInstance()->getOptions()->_checkPointAndTraceDir,
			 and then the input prefix of file names chosen from 
			 {checkpoint_nfa_, checkpoint_instance_, trace_}, 
			 followed by n taking the value 
			 of startingTime, follow by the file extension .txt or .xml 
			 controlled by bIsXML
	* @param prefix The file name prefix
	* @param startingTime This piece of info will be incorporated into the
			file name that is generated.
	* @param bIsXML True if the file name extension is .xml. False if the
			extension is .txt.
	* @return The generated file name
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 9, 2007
	* @version 1.0
	*/
	string _getFileName(const string& prefix, 
		const TimeStamp& startingTime, 
		bool bIsXML);

	/**
	* @brief Open a new file to write trace messages to, if startingTime > 0.
			If startingTime = 0, the file has already been opened in the 
			constructor, so will not open it again.
	* @param startingTime This piece of info will be incorporated into the
			file name that is generated.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 9, 2007
	* @version 1.0
	*/
	
	void _openNewTraceFile(const TimeStamp& startingTime);


	//add timestamp info of the input ev into eventMsg
	void _addTS(const Event* ev, CMessage& eventMsg);

	void _dispatchMsg(const CMessage& msg);

	void _dumpMsg(const CMessage& msg);

	void _sendMsg(const CMessage& msg);

	/**
	* This variable stores a handle to the output stream for log traces.
	*/
	ofstream _log;

	//for trace server
	TCPServerSocket* _servSock;
	TCPClientSocket* _clntSock;
	MessageChannel* _messageChannel;

}; //class TraceMgr
}  //namespace SystemMgr
}  //namespace Cayuga

#endif //_TRACE_MGR_H
