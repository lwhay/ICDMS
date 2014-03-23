/**
 * @file StaAtomPredString.h
 * @date November 30, 2005
 * @brief Class of static atomic predicates read from AIR.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _STA_ATOM_PRED_STRING_H
#define _STA_ATOM_PRED_STRING_H

#include "NormalAtomPredString.h"

#include <string>
#include <iostream>
using namespace std;

namespace Cayuga {
namespace AutomatonCompiler {
class ConjunctiveClauseString;
class Context;

/**
 * @brief A StaAtomPredString object represents a static atomic predicate 
		  read from AIR.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 30, 2005
 * @version 1.0
 */
class StaAtomPredString : public NormalAtomPredString {
public:
	/**
	* @brief default constructor
	* @param attr The left hand side attribute of the event
			 read from AIR.
	* @param op Comparison operator of this atomic predicate
	* @param nConst The right hand side constant of the static atomic predicate
	*		 read from AIR.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 30, 2005
	* @version 1.0
	*/
	StaAtomPredString(const string& attr, 
		const string& op, 
		const string& nConst);

	/**
	* @brief Virtual destructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 30, 2005
	* @version 1.0
	*/
	virtual ~StaAtomPredString();

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
	* @brief Return the right hand side constant of the static atomic predicate
	*		 read from AIR.
	* @return As above.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 30, 2005
	* @version 1.0
	*/
	string getConst();

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
protected:
	/**
	* This variable stores the right hand side constant of the static atomic 
	* predicate read from AIR.
	*/
	string _rhs;

}; //class StaAtomPredString
}  //namespace AutomatonCompiler
}  //namespace Cayuga

#endif //_STA_ATOM_PRED_STRING_H
