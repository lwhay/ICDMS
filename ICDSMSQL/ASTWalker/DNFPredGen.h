/**
 * @file DNFPredGen.h
 * @date April 11, 2007
 * @brief This file declares DNFPredGen class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#pragma once

#include "CayugaQLASTWalker.h"

namespace Cayuga {
namespace AutomatonCompiler {
class Predicates_DNFString;
class ConjunctiveClauseString;

class AtomPredString;
class ComplexAtomPredString;
}

namespace QLCompiler {
using namespace Cayuga::AutomatonCompiler;
/**
 * @brief A DNFPredGen object generates DNF predicates for each edge, 
		 and put them under uOps and binOp nodes.

		 uOps store predicates for sigma operator. binOps store predicates
		 for sequence predicate, and the two predicates for mu.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date April 11, 2007
 * @version 1.0
 */
class DNFPredGen : public CayugaQLASTWalker
{
public:
	//DNFPredGen(void);
	//virtual ~DNFPredGen(void);

	/**
	* @brief Visit the AST.
	* @param thisAst The root of the input AST.
	* @invariant thisAst must be a QUERY node.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 11, 2007
	* @version 1.0
	*/
	virtual void visit(RefCayugaQLAST& thisAst);

private:
	//predicate compilation
	///@invariant The input AST node mus be of type uOps or sexp
	void _compilePredicate(RefCayugaQLAST& uOpsOrStreamExprAST);
	
	Predicates_DNFString* _compileSigmaPred(RefCayugaQLAST& uOpsAST,
		const ASTSchema& schema);

	//compile the predicate from a uOps node
	void _compileUnaryOpsPredicate(RefCayugaQLAST& uOpsAST);

	//the two schema input paramters are not required to generate AIR 
	//predicates, but may be useful for type checking later
	//require: rhsSchema is always the event stream schema, and lhsSchema
	//is the node schema. for compilation sigma operator predicate,
	//lhsSchema will be NULL.
	Predicates_DNFString* _compileDNF(RefCayugaQLAST& predExprAST,
		const ASTSchema& lhsSchema,
		const ASTSchema& rhsSchema,
		AttributeLocation attrLoc);

	ConjunctiveClauseString* _compileCC(RefCayugaQLAST& andPredAST,
		const ASTSchema& lhsSchema,
		const ASTSchema& rhsSchema,
		AttributeLocation attrLoc);

	AtomPredString* _compileAtomPred(RefCayugaQLAST& atomPredAST,
		const ASTSchema& lhsSchema,
		const ASTSchema& rhsSchema,
		AttributeLocation attrLoc);

	ComplexAtomPredString* _compileComplexPredicate(RefCayugaQLAST& lhsTermAST, 
		RefCayugaQLAST& rhsTermAST,
		const string& compOp,
		const ASTSchema& lhsSchema, 
		const ASTSchema& rhsSchema,
		AttributeLocation attrLoc);

	//in the following three functions,
	//return NULL if the input atom pred does not qualify for a 
	//particular predicate type specified by the function.
	//Otherwise return the compiled predicate.
	AtomPredString* _qualifyForStaticPredicateAndCompile(
		RefCayugaQLAST& atomPredAST,
		const ASTSchema& lhsSchema,
		const ASTSchema& rhsSchema,
		AttributeLocation attrLoc);

	AtomPredString* _qualifyForDynamicPredicateAndCompile(
		RefCayugaQLAST& atomPredAST,
		const ASTSchema& lhsSchema,
		const ASTSchema& rhsSchema,
		AttributeLocation attrLoc);

	AtomPredString* _qualifyForDurationPredicateAndCompile(
		RefCayugaQLAST& atomPredAST,
		const ASTSchema& lhsSchema,
		const ASTSchema& rhsSchema,
		AttributeLocation attrLoc);

	//always succeed -- return non-NULL
	AtomPredString* _qualifyForComplexPredicateAndCompile (
		RefCayugaQLAST& atomPredAST,
		const ASTSchema& lhsSchema,
		const ASTSchema& rhsSchema,
		AttributeLocation attrLoc);

	void _getLHS_CompOp_RHS_FromAtomPred(
		const RefCayugaQLAST& atomPredAST,
		RefCayugaQLAST& lhsTermAST,
		RefCayugaQLAST& compOpAST,
		RefCayugaQLAST& rhsTermAST);

	bool _attrIsFromEventSchema(
		const RefCayugaQLAST& attrAST,
		AttributeLocation attrLoc);
	
	bool _attrIsFromNodeSchema(
		const RefCayugaQLAST& attrAST,
		AttributeLocation attrLoc);

	//reverse the input comparison operator. e.g. reverse of <= is >=
	void _reverse(string& compOp);
	
	//convert the input time contant to a normalized constant
	//in Cayuga time unt according to the input time unit
	string _convertDurationConst(
		const string& strConst,
		const string& timeUnit);

};//class DNFPredGen
} //namespace QLCompiler
} //namespace Cayuga
