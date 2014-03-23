/**
 * @file ConjunctiveClauseString.h
 * @date November 17, 2005
 * @brief Stores the information of a conjunctive clause of atomic predicates 
          read from AIR.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _CONJUNCTIVE_CLAUSE_STRING_H
#define _CONJUNCTIVE_CLAUSE_STRING_H

#include <iostream>
#include <set>
#include <vector>
#include <typeinfo>

using namespace std;

namespace Cayuga {
namespace BasicDataStructure {
namespace Predicate {
class ConjunctiveClause;
}
}

namespace AutomatonCompiler {
class AtomPredString;
class SchemaMapString;

class Context;
using Cayuga::BasicDataStructure::Predicate::ConjunctiveClause;

/**
 * @brief A ConjunctiveClauseString object represents a conjunctive predicate 
		  clause constructed from SAX events.	
 * @see	Predicates_DNFString
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 17, 2005
 * @version 1.0
 */
class ConjunctiveClauseString {
public:
	typedef vector<AtomPredString*> AtomPredSet;

	/**
	* @brief Compiles this predicate clause into Cayuga engine.
	* @param context The compilation context
	* @return Pointer to the internalized conjunctive predicate clause.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 17, 2005
	* @version 1.0
	*/
	ConjunctiveClause* compile(Context& context);

	/**
	* @brief Output member variable information in XML format, aiming at 
			 reconstructing the XML piece in AIR which encodes this clause
	* @param out The output stream handle.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 17, 2005
	* @version 1.0
	*/
	void write(ostream& out = cout);

	/**
	* @brief Add an atomic predicate to this clause
	* @param p Pointer to the atomic predicate to add
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 17, 2005
	* @version 1.0
	*/
	void addAtomPred(AtomPredString* p);

	/**
	* @brief Copy the atomic predicates stored in this object into the input
	* variable
	* @param atomPreds The object into which the atomic predicates which be 
	copied
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 1, 2007
	* @version 1.0
	*/
	void copyAtomPredsTo(AtomPredSet& atomPreds);

	/**
	* @brief Return true iff this predicate has a complex atomic predicate.
	* @return As above.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 17, 2007
	* @version 1.0
	*/
	bool hasComplexPredicate() const;

	/**
	* @brief Put into referredAttrs the set of referred attributes in the 
			 atomic predicates.
	* @param referredAttrs The collection which stores the referred attrs.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 17, 2007
	* @version 1.0
	*/
	void getReferredAttributes(
		set<string>& referredAttrs) const;
	
	/**
	* @brief Rename each attribute reference att in some non-complex atomic 
			predicate of this clause to schemaMap^{-1}(att)
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
	* @brief Merge the input clause into this clause to form a bigger conjunct.
	* @param inputClause The input predicate to merge
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 18, 2007
	* @version 1.0
	*/
	void mergePredicate(const ConjunctiveClauseString* inputClause);

private:
	/**
	* This variable stores the set of atomic predicates in this clause.
	*/
	AtomPredSet _atomPreds;

}; //class ConjunctiveClauseString
}  //namespace AutomatonCompiler
}  //namespace Cayuga

#endif //_CONJUNCTIVE_CLAUSE_STRING_H
