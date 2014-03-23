/**
 * @file ConjunctiveClause.h
 * @date November 30, 2005
 * @brief Definition of a conjunctive clause of atomic predicates in Cayuga
		  engine.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _CONJUNCTIVE_CLAUSE_H
#define _CONJUNCTIVE_CLAUSE_H

#include <vector>
#include <iostream>
using namespace std;

namespace Cayuga {
namespace AutomatonCompiler {
class ConjunctiveClauseString;
class AtomPredString;
}

namespace BasicDataStructure {
namespace Automaton {
class Instance;
}

namespace EventNS {
class Event;
}

namespace Predicate {
struct DurationStruct;

/**
 * @brief A ConjunctiveClause object represents a conjunctive clause of 
          atomic predicates for event processing.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 30, 2005
 * @version 1.0
 */
class ConjunctiveClause {
	friend class Cayuga::AutomatonCompiler::ConjunctiveClauseString;
	typedef vector<Cayuga::AutomatonCompiler::AtomPredString*> AtomPredSet;
public:
	/**
	* @brief Default constructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 22, 2006
	* @version 1.0
	*/
	ConjunctiveClause();

	/**
	* @brief Virtual destructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 22, 2006
	* @version 1.0
	*/
	~ConjunctiveClause();
	
	/**
	* @brief Output member variable information in XML format.
	* @param out The output stream handle.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 30, 2005
	* @version 1.0
	*/
	void write(ostream& out = cout) const;

	/**
	* @brief Set the Static predicate program for this clause.
	* @param p The program to set to
	* @param sz The size of the program
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 30, 2005
	* @version 1.0
	*/
	void setStaAtomPredProgram(char* p, size_t sz);

	/**
	* @brief Set the Dynamic predicate program for this clause.
	* @param p The program to set to
	* @param sz The size of the program
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 30, 2005
	* @version 1.0
	*/
	void setDynAtomPredProgram(char* p, size_t sz);

	/**
	* @brief Add the predicate program for an indexed Static predicate.
	* @param p The program to add
	* @param sz The size of the program
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 30, 2005
	* @version 1.0
	*/
	void addIndexedStaAtomPred(const char* p, size_t sz);

	/**
	* @brief Add the predicate program for an indexed Dynamic predicate.
	* @param p The program to add
	* @param sz The size of the program
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 30, 2005
	* @version 1.0
	*/
	void addIndexedDynAtomPred(const char* p, size_t sz);

	/**
	* @brief Evaluate the un-indexed (remaining) dynamic predicates on this clause.
	* @param ev The pointer of the current event
	* @param pInst The pointer of the current instance
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 30, 2005
	* @version 1.0
	*/
	bool evalDynamic(const Cayuga::BasicDataStructure::EventNS::Event* ev, 
		Cayuga::BasicDataStructure::Automaton::Instance* pInst);

	/**
	* @brief Evaluate the un-indexed (remaining) static predicates on this clause.
	* @param ev The pointer of the current event
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 30, 2005
	* @version 1.0
	*/
	bool evalStatic(const Cayuga::BasicDataStructure::EventNS::Event* ev);

	/**
	* @brief Equality comparison operator.
	* @param rhs The input clause for this clause to compare to
	* @return True iff the two clauses have the same indexed and un-indexed 
	          predicates
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 30, 2005
	* @version 1.0
	*/
	bool operator == (const ConjunctiveClause& rhs) const;
	
	/**
	* @brief Inequality comparison operator.
	* @param rhs The input clause for this clause to compare to
	* @return True iff the two clauses does not have the same indexed and 
	          un-indexed predicates
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 30, 2005
	* @version 1.0
	*/
	bool operator != (const ConjunctiveClause& rhs) const;

private:
		
	/**
	* @brief Check equivalence of two possibly dull duration structs.
	* @param lhs The first struct
	* @param rhs The second struct
	* @return True iff the two structs are the same
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date October 24, 2007
	* @version 1.0
	*/
	bool checkDurationStruct(DurationStruct* lhs, DurationStruct* rhs) const;

	/** 
	* This variable stores the predicate program for static un-indexed predicates.
	*/
	char* _atomStaPredsProgram;

	/** 
	* This variable stores the predicate program for dynamic un-indexed predicates.
	*/
	char* _atomDynPredsProgram;

	/** 
	* This variable stores the size of the un-indexed static predicate program.
	*/
	size_t _szStaPredProgram;

	/**
	* This variable stores the size of the un-indexed dynamic predicate program.
	*/
	size_t _szDynPredProgram;

	/** 
	* This variable stores duration constraint on the incoming event.
	* NULL means there is no such constraint.
	*/
	DurationStruct* _pEventDur;

	/** 
	* This variable stores duration constraint on the incoming event 
	* concatenated with the instance .
	* NULL means there is no such constraint.
	*/
	DurationStruct* _pCompositeDur;

	/** 
	* This variable stores the programs of all the indexed Static atomic predicates.
	*/
	vector<pair<const char*, size_t> > _indexedStaAtomPreds;

	/**
	* This variable stores the programs of all the indexed Dynamic atomic predicates.
	*/
	vector<pair<const char*, size_t> > _indexedDynAtomPreds;

	/**
	* This variable stores the set of atomic predicates in this clause. They
	* are only used by write() function.
	*/
	AtomPredSet _atomPreds;

}; //class ConjunctiveClause
}  //namespace Predicate
}  //namespace BasicDataStructure
}  //namespace Cayuga
#endif //_CONJUNCTIVE_CLAUSE_H
