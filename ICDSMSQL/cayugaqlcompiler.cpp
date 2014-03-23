/**
 * @file CayugaQLCompiler.cpp
 * @date April 4, 2007
 * @brief This file defines CayugaQLCompiler class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "CayugaQLCompiler.h"

//#include "CayugaQLASTVisitor.h"
#include "StreamExprTransformer.h"
#include "SchemaMapBuilder.h"
#include "TypeChecker.h"
#include "DNFPredGen.h"
#include "NFAGen.h"
#include "WindowGen.h"
#include "predWindowGen.h"

using namespace Cayuga::QLCompiler;

#include "antlr/AST.hpp"
#include "L2.hpp"
#include "P2.hpp"
using namespace antlr;

#include "../ICDSMSCE/BasicDataStructure/Automaton/Query.h"

#include "../ICDSMSCE/SAXHandler/ConfigMgr.h"
using namespace Cayuga::SAXHandler;

#include "../ICDSMSCE/SystemMgr/Options.h"
#include "../ICDSMSCE/SystemMgr/StreamSchemaMgr.h"
using namespace Cayuga::SystemMgr;

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

CayugaQLCompiler::CayugaQLCompiler(Query& this_nfa_dag) 
: _nfa_dag(this_nfa_dag) {
}

CayugaQLCompiler::~CayugaQLCompiler(void) {
}

int CayugaQLCompiler::compile(const vector<string>& queryFileNames,
							  ostream& debugOut) {
	size_t succNum = 0;
	for (size_t fileIdx = 0; fileIdx < queryFileNames.size(); ++fileIdx) {
		if (fileIdx % 1 == 0) {
			debugOut
				<<"opening query file "
				<<queryFileNames[fileIdx]<<endl;
		}
		try { 
			//cout<<"opening file "<<filename<<endl;
			ifstream in(queryFileNames[fileIdx].c_str());
			if (!in) {
				throw string("cannot open file");
			}
			_compileAQuery(in); 
			++succNum;
		}
		catch (string &s) {  
			//Ideally, this exception which stems from ValueIDMap OR AIR_Handler
			//should be a more "sophisticated" type, such as a CayugaException.
			cerr << "Cayuga could not compile query from input file " 
				<< queryFileNames[fileIdx] <<" "<< s <<endl;

			if (ConfigMgr::getInstance()->getOptions()->_bStrict) {
				throw s;
			}
			else continue;
		}
	}

	//merge all the queries
	for(vector<Query*>::iterator it = _queries.begin();
		it != _queries.end(); ++it) {
			_nfa_dag.mergeQuery(*it);
	}

	debugOut<<succNum<<" out of "
		<<queryFileNames.size()
		<<" queries are compiled successfully."<<endl;
	if (succNum == queryFileNames.size())
		return 0;
	
	//if some query compilation failed, return -1
	return -1;
}

void CayugaQLCompiler::_compileAQuery(istream& in) {
	L2 lexer(in);
	P2 parser(lexer);

	ASTFactory my_factory("CayugaQLAST", CayugaQLAST::factory);
	parser.initializeASTFactory(my_factory);
	parser.setASTFactory(&my_factory);
	//parser.setASTNodeFactory( BasicAST::factory() );

	// invoke the parser...
	parser.query();
	//cout<<"parsing successful!"<<endl;

	//get AST
	RefCayugaQLAST ast = RefCayugaQLAST(parser.getAST());

	//luoluo 3.13
	cout<<"print AST"<<endl;
	//if (ast)
	cout << ast->toStringList() << endl;
	//else
	//	cout << "null AST" << endl;
	assert (ast);
	if (ConfigMgr::getInstance()->getOptions()->_bVerbose) {
		cout << ast->toStringList() << endl;
	}
	
	_iterateAST(ast);

}

void CayugaQLCompiler::_iterateAST(RefCayugaQLAST& ast) {
	vector<CayugaQLASTWalker*> walks;
	walks.push_back(new WindowGen());
	walks.push_back(new StreamExprTransformer());
	//luoluo 7-29
// 	walks.push_back(new predWindowGen());

	walks.push_back(new TypeChecker());
	walks.push_back(new SchemaMapBuilder());
	walks.push_back(new DNFPredGen());
	walks.push_back(new NFAGen(_queries));


	for (vector<CayugaQLASTWalker*>::iterator it = walks.begin();
		it != walks.end(); ++it) {
		(*it)->visit(ast);

		delete *it;
	}
}
