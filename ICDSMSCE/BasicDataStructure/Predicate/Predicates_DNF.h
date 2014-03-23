/**
 * @file Predicates_DNF.h
 * @date November 30, 2005
 * @brief Definition of a DNF of edge predicate in Cayuga engine.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _PREDICATES_DNF_H
#define _PREDICATES_DNF_H

#include "ConjunctiveClause.h"

#include <cassert>
#include <iostream>
using namespace std;

namespace Cayuga {
namespace QueryProcessor {
class FilterIndex;
class FilterEvaluator;
class FREvaluator;
}

namespace BasicDataStructure {
namespace Automaton {
class Instance;
}

namespace EventNS {
class Event;
}

namespace Predicate {
/**
 * @brief A Predicates_DNF object represents a DNF of 
          edge predicate for event processing.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 30, 2005
 * @version 1.0
 */
class Predicates_DNF {
	friend class Cayuga::QueryProcessor::FilterIndex;
	friend class Cayuga::QueryProcessor::FREvaluator;
public:
	typedef vector<ConjunctiveClause*> ConjunctiveClauses;
	void addClause(ConjunctiveClause* pClause);

	/**
	* @brief Output member variable information in XML format.
	* @param out The output stream handle.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 30, 2005
	* @version 1.0
	*/
	void write(ostream& out = cout) const;

	/**
	* @brief Evaluate the un-indexed (remaining) predicates of sword clauses.
	* @param ev The pointer of the current event
	* @param pInst The pointer of the current instance
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 30, 2005
	* @version 1.0
	*/
	bool eval(const Cayuga::BasicDataStructure::EventNS::Event* ev, 
		Cayuga::BasicDataStructure::Automaton::Instance* pInst);

	/**
	* @brief Set a particular clause to be a sword clause.
	* @param clauseNo The ID of the clause to set
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 30, 2005
	* @version 1.0
	*/
	void setSwordDNFClause(size_t clauseNo);

	/**
	* @brief Clear the sword clauses of this DNF.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 20, 2006
	* @version 1.0
	*/
	void clearSwordClauses();

	/**
	* @brief Get the number of clauses in this DNF.
	* @return As above.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 30, 2005
	* @version 1.0
	*/
	size_t getNumClauses() const;

	/**
	* @brief Equality comparison operator.
	* @param rhs The input DNF for this DNF to compare to
	* @return True iff the two DNFs have the same set of clauses
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 30, 2005
	* @version 1.0
	*/
	bool operator == (const Predicates_DNF& rhs) const;
	
	/**
	* @brief Inequality comparison operator.
	* @param rhs The input DNF for this DNF to compare to
	* @return True iff the two DNFs do not have the same set of clauses
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 30, 2005
	* @version 1.0
	*/
	bool operator != (const Predicates_DNF& rhs) const;

private:
	/** 
	* This variable stores the set of clauses in this DNF.
	*/
	ConjunctiveClauses _clauses;
	
	/** 
	* This variable stores IDs of sword clauses.
	*/
	vector<size_t> _swordClauses;
}; //class Predicates_DNF
}  //namespace Predicate
}  //namespace BasicDataStructure
}  //namespace Cayuga

#endif //_PREDICATES_DNF_H
