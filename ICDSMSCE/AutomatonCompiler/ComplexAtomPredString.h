/**
 * @file ComplexAtomPredString.h
 * @date April 15, 2006
 * @brief class of complex atomic predicates read from AIR.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _COMPLEX_ATOM_PRED_STRING_H
#define _COMPLEX_ATOM_PRED_STRING_H

#include "StaAtomPredString.h"

#include "BasicDataStructure/StreamID.h"
using Cayuga::BasicDataStructure::StreamID;

#include "AutomatonCompiler/PredExprEval/BinaryCompArithOpAST.h"
using namespace Cayuga::AutomatonCompiler::PredExprEval;

#include <string>
#include <iostream>
using namespace std;

namespace Cayuga {
namespace AutomatonCompiler {
class ConjunctiveClauseString;
class Context;

/**
 * @brief A ComplexAtomPredString object represents a complex atomic predicate 
		  read from AIR, where its LHS and RHS are general arithmetic 
		  expressions involving constants and attributes from events and nodes.
		  It subsumes simple static or dynamic predicates.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date April 15, 2006
 * @version 1.0
 */
class ComplexAtomPredString : public StaAtomPredString {
public:
	/**
	* @brief default constructor
	* @param expr The expression encoding the predicate read from AIR.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 15, 2006
	* @version 1.0
	*/
	ComplexAtomPredString(const string& expr);

	/**
	* @brief Virtual destructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 15, 2006
	* @version 1.0
	*/
	virtual ~ComplexAtomPredString();

	/**
	* @brief Compiles this predicate into a byte code program.
	* @param context The compilation context
	* @return Pointer to the compiled program.
	* @see ByteCode
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 15, 2006
	* @version 1.0
	*/
	virtual char* compile(Context& context);

	/**
	* @brief Output member variable information in XML format, aiming at 
			 reconstructing the XML piece in AIR which encodes this predicate
	* @param out The output stream handle.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 15, 2006
	* @version 1.0
	*/
	virtual void write(ostream& out = cout);

	virtual bool canBeIndexed();

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
	* @brief If the input expression contains < symbol, convert it to the XML
		frield &lt; instead.
	* @param expr The input expr to fix
	* @return The fixed expression
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 12, 2007
	* @version 1.0
	*/
	const string _fixXML(const string& expr) const;

	RefBinaryCompArithOpAST _buildAST(const string& expr) const;

	/**
	* This variable stores the expression encoding this atomic 
	* predicate read from AIR.
	*/
	string _expr;

	/**
	* This variable indicates whether this attribute can be indexed. It is
	* obtained from analyzing its syntactic form. It is true iff the pred is of
	* form attr = const.
	*/
	bool _bIndexedPred;
}; //class ComplexAtomPredString
}  //namespace AutomatonCompiler
}  //namespace Cayuga

#endif //_COMPLEX_ATOM_PRED_STRING_H
