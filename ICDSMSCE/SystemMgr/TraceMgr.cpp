/**
* @file TraceMgr.cpp
* @date April 25, 2006
* @brief This implementation file of class TraceMgr.
* @author Mingsheng Hong (mshong@cs.cornell.edu)
*/

#include <sstream>
using namespace std;

#include "network/common.h"
#include "network/Message.h"
#include "network/TCPServerSocket.h"
#include "network/TCPClientSocket.h"
#include "network/MessageChannel.h"

#include "BasicDataStructure/Automaton/IntermediateNode.h"
using Cayuga::BasicDataStructure::Automaton::Node;
using Cayuga::BasicDataStructure::Automaton::IntermediateNode;

//#include "BasicDataStructure/Event/Event.h"
#include "BasicDataStructure/Automaton/Instance.h"
using Cayuga::BasicDataStructure::EventNS::Event;
using Cayuga::BasicDataStructure::Automaton::Instance;

#include "BasicDataStructure/Automaton/Query.h"
using Cayuga::BasicDataStructure::Automaton::Query;

#include "BasicDataStructure/CMessage.h"
using Cayuga::BasicDataStructure::CMessage;

#include "CayugaSystem.h"
using Cayuga::CayugaSystem;

#include "SAXHandler/ConfigMgr.h"
using namespace Cayuga::SAXHandler;

#include "SystemMgr/TraceMgr.h"
#include "SystemMgr/StreamSchemaMgr.h"
#include "SystemMgr/Options.h"
using namespace Cayuga::SystemMgr;

#include "Utils/AttrMap.h"
using namespace Cayuga::Utils;


TraceMgr::TraceMgr() : 
_servSock(NULL), _clntSock(NULL), _messageChannel(NULL) {
	if (ConfigMgr::getInstance()->getOptions()->_nTracePort != 0) {
		_startTraceServer();
	}

	//open the first tarce file to write trace messages to.
	//it has a special file name trace_0.txt
	string traceFileName = "trace_0.txt";
	const string& dirName 
		= ConfigMgr::getInstance()->getOptions()->_checkPointAndTraceDir;
	traceFileName = dirName + traceFileName;
	if (ConfigMgr::getInstance()->getOptions()->_bRecordTrace) {
		_log.open(traceFileName.c_str());
		if (!_log) {
			cerr<<"error opening "<<traceFileName<<endl;
			exit(-1);
		}
	}
}

TraceMgr::~TraceMgr() {
	delete _messageChannel;
	delete _clntSock; 
	delete _servSock;
}

void TraceMgr::_startTraceServer() {
	CayugaSystem::getInstance()->debugOstream()
		<<"[trace server] waiting for network connection"<<endl;
	_servSock  = new TCPServerSocket(
		ConfigMgr::getInstance()->getOptions()->_nTracePort, 1);
	_clntSock = _servSock->accept();
	_messageChannel = new MessageChannel(_clntSock);
	CayugaSystem::getInstance()->debugOstream()
		<<"[trace server] connection setup"<<endl;
}

void TraceMgr::generateAIRMsg(const Query& nfa) {
	CMessage airMsg("AIR", 
		ConfigMgr::getInstance()->getOptions()->_attrDelimiter);
	
	ostringstream ostr;
	nfa.write(ostr);
	airMsg.appendField(ostr.str());

	_dispatchMsg(airMsg);
}

void TraceMgr::generateEventMsg(const Event* ev) {
	CMessage eventMsg("EVENT", 
		ConfigMgr::getInstance()->getOptions()->_attrDelimiter);
	
	StreamSchemaMgr::getInstance()->getStreamSchema(ev->getStreamID())
		->genEventSchemaContentForTraceMsg(ev, eventMsg);

	eventMsg.appendField(
		StreamSchemaMgr::getInstance()->getStreamName(ev->getStreamID()));

	_addTS(ev, eventMsg);

	_dispatchMsg(eventMsg);
}

void TraceMgr::_addTS(const Event* ev, CMessage& eventMsg) {
	TimeStamp startTS = ev->getStartTime();
	eventMsg.appendField(toString(startTS));
	TimeStamp endTS = ev->getEndTime();
	eventMsg.appendField(toString(endTS));
}

void TraceMgr::generateInstanceMsg(const NodeID& srcNodeID,
								   const EventID& srcInstID,
								   const IntermediateNode* pDestNode,
								   const Instance* pDestInst,
								   const TimeStamp& startTime,
								   const TimeStamp& endTime) {
	CMessage instMsg("INSTANCE", 
		ConfigMgr::getInstance()->getOptions()->_attrDelimiter);
	//EventID srcInstID = pSrcInst ? pSrcInst->getID() : 0; 
	_generateInstanceMsgWithoutDispatching(srcNodeID,
		srcInstID,
		pDestNode,
		pDestInst,
		startTime,
		endTime,
		instMsg);

	_dispatchMsg(instMsg);
}

