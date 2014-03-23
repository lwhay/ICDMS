/**
 * @file Command_AddSchema.h
 * @date April 22, 2007
 * @brief The data structure of a Cayuga add_schema command.

 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _COMMAND_ADD_SCHEMA_H
#define _COMMAND_ADD_SCHEMA_H

#include "../Receiver/CR/Command.h"

#include <iostream>
#include <string>
using namespace std;

namespace Cayuga {
namespace Receiver {
/**
* @brief A Command_AddSchema object is a command for adding a new schema into
		 the system at run time.
* @author Mingsheng Hong (mshong@cs.cornell.edu)
* @date April 24, 2007
* @version 1.0
*/
class Command_AddSchema : public Command {
public:
	/**
	* @brief Default constructor
	* @param streamSchema The stream schema in SIR format.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 22, 2007
	* @version 1.0
	*/
	Command_AddSchema(const string& streamSchema);

	/**
	* @brief Virtual destructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 22, 2007
	* @version 1.0
	*/
	virtual ~Command_AddSchema() {
	}

	virtual const string type() const;

	virtual void exec(QueryProcessorImpl* qpImpl,
		ostream& debugOut = cout) const;

private:
	const string _streamSchema;
}; //class Command_AddSchema
}  //namespace Receiver
}  //namespace Cayuga

#endif //_COMMAND_ADD_SCHEMA_H
