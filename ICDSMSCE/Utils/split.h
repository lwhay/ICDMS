/**
 * @file split.h
 * @date April 7, 2007
 * @brief Forward declaration of Cayuga::Utils::split().
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */
#ifndef _SPLIT_H
#define _SPLIT_H

namespace Cayuga {
namespace Utils {
/**
* @brief Split multiple fields in a string into a vector of strings
* @param input The input stream containing at least 1 field
* @param output The output vector of strings
* @param delim The character field delimiter in the input stream
* @author Mingsheng Hong (mshong@cs.cornell.edu)
* @date Feburary 16, 2006
* @version 1.0
*/
void split(const string& input,
		   vector<string>& output,
		   char delim = ';');
}  //namespace Utils
}  //namespace Cayuga

#endif //_SPLIT_H
