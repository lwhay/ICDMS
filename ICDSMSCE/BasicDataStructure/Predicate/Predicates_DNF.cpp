/**
 * @file Predicates_DNF.cpp
 * @date November 30, 2005
 * @brief Implementation file of Predicates_DNF class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "BasicDataStructure/Predicate/Predicates_DNF.h"
using namespace Cayuga::BasicDataStructure::Predicate;

void Predicates_DNF::addClause(ConjunctiveClause* pClause) {
	_clauses.push_back(pClause);
}

bool Predicates_DNF::eval(const Cayuga::BasicDataStructure::EventNS::Event* ev, 
						  Cayuga::BasicDataStructure::Automaton::Instance* pInst) {
	for (vector<size_t>::iterator it = _swordClauses.begin();
		it != _swordClauses.end(); ++it) {
			bool ret = _clauses[*it]->evalStatic(ev) &&
						_clauses[*it]->evalDynamic(ev, pInst);
			if (ret) {
				//_swordClauses.clear();
				return true;
			}
	}
	//_swordClauses.clear();
	return false;
}

void Predicates_DNF::setSwordDNFClause(size_t clauseNo) {
	assert (clauseNo >= 0 && clauseNo < _clauses.size());
	_swordClauses.push_back(clauseNo);
}

void Predicates_DNF::clearSwordClauses() {
	_swordClauses.clear();
}


void Predicates_DNF::write(ostream& out) const {
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

size_t Predicates_DNF::getNumClauses() const {
	return _clauses.size();
}

bool Predicates_DNF::operator == (const Predicates_DNF& rhs) const {
	//SHORT CUT
	///@bug Assume the order of clauses in lhs and rhs are the same
	if (_clauses.size() != rhs._clauses.size()) {
		return false;
	}

	for (size_t i = 0; i < _clauses.size(); ++i) {
		if (*_clauses[i] != *rhs._clauses[i]) {
			return false;
		}
	}

	return true;
}

bool Predicates_DNF::operator != (const Predicates_DNF& rhs) const {
	return !(*this == rhs);
}
