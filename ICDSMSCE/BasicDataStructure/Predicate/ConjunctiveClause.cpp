/**
 * @file ConjunctiveClause.cpp
 * @date November 30, 2005
 * @brief Implementation file of ConjunctiveClause class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "AutomatonCompiler/AtomPredString.h"
using Cayuga::AutomatonCompiler::AtomPredString;

#include "BasicDataStructure/Predicate/ConjunctiveClause.h"
#include "BasicDataStructure/Predicate/DurationStruct.h"
using namespace Cayuga::BasicDataStructure::Predicate;

#include "BasicDataStructure/Automaton/Instance.h"
using namespace Cayuga::BasicDataStructure::Automaton;

#include "BasicDataStructure/Event/Event.h"
using namespace Cayuga::BasicDataStructure::EventNS;

#include "QueryProcessor/Interpreter.h"
using namespace Cayuga::QueryProcessor;

#include "SAXHandler/ConfigMgr.h"
using namespace Cayuga::SAXHandler;

#include "SystemMgr/TraceMgr.h"
#include "SystemMgr/Options.h"
using namespace Cayuga::SystemMgr;

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;


namespace Cayuga {
namespace AutomatonCompiler {
void overwriteArrayFieldInEVALInstruction(char* program, 
										const char* ev, 
										const char* srcInstanceArray);
} //namespace AutomatonCompiler
} //namespace Cayuga
using Cayuga::AutomatonCompiler::overwriteArrayFieldInEVALInstruction;


ConjunctiveClause::ConjunctiveClause() : 
	_atomStaPredsProgram(NULL), _atomDynPredsProgram(NULL), 
	_pEventDur(NULL), _pCompositeDur(NULL) {
}

ConjunctiveClause::~ConjunctiveClause() {
	delete _atomStaPredsProgram;
	delete _atomDynPredsProgram;
	delete _pEventDur;
	delete _pCompositeDur;
}

void ConjunctiveClause::setStaAtomPredProgram(char* p, size_t sz) {
	_atomStaPredsProgram = p;
	_szStaPredProgram = sz;
}

void ConjunctiveClause::setDynAtomPredProgram(char* p, size_t sz) {
	_atomDynPredsProgram = p;
	_szDynPredProgram = sz;
}

void ConjunctiveClause::addIndexedStaAtomPred(const char* p, size_t sz) {
	_indexedStaAtomPreds.push_back(make_pair(p, sz));
}

void ConjunctiveClause::addIndexedDynAtomPred(const char* p, size_t sz) {
	_indexedStaAtomPreds.push_back(make_pair(p, sz));
}

bool ConjunctiveClause::evalStatic(
	const Cayuga::BasicDataStructure::EventNS::Event* ev) {
	if (_pEventDur) {
		TimeStamp diff = ev->getEndTime() - ev->getStartTime();
		bool dummy; //not used for event duration constraint
		bool ret = _pEventDur->eval(diff, dummy);
		if (ret == false) {
			return false;
		}
	}

	const char* evArray = ev->getBody();
	//const char* srcInstanceArray = pInst ? pInst->getBody() : NULL;
	const char* srcInstanceArray = NULL;

	Interpreter* pInterpreter = Interpreter::getInstance();

	overwriteArrayFieldInEVALInstruction(_atomStaPredsProgram, 
		evArray, 
		srcInstanceArray);
	return pInterpreter->exec(_atomStaPredsProgram);
}

bool ConjunctiveClause::evalDynamic(
	const Cayuga::BasicDataStructure::EventNS::Event* ev, 
	Cayuga::BasicDataStructure::Automaton::Instance* pInst) {
	if (_pCompositeDur) {
		//pInst could be NULL if we are evaluating the forwad edge of start node.
		//However, in this case that forward edge would not have any such 
		//composite duration predicate.
		assert (pInst);
		TimeStamp diff = ev->getEndTime() - pInst->getStartTime();
		bool bDeleteInst;
		bool ret = _pCompositeDur->eval(diff, bDeleteInst);
		if (ret == false) {
			if (bDeleteInst) {
				pInst->setTouched();
				if (ConfigMgr::getInstance()->getOptions()->_bRecordTrace) {
					//This instance trace message is to mark the deletion of
					//this instance.
					TraceMgr::getInstance()->generateInstanceMsg(
						BAD_NODE_ID, 
							  //should pass the ID to the node storing pInst. 
							  //however it is not available in this context,
							  //and is not used by the Automaton Visualizer 
							  //currently. So passing BAD_NODE_ID here will not 
							  //produce an error.
						pInst->getID(), 
						NULL, 
						NULL,
						pInst->getStartTime(),
						ev->getEndTime());
				}
			}
			return false;
		}
	}

	const char* evArray = ev->getBody();
	const char* srcInstanceArray = pInst ? pInst->getBody() : NULL;

	Interpreter* pInterpreter = Interpreter::getInstance();

	overwriteArrayFieldInEVALInstruction(_atomDynPredsProgram, 
		evArray, 
		srcInstanceArray);
	return pInterpreter->exec(_atomDynPredsProgram);
}

//the logic is identical to ConjunctiveClauseString::write
void ConjunctiveClause::write(ostream& out) const {
	//for_each(_atomPreds.begin(),_atomPreds.end(), 
	//	bind2nd(mem_fun(&AtomPredString::write), out));
	for (AtomPredSet::const_iterator it = _atomPreds.begin();
		it != _atomPreds.end();
		++it) {
			(*it)->write(out);
	}
}

///@bug For now assume the order of indexed predicates on both sides are the 
///same
bool ConjunctiveClause::operator == (const ConjunctiveClause& rhs) const {
	if (memcmp(_atomStaPredsProgram, rhs._atomStaPredsProgram, _szStaPredProgram)) {
		return false;
	}

	if (_indexedStaAtomPreds.size() != rhs._indexedStaAtomPreds.size()) {
		return false;
	}

	if (memcmp(_atomDynPredsProgram, rhs._atomDynPredsProgram, _szDynPredProgram)) {
		return false;
	}

	if (_indexedDynAtomPreds.size() != rhs._indexedDynAtomPreds.size()) {
		return false;
	}

	for (size_t i = 0; i < _indexedStaAtomPreds.size(); ++i) {
		if (_indexedStaAtomPreds[i].second != rhs._indexedStaAtomPreds[i].second) {
			return false;
		}

		if (memcmp(_indexedStaAtomPreds[i].first, 
			rhs._indexedStaAtomPreds[i].first, 
			_indexedStaAtomPreds[i].second)) {
				return false;
		}
	}

	for (size_t i = 0; i < _indexedDynAtomPreds.size(); ++i) {
		if (_indexedDynAtomPreds[i].second != rhs._indexedDynAtomPreds[i].second) {
			return false;
		}

		if (memcmp(_indexedDynAtomPreds[i].first, 
			rhs._indexedDynAtomPreds[i].first, 
			_indexedDynAtomPreds[i].second)) {
				return false;
		}
	}

	//bug fix 07.10.24
	//need to check equivalence of duration constructs
	if (!checkDurationStruct(_pEventDur, rhs._pEventDur)) return false;

	if (!checkDurationStruct(_pCompositeDur, rhs._pCompositeDur)) return false;
	
	return true;
}

bool ConjunctiveClause::checkDurationStruct(DurationStruct* lhs,
											DurationStruct* rhs) const {
	if (lhs == NULL && rhs != NULL) return false;

	if (lhs != NULL && rhs == NULL) return false;

	if (lhs == NULL && rhs == NULL) return true;

	return (*lhs == *rhs);
}

bool ConjunctiveClause::operator != (const ConjunctiveClause& rhs) const {
	return !(*this == rhs);
}
