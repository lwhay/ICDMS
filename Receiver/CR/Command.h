/**
 * @file Command.h
 * @date March 23, 2007
 * @brief The data structure of a Cayuga command.

 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _COMMAND_H
#define _COMMAND_H

#include <iostream>	
#include <string>
using namespace std;

namespace Cayuga {	
namespace QueryProcessor {
class QueryProcessorImpl;
}
using Cayuga::QueryProcessor::QueryProcessorImpl;

namespace Receiver {
/**
* @brief Command class is the abstract base class for Cayuga commands.
* @author Mingsheng Hong (mshong@cs.cornell.edu)
* @date April 24, 2007
* @version 1.0
*/
class Command {
public:
	/**
	* @brief Default constructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date March 23, 2007
	* @version 1.0
	*/
	Command() {
	}

	/**
	* @brief Virtual destructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date March 23, 2007
	* @version 1.0
	*/
	virtual ~Command() {
	}

	/**
	* @brief Execute the command
	* @param qpImpl Pointer to the query processor
	* @param debugOut The debug message output stream
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 22, 2007
	* @version 1.0
	*/
	virtual void exec(QueryProcessorImpl* qpImpl,
		ostream& debugOut = cout) const = 0;

	/**
	* @brief Type of this command
	* @return Type of this command
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 22, 2007
	* @version 1.0
	*/
	virtual const string type() const = 0;

	/**
	* @brief Log the current command into a file
	* @param filename_extension Extension fo the file name
	* @param cmd The current command
	* @return Name of the logged file
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 22, 2007
	* @version 1.0
	*/
	virtual const string logCommand(
		const string& filename_extension,
		const string& cmd) const;

private:
	/** 
	* This variable stores the number of commands processed at run-time.
	*/
	static long _numCommands;

}; //class Command

/**
* @brief Create a command from the serialized format in buffer
* @param buffer The character buffer storing the serialized command
* @invariant buffer must be NULL-terminated.
* @param delim Character delimiter
* @author Mingsheng Hong (mshong@cs.cornell.edu)
* @date March 23, 2007
* @version 1.0
*/
Command* makeCommand(const char* buffer, char delim);

}  //namespace Receiver
}  //namespace Cayuga

#endif //_COMMAND_H
