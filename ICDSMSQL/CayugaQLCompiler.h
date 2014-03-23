/**
 * @file CayugaQLCompiler.h
 * @date April 4, 2007
 * @brief This file declares CayugaQLCompiler class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#pragma once

#include <iostream>
#include <string>
#include <vector>
using namespace std;

#include "CayugaQLAST.h"

namespace Cayuga {
namespace BasicDataStructure {
namespace Automaton {
class Query;
}
}

namespace QLCompiler {
using Cayuga::BasicDataStructure::Automaton::Query;

/**
 * @brief A CayugaQLCompiler object represents a CEL query compiler.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date April 4, 2007
 * @version 1.0
 */
class CayugaQLCompiler {
public:
	/**
	* @brief Default constructor
	* @param this_nfa_dag Reference to the input set of queries in the system.
		Ownership of this_nfa_dag still belongs to the caller. However
		CayugaQLCompiler can modify it by merging more queries with it.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 4, 2007
	* @version 1.0
	*/
	CayugaQLCompiler(Query& this_nfa_dag);
	
	/**
	* @brief Destructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 4, 2007
	* @version 1.0
	*/
	~CayugaQLCompiler(void);

	/**
	* @brief Compile the queries stored in the input set of files and merge
			them into the current set of queries in the system
	* @param queryFileNames The set of names of the query files
	* @param debugOut The debug message output stream
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 4, 2007
	* @version 1.0
	*/
	int compile(const vector<string>& queryFileNames,
		ostream& debugOut = cout);

private:
	/**
	* @brief Specifies a sequence of AST walkers such as semantic checker,
			AIR generator and so on, and apply each of them to the AST.
	* @param ast The input AST to transform.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 4, 2007
	* @version 1.0
	*/
	void _iterateAST(RefCayugaQLAST& ast);

	/**
	* @brief Compile one query from the input istream handle
	* @param in The istream handle from which we can read the CEL query
			expression.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 4, 2007
	* @version 1.0
	*/
	void _compileAQuery(istream& in);

	/** This variable stores the set of queries in the system. */
	Query& _nfa_dag;

	/** This variable stores the set of newly created queries. */
	vector<Query*> _queries;
};//class CayugaQLCompiler
} //namespace QLCompiler
} //namespace Cayuga
