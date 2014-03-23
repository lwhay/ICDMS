/**
 * @file DurationPredString.h
 * @date April 22, 2006
 * @brief class of duration atomic predicates read from AIR.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _DURATION_PRED_STRING_H
#define _DURATION_PRED_STRING_H

#include "AtomPredString.h"

#include <string>
#include <iostream>
using namespace std;

namespace Cayuga {
namespace BasicDataStructure {
namespace Predicate {
struct DurationStruct;
}
}
using namespace Cayuga::BasicDataStructure::Predicate;

namespace AutomatonCompiler {
class ConjunctiveClauseString;

using Cayuga::BasicDataStructure::StreamID;
class Context;

/**
 * @brief A DurationPredString object represents a duration atomic predicate 
		  constructed from SAX events.	
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 30, 2005
 * @version 1.0
 */
class DurationPredString : public AtomPredString {
	friend class ConjunctiveClauseString;
public:
	/**
	* @brief Default constructor
	* @param type The type of this duration predicate, which could be EVENT
			 or COMPOSITE. The former is evaluated on the incoming event, while
			 the latter on the composition of the event with automaton instance.
	* @param op Comparison operator of this atomic predicate
	* @param rhs The right hand side constant of the duration constraint
			 read from AIR.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 30, 2005
	* @version 1.0
	*/
	DurationPredString(const string& type,
		const string& op, 
		const string& rhs);

	/**
	* @brief Virtual destructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 30, 2005
	* @version 1.0
	*/
	virtual ~DurationPredString();
	
	/**
	* @brief Compiles this predicate into a byte code program.
	* @param context The compilation context
	* @return Pointer to the compiled program.
	* @see ByteCode
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 30, 2005
	* @version 1.0
	*/
	virtual char* compile(Context& context);

	DurationStruct* createDurationStruct();

	/**
	* @brief Output member variable information in XML format, aiming at 
			 reconstructing the XML piece in AIR which encodes this predicate
	* @param out The output stream handle.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 30, 2005
	* @version 1.0
	*/
	virtual void write(ostream& out = cout);

	/**
	* @brief Return a cloned copy of the predicate string object
	* @return A cloned predicate object identical to the original one
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 1, 2007
	* @version 1.0
	*/
	virtual AtomPredString* clone();
	
	virtual void getReferredAttributes(
		set<string>& referredAttrs) const;
	
	virtual AtomPredString* renameAttrRefInPredsToAttrIn(
		const SchemaMapString* schemaMap);
private:
	/**
	* This variable stores the type of the duration 
	* constraint read from AIR.
	*/
	string _type;

	/**
	* This variable stores the right hand side constant of the duration 
	* constraint read from AIR.
	*/
	string _rhs;
}; //class DurationPredString
}  //namespace AutomatonCompiler
}  //namespace Cayuga

#endif //_DURATION_PRED_STRING_H
