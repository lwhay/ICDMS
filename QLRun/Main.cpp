#include "../ICDSMSQL/CayugaQLCompiler.h"
using namespace Cayuga::QLCompiler;

#include "../ICDSMSCE/BasicDataStructure/Automaton/Query.h"
using namespace Cayuga::BasicDataStructure::Automaton;

#include "../ICDSMSCE/SystemMgr/StreamSchemaMgr.h"
using namespace Cayuga::SystemMgr;

#include "../ICDSMSCE/Utils/AttrMap.h"
using Cayuga::Utils::toString;

#include "../ICDSMSCE/CayugaSystem.h"
using namespace Cayuga;

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void insertUDFs();

int main( int argc, char** argv) {
	//ANTLR_USING_NAMESPACE(std)
	//ANTLR_USING_NAMESPACE(antlr)

	CayugaSystem* pCayuga = CayugaSystem::getInstance();
	pCayuga->init("Cayuga/test/CayugaQL/ConfigCEL.xml");

	StreamSchema schema;
	schema.putAttributeAtEnd("Name", "string");
	schema.putAttributeAtEnd("Price", "float");
	//schema.putAttributeAtEnd("Volume", "int");
	StreamSchemaMgr::getInstance()->insertNewSchema("Stock1", schema);
//	StreamSchemaMgr::getInstance()->insertNewSchema("Stock1", schema);
// 	StreamSchemaMgr::getInstance()->insertNewSchema("Stock2", schema);
	//StreamSchemaMgr::getInstance()->insertNewSchema("Stock3", schema);

	//already loaded
	//StreamSchema dualSchema;
	//StreamSchemaMgr::getInstance()->insertNewSchema("DUAL", dualSchema);

	insertUDFs();

	assert (argc == 3);
	int startTestNum = atoi(argv[1]);
	int numTests = atoi(argv[2]);

	string baseName("test");
	
	vector<string> queryFileNames;
	int endTestNum = startTestNum + numTests;
	for (int i = startTestNum; i < endTestNum; ++i) {
		string suffix = toString(i);
		string testName = baseName + suffix;
		testName += ".txt";
		testName = "Cayuga/test/CayugaQL/tests/" + testName;

		queryFileNames.push_back(testName);
		//bool ret = test(testName);
	}

	Query nfa_dag;
	CayugaQLCompiler compiler(nfa_dag);
	compiler.compile(queryFileNames);

	return 0;
}
