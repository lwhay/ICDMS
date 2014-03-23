/**
 * @file Edge.cpp
 * @date November 27, 2005
 * @brief Implementation file of Edge class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "BasicDataStructure/Automaton/Edge.h"
using namespace Cayuga::BasicDataStructure::Automaton;

#include "BasicDataStructure/Predicate/Predicates_DNF.h"

#include <cassert>
using namespace std;

//unsigned Edge::_edgeIDCnt = 0;

Edge::Edge(EdgeID thisEdgeID) : _edgeID(thisEdgeID), _pDNF(NULL) {
}

Edge::~Edge() {
	delete _pDNF;
}


void Edge::setDNF(Predicates_DNF* pDNF) {
	_pDNF = pDNF;
}

Predicates_DNF* Edge::getDNF() const {
	return _pDNF;
}

size_t Edge::getNumClauses() const {
	if (_pDNF == NULL) {
		return 0;
	}
	else {
		return _pDNF->getNumClauses();
	}
}

const type_info& Edge::getEdgeType() const {
	return typeid(*this);
}

void Edge::write(ostream& out) const {
	if (_pDNF) {
		_pDNF->write(out);
	}
}

bool isSame(Predicates_DNF* lhs, Predicates_DNF* rhs) {
	if (lhs == NULL || rhs == NULL) {
		if (lhs != rhs)
			return false;
	}
	else if (*lhs != *rhs) {
		return false;
	}
	return true;
}

bool Edge::operator == (const Edge& rhs) const {
	if (typeid(*this) != typeid(rhs)) {
		return false;
	}

	if (!isSame(_pDNF, rhs._pDNF)) {
		return false;
	}

	return true;
}

bool Edge::operator != (const Edge& rhs) const {
	return !(Edge::operator ==(rhs));
}

EdgeID Edge::getID() {
	assert (_edgeID != INVALID_EDGE_ID);
	return _edgeID;
}

void Edge::setID(EdgeID thisEdgeID) {
	assert (thisEdgeID != INVALID_EDGE_ID);
	_edgeID = thisEdgeID;
}
