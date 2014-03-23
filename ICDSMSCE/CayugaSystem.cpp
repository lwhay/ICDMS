/**
* @file CayugaSystem.cpp
* @date April 4, 2007
* @brief This implementation file of class CayugaSystem.
* @author Mingsheng Hong (mshong@cs.cornell.edu)
*/

#include "CayugaSystem.h"
using namespace Cayuga;

#include "BasicDataStructure/Automaton/Query.h"
using namespace Cayuga::BasicDataStructure::Automaton;

#include "QueryProcessor/QueryProcessor.h"
using Cayuga::QueryProcessor::QueryProcessor;

#include "MemoryMgr/CY_HeapFace.h"
using namespace CY_Heap;

#include "SAXHandler/CAXParser.h"
#include "SAXHandler/ConfigMgr.h"
#include "SAXHandler/SAXEventType.h"
#include "SAXHandler/SIR_Handler.h"
using namespace Cayuga::SAXHandler;

#include "SystemMgr/DataTypeMgr.h"
#include "SystemMgr/Options.h"
#include "SystemMgr/StreamSchemaMgr.h"
#include "SystemMgr/TraceMgr.h"
using namespace Cayuga::SystemMgr;

#include "../Receiver/NetworkReceiverServerThread.h"
#include "../Receiver/CR/NetworkCR.h"

#include "../Receiver/ER/FileRelER.h"
#include "../Receiver/ER/NetworkRelER.h"
#include "../Receiver/Options/FileReceiverOptions.h"
using namespace Cayuga::Receiver;

#include "Utils/split.h"
#include "Utils/Timer.h"
#include "Utils/AttrMap.h"
using namespace Cayuga::Utils;

#include <iostream>
#include <cassert>
using namespace std;

CayugaSystem::CayugaSystem() : _pCayugaEngine(NULL), 
_bDebugOutputToFile(false) {
}

void CayugaSystem::init(const string& configFileName) {
	_initXMLSystem();

	//insert primitive data types into system catalog, including int, 
	//float, string
	// lwh
	// How to define the primitive types
	_insertPrimitiveTypes();

	//read config options
	_loadConfig(configFileName);

	//make modify debugOstream() according to config option 
	//DebugMessageDestination
	//@note: if debugOstream() is used before this point, the output destination
	//may be different from what is specified in the config file.
	const string& debugDest = 
		ConfigMgr::getInstance()->getOptions()->
		_debugMessageDestination;
	if (!debugDest.empty()) {
			_bDebugOutputToFile = true;

			//vector<string> fields;
			//split(ConfigMgr::getInstance()->getOptions()->
			//	_debugMessageDestination, fields, '/');

			_debugOutputStream.open(debugDest.c_str());
			if (!_debugOutputStream) {
				cerr<<"cannot open file "<<debugDest<<endl;
			}
	}
	debugOstream()<<"finished reading configuration file."<<endl;
	
	//read stream schemas
	_loadSchemas();

	_createGCHeap();

	//int ret = loadQueries(existingQs);
	// lwh
	// Enter the processing procedure
	_pCayugaEngine = new Cayuga::QueryProcessor::QueryProcessor(debugOstream());
	_loadQueries();

	_loadCommands();

	_loadEvents();

	//return ret;
}

CayugaSystem::~CayugaSystem() {
		//clean up
	_terminateXMLSystem();

	delete _pCayugaEngine;

	if (_bDebugOutputToFile) {
		_debugOutputStream.close();
	}
}

ostream& CayugaSystem::debugOstream() {
	if (!_bDebugOutputToFile) {
		return cout;
	}
	else {
		return _debugOutputStream;
	}
}

void CayugaSystem::startQP() {
	debugOstream()<<"start event processing."<<endl;
	Timer timer;
	const string totalTimeCost = "total_cost";
	timer.addAndStartTimer(totalTimeCost);
	_pCayugaEngine->startEventProcessing();
	timer.stopTimer(totalTimeCost);
	timer.printTimer(totalTimeCost, debugOstream());

	_pCayugaEngine->printStats();

	//Print Average Occupancy of Intermediate/End nodes
	//existingQs.printOccupancy();

}

