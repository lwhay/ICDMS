/**
 * @file AtomPredString.h
 * @date November 30, 2005
 * @brief Base class of all possible types of atomic predicates read from AIR.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _ATOM_PRED_STRING_H
#define _ATOM_PRED_STRING_H

#include "BasicDataStructure/ByteCode.h"

#include "BasicDataStructure/StreamID.h"

#include <set>
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
using namespace Cayuga::BasicDataStructure;

namespace AutomatonCompiler {
class ConjunctiveClauseString;
class SchemaMapString;

using Cayuga::BasicDataStructure::StreamID;
class Context;

/**
 * @brief An AtomPredString object represents an atomic predicate constructed 
		  from SAX events.	
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 30, 2005
 * @version 1.0
 */
class AtomPredString {
public:
	/**
	* @brief Default constructor
	* @param op Comparison operator of this atomic predicate
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 30, 2005
	* @version 1.0
	*/
	AtomPredString(const string& op);

	/**
	* @brief Virtual destructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 30, 2005
	* @version 1.0
	*/
	virtual ~AtomPredString() = 0;

	/**
	* @brief Compiles this predicate into a byte code program.
	* @param context The compilation context
	* @return Pointer to the compiled program.
	* @see ByteCode
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 30, 2005
	* @version 1.0
	*/
	virtual char* compile(Context& context) = 0;

	/**
	* @brief Output member variable information in XML format, aiming at 
			 reconstructing the XML piece in AIR which encodes this predicate
	* @param out The output stream handle.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 30, 2005
	* @version 1.0
	*/
	virtual void write(ostream& out = cout) = 0;

	/**
	* @brief Return a cloned copy of the predicate string object
	* @return A cloned predicate object identical to the original one
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 1, 2007
	* @version 1.0
	*/
	virtual AtomPredString* clone() = 0;

	/**
	* @brief Put into referredAttrs the set of referred attributes in this 
			 atomic predicate.
	* @param referredAttrs The collection which stores the referred attrs.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 17, 2007
	* @version 1.0
	*/
	virtual void getReferredAttributes(
		set<string>& referredAttrs) const = 0;

	/**
	* @brief Rename each attribute reference att in this predicate 
			 to schemaMap^{-1}(att) if it is not  a complex atomic predicate 
	* @param schemaMap The input schema map to look up attributes.
	* @return If a new atomic predicate has to be created due to this renaming,
			  will return its pointer. Otherwise return NULL.
	* @invariant The attribute references in this predicate must not be produced
			by some UDF spec in the input schema map.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 18, 2007
	* @version 1.0
	*/
	virtual AtomPredString* renameAttrRefInPredsToAttrIn(
		const SchemaMapString* schemaMap) = 0;

protected:
	/**
	* @brief Return the comparison operator of this predicate.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 22, 2006
	* @version 1.0
	*/
	ByteCodeNS::ByteCode getOp();

	/**
	* This variable stores the comparison operator of this atomic predicate
	* read from AIR.
	*/
	string _op;
private:
	/**
	* @brief If the input operator contains < symbol, convert it to the XML
		frield &lt; instead.
	* @param op The input operator to fix
	* @return The fixed operator
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 12, 2007
	* @version 1.0
	*/
	const string _fixXML(const string& op);

}; //class AtomPredString
}  //namespace AutomatonCompiler
}  //namespace Cayuga

#endif //_ATOM_PRED_STRING_H
