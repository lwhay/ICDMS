/**
 * @file Options.cpp
 * @date October 20, 2006
 * @brief Implementation file of Options class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "SystemMgr/Options.h"
using namespace Cayuga::SystemMgr;

#include "Utils/split.h"
#include "Utils/AttrMap.h"
using namespace Cayuga::Utils;

void Options::setOptions(const StringMap& optionMap) {
	_setOption<bool>(optionMap, 
		_saveInternalState, 
		"SaveInternalState", 
		false);
	
	//Query
	_setOption<DataSource>(optionMap, 
		_queryInputMode, 
		"QueryInputMode", 
		Cayuga::SAXHandler::FILE);

	_setOption<string>(optionMap, 
		_queryInputName, 
		"QueryInputName", 
		"");
	
	_setOption<unsigned>(optionMap, 
		_queryNumber, 
		"QueryNumber", 
		0);

	_setOption<bool>(optionMap, 
		_bAirQueries, 
		"AirQuery", 
		true);
	
	//Doc
	string schemaFileName;
	_setOption<string>(optionMap, 
		schemaFileName, 
		"StreamSchema", 
		"");

	split(schemaFileName,
		_streamSchemaFiles);

	_setOption<DataSource>(optionMap, 
		_docInputMode, 
		"DocInputMode", 
		Cayuga::SAXHandler::FILE);

	_setOption<string>(optionMap, 
		_docInputName, 
		"DocInputName", 
		"");
	
	//_setOption<string>(optionMap, 
	//	_docInputStream, 
	//	"DocInputStream", 
	//	"XML");

	_setOption<bool>(optionMap, 
		_bXMLStream, 
		"XMLStream", 
		false);
	
	_setOption<unsigned>(optionMap, 
		_docNumber, 
		"DocNumber", 
		1);

	_setOption<short>(optionMap, 
		_nERPort, 
		"ERPort", 
		0);

	_setOption<string>(optionMap,
		_LocalNICInterface,
		"LocalNICInterface",
		"");
				
	_setOption<char>(optionMap, 
		_attrDelimiter, 
		"AttrDelimiter", 
		',');

	//GC
	_setOption<unsigned>(optionMap, 
		_gcSize, 
		"GCSize", 
		32);	

	//this variable is obsolete
	//_setOption<unsigned>(optionMap, 
	//	_gcFrequency, 
	//	"GCFrequency", 
	//	1000);

	_setOption<string>(optionMap, 
		_gcSizeUnit, 
		"GCSizeUnit", 
		"MB");

	//system
	_setOption<bool>(optionMap, 
		_bVerbose, 
		"Verbose", 
		false);

	_setOption<unsigned>(optionMap, 
		_printFrequency, 
		"PrintFrequency", 
		1000);

	_setOption<string>(optionMap, 
		_debugMessageDestination, 
		"DebugMessageDestination", 
		"");

	_setOption<bool>(optionMap, 
		_bRecordTrace, 
		"RecordTrace", 
		true);

	_setOption<short>(optionMap, 
		_nTracePort, 
		"TracePort", 
		0);

	_setOption<short>(optionMap, 
		_nOutputEventPort, 
		"OutputEventPort", 
		0);

	_setOption<string>(optionMap,
		_OutputEventHost,
		"OutputEventHost",
		"");

	_setOption<string>(optionMap, 
		_compiledAIRLogDir, 
		"CompiledAIRLogDir", 
		"");	
	normalizeDirectory(_compiledAIRLogDir);

	_setOption<string>(optionMap, 
		_commandLogDir, 
		"CommandLogDir", 
		"");	
	normalizeDirectory(_commandLogDir);

	_setOption<string>(optionMap, 
		_witnessLogDir, 
		"WitnessLogDir", 
		"./");	
	normalizeDirectory(_witnessLogDir);
	_bLogOutput = (!_witnessLogDir.empty());

	_setOption<bool>(optionMap, 
		_bMeasure, 
		"Measure", 
		false);

	_setOption<short>(optionMap, 
		_nMeasurePort, 
		"MeasurePort", 
		0);
		
	_setOption<unsigned>(optionMap, 
		_eventWindowLen, 
		"EventWindowLen", 
		0xffff);

	_setOption<long>(optionMap, 
		_checkPointFrequency, 
		"CheckPointFrequency", 
		5000);	
	
	_setOption<string>(optionMap, 
		_checkPointAndTraceDir, 
		"CheckPointAndTraceDir", 
		".");	
	//normalize _checkPointAndTraceDir: if it does not end with /, add it
	normalizeDirectory(_checkPointAndTraceDir);

	_setOption<bool>(optionMap, 
		_bStrict, 
		"Strict", 
		false);	

	//run-time commands
	_setOption<short>(optionMap, 
		_nCommandPort, 
		"CommandPort", 
		0);

	//CEL compiler
	_setOption<bool>(optionMap, 
		_bInlineQuery,
		"InlineQuery", 
		true);

	_setOption<bool>(optionMap, 
		_bMergeStates,
		"MergeStates", 
		true);

}

template <typename T>
void Options::_setOption(const StringMap& optionMap,
						T& memberVar, 
						const string& optionName,
						const T& defaultValue) {
	StringMap::const_iterator it = optionMap.find(optionName);
	if (it == optionMap.end()) {
		memberVar = defaultValue;
	}
	else {
		memberVar = convert<T>(it->second);
	}
}

void Options::normalizeDirectory(string& dirName) {
	if (!dirName.empty() && dirName[dirName.size()-1] != '/')
		dirName += "/";
}
