/**
* @file FileRelER.cpp
* @date April 19, 2006
* @brief This file defines the class FileRelER.
* @author Mingsheng Hong (mshong@cs.cornell.edu)
*/

#include "../Receiver/ER/FileRelER.h"
#include "../Receiver/Options/FileReceiverOptions.h"
using namespace Cayuga::Receiver;

#include "../ICDSMSCE/Utils/split.h"
using namespace Cayuga::Utils;

#include <string>
#include <vector>
using namespace std;

FileRelER::FileRelER() {
}

void FileRelER::open(const FileReceiverOptions* options) {
	const string& strFileNames = options->_fileNames;
	char charDelim = options->_charDelim;
	bool bStrict = options->_bStrict;
	vector<string> inputFileNames;
	split(strFileNames, inputFileNames, ';'); 

	for (size_t i = 0; i < inputFileNames.size(); ++i) {
		if (inputFileNames[i] == "") continue;
		ifstream inFile(inputFileNames[i].c_str());
		string line;
		while (getline(inFile, line)) {
			parseEvent(line, charDelim, bStrict);
		}
		inFile.close();
	}
}
