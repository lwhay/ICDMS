/**
 * @file CMessage.h
 * @date April 3, 2007
 * @brief Definition of a Cayuga message.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _CMESSAGE_H
#define _CMESSAGE_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

namespace Cayuga {
namespace BasicDataStructure {
/**
 * @brief A CMessage is a Cayuga message consisting of a sequence of fields.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date April 3, 2007
 * @version 1.0
 */
class CMessage {
public:
	/**
	* @brief Constructor
	* @param msgType The message type
	* @param delim The character delimiter of fields
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 3, 2007
	* @version 1.0
	*/
	CMessage(const string& msgType,
		const char delim);

	/**
	* @brief add a new field at the end of this message
	* @param field The new field to append at the end of this message
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 3, 2007
	* @version 1.0
	*/
	void appendField(const string& field);

	/**
	* @brief Output this message in according to Cayuga message format.
	* @param out The output stream handle.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 3, 2007
	* @version 1.0
	*/
	void write(ostream& out = cout) const;

private:
	/** This variable stores the sequence of fields in this message.*/
	vector<string> _fields;

	/** This variable stores the message type.*/
	const string _msgType;

	/** This variable stores the character delimiter used in this message.*/
	const char _delim;
}; //class CMessage
}  //namespace BasicDataStructure
}  //namespace Cayuga



#endif //_CMESSAGE_H
