/**
 * @file RelER.h
 * @date April 4, 2007
 * @brief The file declares the calss RelER.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _REL_ER_H
#define _REL_ER_H


#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

#include "../Receiver/receiver.h"

namespace Cayuga {
namespace Receiver {

/**
* @brief Parse the input event and if it succeeds, put the event in 
		 priority queue.
* @param line The input event in string format
* @param charDelim The field delimiter used in line
* @param bStrict If true, throw exception when the parsing fails. otherwise
	     simply return.
* @author Mingsheng Hong (mshong@cs.cornell.edu)
* @date April 4, 2007
* @version 1.0
*/
void parseEvent(const string& line, char charDelim, bool bStrict);

/**
 * @brief A RelER object reads relational events. This is an interface.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date April 4, 2007
 * @version 1.0
 */
template<typename ReceiverOptionT>
class RelER : public Receiver<ReceiverOptionT> {
public:
	/**
	* @brief Start receiving inputs according to the input options.
	* @param options The options for this receiver.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 4, 2007
	* @version 1.0
	*/
	virtual void open(const ReceiverOptionT* options) = 0;

}; //class RelER
}  //namespace Receiver
}  //namespace Cayuga

#endif //_REL_ER_H
