/**
 * @file Predicates_DNFString.h
 * @date November 17, 2005
 * @brief Stores the information of a DNF of edge predicate read from AIR.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _PREDICATES_DNF_STRING_H
#define _PREDICATES_DNF_STRING_H


#include <iostream>
#include <set>
#include <string>
#include <vector>
using namespace std;

namespace Cayuga {
namespace BasicDataStructure {
namespace Predicate {
class Predicates_DNF;
}
}

namespace AutomatonCompiler {	
class SchemaMapString;
	
using Cayuga::BasicDataStructure::Predicate::Predicates_DNF;

class ConjunctiveClauseString;
class Context;

/**
 * @brief A Predicates_DNFString object represents a DNF of edge predicate 
		  constructed from SAX events.	
 * @see	ConjunctiveClauseString
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 17, 2005
 * @version 1.0
 */
class Predicates_DNFString {
public:
	typedef vector<ConjunctiveClauseString*> ConjunctiveClauses;

	/**
	* @brief Compiles this edge predicate into Cayuga engine.
	* @param context The compilation context
	* @return Pointer to the internalized edge predicate.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 17, 2005
	* @version 1.0
	*/
	Predicates_DNF* compile(Context& context);

	/**
	* @brief Output member variable information in XML format, aiming at 
			 reconstructing the XML piece in AIR which encodes this predicate
	* @param out The output stream handle.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 17, 2005
	* @version 1.0
	*/
	void write(ostream& out = cout) const;

	/**
	* @brief Add a conjunctive predicate clause to this DNF
	* @param pClause Pointer to the predicate clause to add
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 17, 2005
	* @version 1.0
	*/
	void addClause(ConjunctiveClauseString* pClause);

	/**
	* @brief Return true iff this predicate is a conjunction
	* @return As above.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 17, 2007
	* @version 1.0
	*/
	bool isConjunctive() const;
	
	/**
	* @brief Return true iff this predicate is empty
	* @return As above.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 17, 2007
	* @version 1.0
	*/
	bool isEmpty() const;

	/**
	* @brief Return true iff this predicate has a complex atomic predicate.
	* @return As above.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 17, 2007
	* @version 1.0
	*/
	bool hasComplexPredicate() const;

	/**
	* @brief Return the set of referred attributes in the atomic predicates.
	* @return As above.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 17, 2007
	* @version 1.0
	*/
	set<string> getReferredAttributes() const;

	/**
	* @brief Rename each attribute reference att in some non-complex atomic 
			predicate of this DNF predicate to schemaMap^{-1}(att)
	* @param schemaMap The input schema map to look up attributes.
	* @invariant The attribute references in this predicate must not be produced
			by some UDF spec in the input schema map.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 18, 2007
	* @version 1.0
	*/
	void renameAttrRefInPredsToAttrIn(
		const SchemaMapString* schemaMap);

	/**
	* @brief Merge the input predicate into this predicate as follows.
			If this predicate is empty, just copy the input predicate over.
			If this predicate is not empty but the input predicate is, return.
			Otherwise, both this predicate and the input predicate have to be		
			conjunctive to be merged.
	* @param inputPred The input predicate to merge
	* @invariant If both predicates are non-empty, they both have to 
				 be conjunctive.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 18, 2007
	* @version 1.0
	*/
	void mergePredicate(const Predicates_DNFString* inputPred);

private:
	/**
	* This variable stores the set of conjunctive predicate clauses in this DNF.
	*/
	ConjunctiveClauses _clauses;

}; //class Predicates_DNFString
}  //namespace AutomatonCompiler
}  //namespace Cayuga

#endif //_PREDICATES_DNF_STRING_H
