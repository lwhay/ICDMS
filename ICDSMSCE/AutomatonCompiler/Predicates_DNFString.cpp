/**
 * @file Predicates_DNFString.cpp
 * @date November 27, 2005
 * @brief Implementation file of Predicates_DNFString class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "AutomatonCompiler/Predicates_DNFString.h"
#include "AutomatonCompiler/ConjunctiveClauseString.h"
#include "AutomatonCompiler/Context.h"
using namespace Cayuga::AutomatonCompiler;

#include "BasicDataStructure/Predicate/Predicates_DNF.h"

#include "Utils/RAssert.h"
using namespace Cayuga::Utils;

#include <iostream>
using namespace std;


void Predicates_DNFString::addClause(ConjunctiveClauseString* pClause) {
	_clauses.push_back(pClause);
}

bool Predicates_DNFString::isConjunctive() const {
	return (_clauses.size() <= 1);
}

bool Predicates_DNFString::isEmpty() const {
	return _clauses.empty();
}

set<string> Predicates_DNFString::
getReferredAttributes() const {
	set<string> referredAttrs;

	for (ConjunctiveClauses::const_iterator it = _clauses.begin();
		it != _clauses.end(); ++it) {
			ConjunctiveClauseString* pCurClause = *it;
			pCurClause->getReferredAttributes(
				referredAttrs);
	}

	return referredAttrs;
}

bool Predicates_DNFString::hasComplexPredicate() const {
	for (ConjunctiveClauses::const_iterator it = _clauses.begin();
		it != _clauses.end(); ++it) {
			ConjunctiveClauseString* pCurClause = *it;
			if (pCurClause->hasComplexPredicate()) return true;
	}

	return false;
}

void Predicates_DNFString::renameAttrRefInPredsToAttrIn(
	const SchemaMapString* schemaMap) {
	for (ConjunctiveClauses::iterator it = _clauses.begin();
		it != _clauses.end(); ++it) {
			ConjunctiveClauseString* pCurClause = *it;
			pCurClause->renameAttrRefInPredsToAttrIn(
				schemaMap);
	}
}

void Predicates_DNFString::mergePredicate(const Predicates_DNFString* inputPred) {
	if (_clauses.empty()) {
		_clauses = inputPred->_clauses;
		return;
	}

	if (inputPred->isEmpty())
		return;

	Assert (isConjunctive());
	Assert (inputPred->isConjunctive());
	_clauses[0]->mergePredicate(inputPred->_clauses[0]);	
}

void Predicates_DNFString::write(ostream& out) const {
	out<<"<EvalFunc>"<<endl;
	for (ConjunctiveClauses::const_iterator it = _clauses.begin();
		it != _clauses.end();
		++it) {
			out<<"<PredClause>"<<endl;
			(*it)->write(out);
			out<<"</PredClause>"<<endl;
	}
	out<<"</EvalFunc>"<<endl;
}

Predicates_DNF* Predicates_DNFString::compile(Context& context) {
	Predicates_DNF* pNewDNF = new Predicates_DNF();
	context._clauseNo = 0;
	for (ConjunctiveClauses::iterator it = _clauses.begin();
		it != _clauses.end();
		++it) {
			ConjunctiveClause* pInternalCC = (*it)->compile(context);
			pNewDNF->addClause(pInternalCC);
			context._clauseNo++;
	}
	return pNewDNF;
}
