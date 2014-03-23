/**
 * @file Command_AddQuery.h
 * @date March 23, 2007
 * @brief The data structure of a Cayuga add_query command.

 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _COMMAND_ADD_QUERY_H
#define _COMMAND_ADD_QUERY_H

#include "../Receiver/CR/Command.h"

#include <string>
using namespace std;

namespace Cayuga {
namespace Receiver {
/**
* @brief A Command_AddQuery object is a command for adding a new query into
		 the system at run time.
* @author Mingsheng Hong (mshong@cs.cornell.edu)
* @date April 24, 2007
* @version 1.0
*/
class Command_AddQuery : public Command {
public:
	/**
	* @brief Default constructor
	* @param bAIR True if the input query is in AIR format. False if it is in
				CEL format.
	* @param query The ASCII encoding of the input query.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date March 23, 2007
	* @version 1.0
	*/
	Command_AddQuery(bool bAIR, const string& query);

	/**
	* @brief Virtual destructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date March 23, 2007
	* @version 1.0
	*/
	virtual ~Command_AddQuery() {
	}

	virtual const string type() const;

	virtual void exec(QueryProcessorImpl* qpImpl,
		ostream& debugOut = cout) const;

private:
	bool _bAIR;

	const string _strQuery;

}; //class Command_AddQuery
}  //namespace Receiver
}  //namespace Cayuga

#endif //_COMMAND_ADD_QUERY_H