void CayugaSystem::_loadConfig(const string& configFileName) {
	pluginHandler(ConfigMgr::getInstance(), 
		vector<string>(1, configFileName),
		1000, //print frequency. Cannot get from
		//ConfigMgr::getInstance()->getOptions()->_printFrequency yet
		//since the config options are to be loaded from this function call.
		debugOstream());
}

void CayugaSystem::_loadSchemas() {
	debugOstream()
		<<"reading input stream schema(s)."<<endl;
	SIR_Handler mySIRHandler;
	const vector<string>& streamFileNames =
		ConfigMgr::getInstance()->getOptions()->_streamSchemaFiles;
	pluginHandler (&mySIRHandler, 
		streamFileNames, 
		ConfigMgr::getInstance()->getOptions()->_printFrequency,
		debugOstream());

	StreamSchema dualSchema;
	StreamSchemaMgr::getInstance()->insertNewSchema("DUAL", dualSchema);
}

int CayugaSystem::_loadQueries() {
	int ret = 0;
	debugOstream()<<"loading queries."<<endl;
	const Options* pOptions = ConfigMgr::getInstance()->getOptions();
	unsigned numQueries = pOptions->_queryNumber;
	vector<string> queryFileNames;
	if (pOptions->_queryInputMode == Cayuga::SAXHandler::DIR) {
		//now we load a bunch of queries randomly generated
		for (unsigned i = 0; i < numQueries; i++) {
			//scripts/output_queries/
			string dirName = pOptions->_queryInputName;
			//if (dirName[dirName.size()-1] != '/') {
			//	dirName += "/";
			//}
			Options::normalizeDirectory(dirName);

			string fn = dirName + "AIR_";
			fn += toString(i);
			//char num[100];
			//snprintf(num, 100, "%d", i);
			//fn += num;
			fn += ".xml";
			queryFileNames.push_back(fn);
		}
	}
	else {
		split(pOptions->_queryInputName, queryFileNames, ';');

		assert (numQueries == queryFileNames.size());
	}

	_pCayugaEngine->addQueries(queryFileNames, pOptions->_bAirQueries);
	
	return ret;
}

void CayugaSystem::_loadEvents() {
	debugOstream()<<"loading events."<<endl;
	//document comes from network
	if (ConfigMgr::getInstance()->getOptions()->_docInputMode 
		== Cayuga::SAXHandler::NETWORK) {
			_initER();
	}
	else {
		//We put CAX parser in the same address space as Cayuga
		//engine, and therefore the parser will internalize events
		//generated from XML documents and put them in the heap.
		unsigned numDocs = ConfigMgr::getInstance()->getOptions()->_docNumber;
		vector<string> docFileNames;
		if (ConfigMgr::getInstance()->getOptions()->_docInputMode 
			== Cayuga::SAXHandler::DIR) {
				for (unsigned i = 0; i < numDocs; i++) {
					string dirName = 
						ConfigMgr::getInstance()->getOptions()->_docInputName;
					if (dirName[dirName.size()-1] != '/') {
						dirName += "/";
					}
					string fn = dirName + "doc_";
					char num[100];
					snprintf(num, 100, "%d", i);
					fn += num;
					fn += ".xml";
					docFileNames.push_back(fn);
				}
		}
		else {
			assert (ConfigMgr::getInstance()->getOptions()->_docInputMode 
				== Cayuga::SAXHandler::FILE);
			assert (numDocs == 1);
			docFileNames.push_back(
				ConfigMgr::getInstance()->getOptions()->_docInputName);
		}

		if (ConfigMgr::getInstance()->getOptions()->_bXMLStream) {
			CaxParser caxParser;
			pluginHandler (&caxParser, 
				docFileNames, 
				ConfigMgr::getInstance()->getOptions()->_printFrequency,
				debugOstream());
		}
		else {
			Timer timer;
			const string eventLoadingCost = "event_loading";
			timer.addAndStartTimer(eventLoadingCost);

			//parse relational streams
			//inputStreamName is associated with each event
			string* concatedDocFileName = new string();
			for (size_t doc_idx = 0; doc_idx < docFileNames.size(); ++doc_idx) {
				*concatedDocFileName += docFileNames[doc_idx];
				*concatedDocFileName += ";";
			}
			char charDelim 
				= ConfigMgr::getInstance()->getOptions()->_attrDelimiter; 
			bool bStrict = ConfigMgr::getInstance()->getOptions()->_bStrict;
			FileReceiverOptions fro(*concatedDocFileName, charDelim, bStrict);
			FileRelER relParser;
				relParser.open(&fro);
			delete concatedDocFileName;

			timer.stopTimer(eventLoadingCost);
			timer.printTimer(eventLoadingCost, debugOstream());

		}
	}
	debugOstream()
		<<"finished loading documents."<<endl;
}

