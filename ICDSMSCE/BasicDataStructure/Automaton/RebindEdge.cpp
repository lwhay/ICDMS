/**
 * @file RebindEdge.cpp
 * @date November 27, 2005
 * @brief Implementation file of RebindEdge class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "BasicDataStructure/Automaton/RebindEdge.h"
#include "BasicDataStructure/Predicate/Predicates_DNF.h"
using namespace Cayuga::BasicDataStructure::Automaton;

////////////////////////////////////////////////////////////////////////////////
// Definition of utility functions
////////////////////////////////////////////////////////////////////////////////
namespace Cayuga {
namespace AutomatonCompiler {
bool isSameProgram(char* lhs, char* rhs);
}

using Cayuga::AutomatonCompiler::isSameProgram;
} //namespace Cayuga

////////////////////////////////////////////////////////////////////////////////
// RebindEdge member function definitions
////////////////////////////////////////////////////////////////////////////////

RebindEdge::RebindEdge(EdgeID thisEdgeID) 
	: FilterEdge(thisEdgeID), _schemaMapProgram(NULL), 
	_hasDel(false), _pNextSwordEdge(NULL) {
}

RebindEdge::~RebindEdge() {
	delete _schemaMapProgram;
}

void RebindEdge::setSchemaMapProgram(char* program) {
	_schemaMapProgram = program;
}

void RebindEdge::setNextSwordEdge(RebindEdge* pEdge) {
	if (pEdge != NULL) {
		pEdge->setNextSwordEdge(NULL);
	}
	_pNextSwordEdge = pEdge;
}

RebindEdge* RebindEdge::getNextSwordEdge() {
	return _pNextSwordEdge;
}


void RebindEdge::setSwordDNFClause(size_t clauseNo) {
	_pDNF->setSwordDNFClause(clauseNo);
}

void RebindEdge::clearSwordEdges() {
	if (_pDNF) {
		_pDNF->clearSwordClauses();
	}

	if (_pNextSwordEdge) {
		_pNextSwordEdge->clearSwordEdges();
		_pNextSwordEdge = NULL;

	}
}


void RebindEdge::write(ostream& out) const {
	out<<"<RebindEdge>"<<endl;
	Edge::write(out);
	_schemaMap.write(out);
	out<<"</RebindEdge>"<<endl;
}

bool RebindEdge::operator == (const Edge& rhs) const {
	if (Edge::operator !=(rhs)) {
		return false;
	}
	//Check for same Del Predicate
	if(_hasDel != ((RebindEdge*) &rhs)->_hasDel) {
		return false;
	}

	if (!isSameProgram(_schemaMapProgram, (
		(const RebindEdge*)&rhs)->_schemaMapProgram)) {
		return false;
	}

	return true;
}


bool RebindEdge::operator != (const Edge& rhs) const {
	return !(RebindEdge::operator ==(rhs));
}

void RebindEdge::setDel(const bool& hasDel) {
	_hasDel = hasDel;
}

bool RebindEdge::hasDel() const {
	return _hasDel;
}
void RebindEdge::setSchemaMap(const SchemaMapString& schemaMap) {
	_schemaMap = schemaMap;
}
