/**
 * @file FileRelER.h
 * @date Feburary 22, 2006
 * @brief The file declares the calss FileRelER.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _FILE_REL_ER_H
#define _FILE_REL_ER_H


#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

#include "../Receiver/ER/RelER.h"


namespace Cayuga {
namespace Receiver {
class FileReceiverOptions;

/**
 * @brief A FileRelER object reads relational events from disk files.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 26, 2005
 * @version 1.0
 */
class FileRelER : public RelER<FileReceiverOptions> {
public:
	/**
	* @brief Default constructor.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 26, 2005
	* @version 1.0
	*/
	FileRelER();

	/**
	* @brief Start receiving inputs according to the input options.
	* @param options The options for this receiver.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date March 23, 2007
	* @version 1.0
	*/
	virtual void open(const FileReceiverOptions* options);

}; //class FileRelER
}  //namespace Receiver
}  //namespace Cayuga

#endif //_FILE_REL_ER_H
