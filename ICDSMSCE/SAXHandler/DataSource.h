/**
 * @file DataSource.h
 * @date December 8, 2005
 * @brief Definition of the data source for input queries and documents. 
		  
		  Put in a separate file to make it easy for other files to include it.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _DATA_SOURCE_H
#define _DATA_SOURCE_H

namespace Cayuga {
namespace SAXHandler {

/**
 * @brief A DataSource object represents the data source of input queries or 
		  documents. 
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date December 8, 2005
 * @version 1.0
 */
enum DataSource {
	FILE,
	DIR,
	NETWORK
}; //enum DataSource
}  //namespace SAXHandler
}  //namespace Cayuga

#endif //_DATA_SOURCE_H
