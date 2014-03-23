/**
 * @file NormalAtomPredString.h
 * @date April 22, 2006
 * @brief Base class of static and dynamic atomic predicates read from AIR.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _NORMAL_ATOM_PRED_STRING_H
#define _NORMAL_ATOM_PRED_STRING_H

#include "AtomPredString.h"

#include <string>
#include <iostream>
using namespace std;

namespace Cayuga {
namespace AutomatonCompiler {
class ConjunctiveClauseString;
class Context;

/**
 * @brief A NormalAtomPredString object represents a normal atomic predicate 
		  that is not a duration predicate.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 30, 2005
 * @version 1.0
 */
class NormalAtomPredString : public AtomPredString {
public:
	/**
	* @brief Default constructor
	* @param attr The left hand side attribute of the event
			 read from AIR.
	* @param op Comparison operator of this atomic predicate
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 30, 2005
	* @version 1.0
	*/
	NormalAtomPredString(const string& attr, const string& op);

	/**
	* @brief Virtual destructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 30, 2005
	* @version 1.0
	*/
	virtual ~NormalAtomPredString() = 0;

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
	virtual void write(ostream& out = cout);

	/**
	* @brief Return the left hand side event attribute read from AIR.
	* @return As above.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 30, 2005
	* @version 1.0
	*/
	const string getEventAttr() const;

	/**
	* @brief Return the comparison operator of this predicate read from AIR.
	* @return As above.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date December 13, 2005
	* @version 1.0
	*/
	const string getStringOp() const;

	virtual bool canBeIndexed();

	/**
	* @brief Return a cloned copy of the predicate string object
	* @return A cloned predicate object identical to the original one
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 1, 2007
	* @version 1.0
	*/
	virtual AtomPredString* clone() = 0;

protected:
	/**
	* @brief Allocate memory for the byte code program of this atomic predicate.
	* @return Pointer to the allocated program.
	* @see ByteCode
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 30, 2005
	* @version 1.0
	*/
	char* allocatePredInstruction(Context& context);

	/** 
	* This variable stores the left hand side event attribute ID of this 
	* atomic predicate 
	*/
	string _eventAttr; 
}; //class NormalAtomPredString
}  //namespace AutomatonCompiler
}  //namespace Cayuga

#endif //_NORMAL_ATOM_PRED_STRING_H
