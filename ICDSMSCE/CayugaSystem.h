/**
 * @file CayugaSystem.h
 * @date April 4, 2007
 * @brief This file defines CayugaSystem class, 
		 which represents the entire Cayuga system. This is so that the main 
		 file can simply get the singleton instance of this class can start
		 it.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _CAYUGA_SYSTEM_H
#define _CAYUGA_SYSTEM_H

#include "Utils/Singleton.h"
using Cayuga::Utils::Singleton;


#include <string>
#include <fstream>
#include <vector>
using namespace std;

namespace Cayuga {
namespace QueryProcessor {
class QueryProcessor;
}

/**
 * @brief A CayugaSystem object represents the entire Cayuga system. 
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date April 4, 2007
 * @version 1.0
 */
class CayugaSystem;
class CayugaSystem : public Singleton<CayugaSystem> {
	friend class Cayuga::Utils::Singleton<CayugaSystem>;
public:
	/**
	* @brief Default constructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 4, 2007
	* @version 1.0
	*/
	CayugaSystem();
	
	/**
	* @brief Virtual destructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 4, 2007
	* @version 1.0
	*/
	virtual ~CayugaSystem();

	/**
	* @brief Initialize the system. Load configuration, schemas, queries and
			events. Also create Cayuga heap.
	* @param configFileName The input configuration file name
	* @Invariant This function has to be called before startQP()
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 4, 2007
	* @version 1.0
	*/
	void init(const string& configFileName);
	
	/**
	* @brief Start query processing
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 4, 2007
	* @version 1.0
	*/
	void startQP();

	/**
	* @brief return an output stream handle for debug messages
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 30, 2007
	* @version 1.0
	*/
	ostream& debugOstream();

private:
	
	void _initXMLSystem();
	
	void _insertPrimitiveTypes();
	
	void _loadConfig(const string& configFileName);
	
	void _loadSchemas();
	
	void _createGCHeap();
	
	int  _loadQueries();
	
	int _addQueries(const vector<string>& queryFileNames);

	void _loadCommands();
	
	void _loadEvents();
	void _initER();

	void _terminateXMLSystem();

	/** This variable stores the set of queries in the system. */
	//Query* _pQueries;

	/** This variable stores a pointer to the query engine. */
	Cayuga::QueryProcessor::QueryProcessor* _pCayugaEngine;

	/** Used only if Options::_debugMessageDestination is not NULL. */
	ofstream _debugOutputStream;

	/** Used only if Options::_debugMessageDestination is not NULL. */
	bool _bDebugOutputToFile;

}; //class CayugaSystem
}  //namespace Cayuga

#endif //_CAYUGA_SYSTEM_H
