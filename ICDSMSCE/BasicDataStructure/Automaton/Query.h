/**
 * @file Query.h
 * @date November 23, 2005
 * @brief Stores the information of query automata for event processing in 
	      Cayuga engine.

   We decide to use one class to represent both a single query 
   automaton and a general automaton DAG in Cayuga resulted from 
   merging multiple query automata.

 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _QUERY_H
#define _QUERY_H

#include <vector>
#include <iostream>
using namespace std;
/*#include "BasicDataStructure/Window/Window.h"*/
#include "BasicDataStructure/Window/inputWindow.h"
#include "BasicDataStructure/Window/predWindow.h"

#include "Utils/Singleton.h"
using Cayuga::Utils::Singleton;

namespace Cayuga {
namespace QueryProcessor {
class FilterIndex;
class FilterEvaluator;
class FREvaluator;
class QueryProcessorImpl;
}

namespace BasicDataStructure {
namespace Automaton {
class Node;
class StartNode;
class IntermediateNode;
//class Window;

/**
 * @brief A Query object represents a query automaton in Cayuga engine.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 23, 2005
 * @version 1.0
 */
class Query : public Singleton<Query>{
	friend class Cayuga::QueryProcessor::FilterIndex;
	friend class Cayuga::QueryProcessor::FilterEvaluator;
	friend class Cayuga::QueryProcessor::FREvaluator;
	friend class Cayuga::QueryProcessor::QueryProcessorImpl;
	friend class Cayuga::Utils::Singleton<Query>;
public:
	typedef vector<Node*> NodeSet;

	
	/**
	* @brief Output member variable information in XML format.
	* @param out The output stream handle.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 23, 2005
	* @version 1.0
	*/
	void write(ostream& out = cout) const;

	/**
	* @brief Add a node to this query.
	* @param pNode Pointer to the node to add
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 23, 2005
	* @version 1.0
	*/
	void addNode(Node* pNode);
// 	/**
// 	*@brief Add a window to this query.
// 	*@param pWindow Pointer to the window to add
// 	*@author Rainbow
// 	*@date 20011-4-8
// 	*/
// 	void addWindow(Window* pWindow);

	/**
	*@brief		Add a inputwindow to this query
	*@param pWindow Pointer to the window to add
	*@author leixiaoluo
	*@date 20011-5-20
	*/
	void addinputWindow(inputWindow * ptrWindow);

// 	//luoluo 7-29
// 	void addpredWindow(predWindow * ptrPredWindow);


	/**
	* @brief Merge the input query automaton with existing automata.
	* @param q Pointer to the query to merge with
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 23, 2005
	* @version 1.0
	*/
	void mergeQuery(Query* q);

	/**
	* @brief Return the number of nodes in this query.
	* @return As above.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 23, 2006
	* @version 1.0
	*/
	size_t getSize();

	/**
	* @brief Print the occupancy of each IntermediateNode
	* @param OutputStream object to write to, cout by default
	* @author Varun Sharma (varun13687@yahoo.com)
	* @date June 22, 2006
	* @version 1.0
	*/
	//void printOccupancy(ostream& out = cout);
		/**
	*@brief		This variable stores the set of window in the automata .
	*@author leixiaoluo
	*@date		2011-5-20
	*/
/*	vector<inputWindow*> _inputwindows;*/
	//luoluo 5-28
	static inputWindow * _inputwindows ;

// 	//luoluo 7-29
// 	//there 
// 	vector <predWindow*>_predwindow;

private:
	/**
	* @brief Recursively merge the automaton tree rooted at the input node q to 
	*		 node Q.
	* @param q Pointer to the node in the input query
	* @param Q Pointer to the node in the existing system
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 6, 2006
	* @version 1.0
	*/
	void mergeNode(StartNode* q, StartNode* Q);

	/**
	* @brief Push the successor nodes of q all to intermediateNodes. Used in
			 state merging procedure.
	* @param q Pointer to the node to add
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 6, 2006
	* @version 1.0
	*/
	void pushback(IntermediateNode* q);

	/** 
	* This variable stores the set of start nodes in the automata.
	*/
	vector<Node*> _startNodes;
	
	/** 
	* This variable stores the set of intermediate or end nodes in the automata.
	*/
	vector<Node*> _intermediateNodes;
// 	/**
// 	*This variable stores the set of window  in the  automata
// 	*@author Rainbow
// 	*@date 2001-4-8
// 	*/
// 	vector<Window*> _windows;
// 	/**
// 	*@brief		This variable stores the set of window in the automata .
// 	*@author leixiaoluo
// 	*@date		2011-5-20
// 	*/
// 	vector<inputWindow*> _inputwindows;
/*	vector<outputWindow*>_outputwindows;*/

}; //class Query
}  //namespace Automaton
}  //namespace BasicDataStructure
}  //namespace Cayuga


#endif //_QUERY_H