void CayugaSystem::_initER() {
	if (ConfigMgr::getInstance()->getOptions()->_nERPort != 0) {
		NetworkReceiverServerThread<NetworkRelER>* pThread 
			= new NetworkReceiverServerThread<NetworkRelER>(
			ConfigMgr::getInstance()->getOptions()->_nERPort, debugOstream());

		pThread->start();
	}
}


void CayugaSystem::_createGCHeap() {
	size_t multiFactor = 1;
	if (ConfigMgr::getInstance()->getOptions()->_gcSizeUnit == "KB") {
		multiFactor *= 1024;
	}
	else if (ConfigMgr::getInstance()->getOptions()->_gcSizeUnit == "MB") {
		multiFactor *= 1024 * 1024;
	}
	else if (ConfigMgr::getInstance()->getOptions()->_gcSizeUnit == "BYTE") {
		//do nothing
	}
	else {
		cerr<<"the GC size unit parameter does not have a correct value";
		exit(-1);
	}
	CY_Heap::createHeap(
		ConfigMgr::getInstance()->getOptions()->_gcSize*multiFactor);
}

//save into disk file the Automaton structure, remaining predicates, 
//content in FilterEvaluator and FREvaluator
//void checkpoint(const Query& existingQs) {
//	ofstream outNFA("queries.txt");
//	existingQs.write(outNFA);
//
//	ofstream outEvaluator("queries.txt");
//}
//
//void restore(Query& queries) {
//}

void CayugaSystem::_initXMLSystem() {
	//debugOstream()<<"initializing system."<<endl;

	try {
		XMLPlatformUtils::Initialize();
	}
	catch (const XMLException& toCatch) {
		XERCES_STD_QUALIFIER cerr << "Error during initialization! :\n"
			<< toCatch.getMessage() << XERCES_STD_QUALIFIER endl;
		exit (-1);
	}
}

void CayugaSystem::_terminateXMLSystem() {
	XMLPlatformUtils::Terminate();
}

void CayugaSystem::_loadCommands() {
	if (ConfigMgr::getInstance()->getOptions()->_nCommandPort != 0) {
		NetworkReceiverServerThread<NetworkCR>* pThread 
			= new NetworkReceiverServerThread<NetworkCR>(
			ConfigMgr::getInstance()->getOptions()->_nCommandPort, 
			debugOstream());
		pThread->start();

		//for debugging purpose: pausing the main thread
		//while(true) 
		//	Thread::sleep(1000000);
	}
}


void CayugaSystem::_insertPrimitiveTypes() {
	DataTypeMgr* pDataTypeMgr = DataTypeMgr::getInstance();
	pDataTypeMgr->insertType("int", sizeof(int));
	pDataTypeMgr->insertType("float", sizeof(float));
	pDataTypeMgr->insertType("shortstring", 4);
	// lwh
	// 1. What is the means of the SAXEventType
	// 2. Please give the categories of the predict definitions
	pDataTypeMgr->insertType("SAXEventType", sizeof(SAXEventType));

	//a variable length string type
	pDataTypeMgr->insertType("string", VARIABLE_LENGTH, true);
}
