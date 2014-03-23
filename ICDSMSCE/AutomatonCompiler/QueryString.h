/**
 * @file QueryString.h
 * @date November 27, 2005
 * @brief Stores the information of a query automaton read from AIR.

   We decide to use one class to represent both a single query automaton and 
   a general automaton DAG in Cayuga resulted from merging multiple query 
   automata.

 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _QUERY_STRING_H
#define _QUERY_STRING_H

#include <iostream>
#include <vector>
using namespace std;



namespace Cayuga {
namespace BasicDataStructure {
namespace Automaton {
class Query;
}
}

namespace AutomatonCompiler {
using Cayuga::BasicDataStructure::Automaton::Query;
class NodeString;
class StartNodeString;
class IntermediateNodeString;
class ForwardEdgeString;
class WindowString;
//luoluo 7-28
class predWindowString;

/**
 * @brief A QueryString object represents a query automaton constructed from 
		  SAX events.	
 * @see	EdgeString
 * @see NodeString
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 27, 2005
 * @version 1.0
 */
class QueryString {
public:
	typedef vector<StartNodeString*> StartNodeSet;
	typedef vector<IntermediateNodeString*> IntermediateNodeSet;

	/**
	* @brief Compiles this query into Cayuga engine, by translating
			 strings into integers with symbol tables, and so on.
	* @return Pointer to the compiled query.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	QueryString();
	Query* compile();

	/**
	* @brief Output member variable information in XML format, aiming at 
			 reconstructing the XML piece in AIR which encodes this query
	* @param out The output stream handle.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	void write(ostream& out = cout);

	/**
	* @brief Add an automaton node to this query.
	* @param pNode Pointer to the node to add.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	void addNode(NodeString* pNode);

	
	/**
	* @brief Get the last forward edge of this query.
	* @Return As above.
	* @invariant This query must be a non-empty line automaton.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 18, 2007
	* @version 1.0
	*/
	ForwardEdgeString* getLastForwardEdge();

	/**
	* @brief Get the first forward edge of this query.
	* @Return As above.
	* @invariant This query must be a non-empty line automaton.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 19, 2007
	* @version 1.0
	*/
	ForwardEdgeString* getFirstForwardEdge();

	/**
	* @brief Get the start state of this query.
	* @Return As above.
	* @invariant This query must be a non-empty line automaton.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 18, 2007
	* @version 1.0
	*/
	StartNodeString* getStartNode();

	/**
	* @brief Get the second state of this query.
	* @Return As above.
	* @invariant This query must be a non-empty line automaton.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 19, 2007
	* @version 1.0
	*/
	IntermediateNodeString* getSecondNode();

	/**
	* @brief Get the second to end state of this query.
	* @Return As above.
	* @invariant This query must be a non-empty line automaton with no fewer
				 than 3 states.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 19, 2007
	* @version 1.0
	*/
	IntermediateNodeString* getSecondToEndNode();
	
	/**
	* @brief Get the end state of this query.
	* @Return As above.
	* @invariant This query must be a non-empty line automaton.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 18, 2007
	* @version 1.0
	*/
	IntermediateNodeString* getEndNode();
    
	//added by rainbow
	WindowString* _thisWindow;
    //luoluo 7-28
	predWindowString * _thisPredWindow;


private:
	/**
	* This variable stores the start nodes of this query read from AIR.
	*/
	StartNodeSet _startNodes;
	
	/**
	* This variable stores the intermediate or end nodes of this query read 
	* from AIR.
	*/
	IntermediateNodeSet _intermediateNodes;

}; //class QueryString
}  //namespace AutomatonCompiler
}  //namespace Cayuga

#endif //_QUERY_STRING_H
