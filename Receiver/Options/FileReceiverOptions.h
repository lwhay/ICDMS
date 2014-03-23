/**
 * @file FileReceiverOptions.h
 * @date March 23, 2007
 * @brief The file declares the interface for FileReceiverOptions.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _FILE_RECEIVER_OPTIONS_H
#define _FILE_RECEIVER_OPTIONS_H

namespace Cayuga {
namespace Receiver {
/**
  * @brief A FileReceiverOptions object stores the options for a file 
			receiver.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date March 23, 2007
 * @version 1.0
 */
class FileReceiverOptions {
public:
	FileReceiverOptions(const string& fileNames, 
		const char charDelim,
		bool bStrict)
		: _fileNames(fileNames), _charDelim(charDelim), _bStrict(bStrict) {
	}

	const string& _fileNames;
	char _charDelim;
	bool _bStrict;
}; //class FileReceiverOptions
}  //namespace Receiver
}  //namespace Cayuga

#endif //_FILE_RECEIVER_OPTIONS_H
