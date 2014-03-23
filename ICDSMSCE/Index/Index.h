/**
 * @file Index.h
 * @date November 23, 2005
 * @brief Base class of predicate index.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _INDEX_H
#define _INDEX_H

#ifdef STATS_BUILD
	#define STATS_ONLY(x) x;
#else
	#define STATS_ONLY(x) ;
#endif

#include <iostream>
#include <vector>
using namespace std;

#include "arch-dep.h"

namespace Cayuga {
namespace BasicDataStructure {
namespace Automaton {
class Node;
}
}


namespace Index {
using Cayuga::BasicDataStructure::Automaton::Node;

/**
 * @brief An Index object represents a predicate index
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 23, 2005
 * @version 1.0
 */
template <typename PRED_T, typename ID_T, typename ID_SET_ENTRY, typename ID_SET>
class Index {
public: 
	//typedef hash_set<ID_T> ID_SET;

	/**
	* @brief Default constructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 23, 2005
	* @version 1.0
	*/
	Index() {
	}

	/**
	* @brief Virtual destructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 23, 2005
	* @version 1.0
	*/
	virtual ~Index() {
	}

	/**
	* @brief Output member variable information in XML format.
	* @param out The output stream handle.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 23, 2005
	* @version 1.0
	*/
	virtual void write(ostream& out = cout) = 0;
		
	/**
	* @brief Output the statistics collected in this index.
	* @param out The output stream handle.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date October 27, 2006
	* @version 1.0
	*/
	virtual void writeStats(ostream& out = cout) = 0;
	
	/**
	* @brief Evaluates the current event against the set of predicates indexed 
			 here, and returns the IDs of matched predicates
	* @param ev The content of the current event
	* @param predIDs The set of satisfied predicate IDs to return
	* @param pNode NULL if this index works globally. Not NULL if this index
	         only works on predicates on edges associated with this node.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 23, 2005
	* @version 1.0
	*/
	virtual void eval(const char* ev, 
		ID_SET*& predIDs, 
		const Node* pNode = NULL) = 0;

	/**
	* @brief Insert the predicate into this index.
	* @param s The predicate to insert
	* @param predID The ID of this predicate
	* @param predIDEntry The ID set entry of this predicate
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 23, 2005
	* @version 1.0
	*/  
	virtual void insertPred(PRED_T s, 
		const ID_T predID, 
		const ID_SET_ENTRY predIDEntry) = 0;

	/**
	* @brief Delete the predicate from this index.
	* @param predID The ID of the predicate to delete
	* @return True iff this predicate is located and deleted from this index.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 23, 2005
	* @version 1.0
	*/  
	virtual bool deletePred(ID_T predID) = 0;
	
	/**
	* @brief Collect statistics on how well this index works.
	* @param stats The container of statistics
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 23, 2005
	* @version 1.0
	*/ 
	STATS_ONLY(virtual void collectStats(Stats& stats)  = 0);

	/** Delete all predicates. */
	//void clear();

	/** bulk loads the predicates into the pub/sub index. 
	*@param preds predicates to be bulk-loaded to the pub/sub index. 
	*@param predIDs stores the ids of the predicates assigned by the pub/sub index. 
	There is a one-to-one correspondence between a predicate and a predicate id. */
	//virtual void bulkLoad(const vector<PRED_T*>& preds, vector<ID_T>& predIDs) = 0;

private:	
	/** number of predicates in the engine. For developer's internal usage. */
	//size_t size();

	/** write the subscriptions stored in the pub/sub index to the screen in some readable format. 
	We should use XML format here. */
	//virtual void write(ostream& out = cout) = 0;
}; //class Index
}  //namespace Index
}  //namespace Cayuga

#endif //_INDEX_H