void TraceMgr::_generateInstanceMsgWithoutDispatching(
	const NodeID srcNodeID,
	const EventID srcInstID,
	const IntermediateNode* pDestNode,
	const Instance* pDestInst,
	const TimeStamp& startTime,
	const TimeStamp& endTime,
	CMessage& instMsgOut) {
	//schema and value info
	if (pDestNode) {
		StreamSchema* pSchema = pDestNode->getNodeSchema();
		assert (pDestInst);
		pSchema->genEventSchemaContentForTraceMsg(pDestInst, instMsgOut);
	}

	NodeID destNodeID = pDestNode ? pDestNode->getID() : BAD_NODE_ID; 
	EventID destInstID = pDestInst ? pDestInst->getID() : BAD_EVENT_ID; 
		
	//node and instance ID
	instMsgOut.appendField(toString(srcNodeID));
	instMsgOut.appendField(toString(srcInstID));
	instMsgOut.appendField(toString(destNodeID));
	instMsgOut.appendField(toString(destInstID));
	
	//timestamp info
	//_addTS(pDestInst, instMsgOut);
	instMsgOut.appendField(toString(startTime));
	instMsgOut.appendField(toString(endTime));
}

CMessage TraceMgr::generateWitnessMsg(const IntermediateNode* pDestNode,
								  const Instance* pWitness,
								  const string& outputStreamName) {
	CMessage witnessMsg("WITNESS", 
		ConfigMgr::getInstance()->getOptions()->_attrDelimiter);
	
	//schema and value info
	StreamSchema* pSchema = pDestNode->getNodeSchema();
	pSchema->genEventSchemaContentForTraceMsg(pWitness, witnessMsg);

	witnessMsg.appendField(outputStreamName);
	
	//timestamp info
	_addTS(pWitness, witnessMsg);
	
	_dispatchMsg(witnessMsg);

	return witnessMsg;
}

void TraceMgr::generateEndOfEpochMsg() {
	CMessage airMsg("EOE", 
		ConfigMgr::getInstance()->getOptions()->_attrDelimiter);
	
	_dispatchMsg(airMsg);
}

void TraceMgr::_dispatchMsg(const CMessage& msg) {
	if (ConfigMgr::getInstance()->getOptions()->_bRecordTrace) {
		_dumpMsg(msg);
	}

	if (ConfigMgr::getInstance()->getOptions()->_nTracePort != 0) {
		_sendMsg(msg);
	}
}

void TraceMgr::_dumpMsg(const CMessage& msg) {
	msg.write(_log);
}

void TraceMgr::_sendMsg(const CMessage& msg) {
	ostringstream ostr;
	msg.write(ostr);
	size_t szMsg = ostr.str().size();

	//the message ID (1 byte) + body
	Message* pMsg = new Message(szMsg);
	char* buffer = pMsg->getData();
	memcpy(buffer, ostr.str().c_str(), szMsg);
	_messageChannel->send(pMsg); 
	delete pMsg;
}

void TraceMgr::checkPoint(const Query& nfa, 
						  const hash_set<NodeID>& nodesWithInstance,
						  const TimeStamp& startingTime) {
	_checkpointNFA(nfa, startingTime);

	_checkpointInstance(nodesWithInstance, startingTime);

	_openNewTraceFile(startingTime);
}

void TraceMgr::_checkpointNFA(const Query& nfa, 
						  const TimeStamp& startingTime) {
	string cpFileName = _getFileName("checkpoint_nfa_", startingTime, true);
	ofstream out(cpFileName.c_str());
	nfa.write(out);
}

void TraceMgr::_checkpointInstance(
	const hash_set<NodeID>& nodesWithInstance,
	const TimeStamp& startingTime) {
	string cpFileName = _getFileName("checkpoint_instance_", 
		startingTime, false);
	ofstream out(cpFileName.c_str());
	for (hash_set<NodeID>::const_iterator node_it 
		= nodesWithInstance.begin(); node_it != nodesWithInstance.end();
		++node_it) {
				
		const IntermediateNode* pNode 
			= dynamic_cast<IntermediateNode*>(Node::_nodes[*node_it]);
		assert (pNode);
		//const unsigned nodeID = pNode->getID();
		assert (pNode->hasInstance());
		for (list<Instance*>::const_iterator inst_it 
			= pNode->_instances.begin(); inst_it != pNode->_instances.end();
			++inst_it) {
			const Instance* pInstance = *inst_it;	
			
			CMessage instMsg("INSTANCE", 
				ConfigMgr::getInstance()->getOptions()->_attrDelimiter);
			_generateInstanceMsgWithoutDispatching(BAD_NODE_ID,
				BAD_EVENT_ID,
				pNode,
				pInstance,
				pInstance->getStartTime(),
				pInstance->getEndTime(),
				instMsg);

			instMsg.write(out);
		}
	}
}

void TraceMgr::_openNewTraceFile(const TimeStamp& startingTime) {
	if (startingTime > 0 
		&& ConfigMgr::getInstance()->getOptions()->_bRecordTrace) {
		string traceFileName = _getFileName("trace_", startingTime, false);
		_log.close();
		_log.open(traceFileName.c_str());
		assert (_log);
	}
}

string TraceMgr::_getFileName(const string& prefix, 
							const TimeStamp& startingTime, 
							bool bIsXML) {
	string strStartingTime = toString(startingTime);
	string fileName = prefix + strStartingTime;
	fileName += bIsXML? ".xml" : ".txt";

	const string& dirName 
		= ConfigMgr::getInstance()->getOptions()->_checkPointAndTraceDir;
	fileName = dirName + fileName;

	return fileName;
}
