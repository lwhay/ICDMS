/**
 * @file ConjunctiveClauseString.cpp
 * @date November 27, 2005
 * @brief Implementation file of ConjunctiveClauseString class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "AutomatonCompiler/ConjunctiveClauseString.h"
#include "AutomatonCompiler/StaAtomPredString.h"
#include "AutomatonCompiler/DynAtomPredString.h"
#include "AutomatonCompiler/DurationPredString.h"
#include "AutomatonCompiler/ComplexAtomPredString.h"
#include "AutomatonCompiler/Context.h"
#include "AutomatonCompiler/ByteCodeUtils.h"
using namespace Cayuga::AutomatonCompiler;

#include "BasicDataStructure/Predicate/ConjunctiveClause.h"
using namespace Cayuga::BasicDataStructure::Automaton;

#include "BasicDataStructure/ByteCode.h"
using namespace Cayuga::BasicDataStructure::ByteCodeNS;

#include "BasicDataStructure/Automaton/IntermediateNode.h"
using namespace Cayuga::BasicDataStructure::Automaton;



////////////////////////////////////////////////////////////////////////////////
// Foward declaration
////////////////////////////////////////////////////////////////////////////////

namespace Cayuga {
namespace QueryProcessor {
void insertIndexedPred(const Context& context, 
					   const PredIndexSpecString* pSpec, 
					   const vector<NormalAtomPredString*>& indexedPreds);
}
}
using namespace Cayuga::QueryProcessor;

////////////////////////////////////////////////////////////////////////////////
// ConjunctiveClauseString member function definitions
////////////////////////////////////////////////////////////////////////////////

void ConjunctiveClauseString::addAtomPred(AtomPredString* p) {
	_atomPreds.push_back(p);
}

void ConjunctiveClauseString::write(ostream& out) {
	for (AtomPredSet::iterator it = _atomPreds.begin();
		it != _atomPreds.end();
		++it) {
			(*it)->write(out);
	}
}


ConjunctiveClause* ConjunctiveClauseString::compile(Context& context) {
	ConjunctiveClause* pNewCC = new ConjunctiveClause();

	///@todo: deal with the case of duration predicate
	vector<NormalAtomPredString*> IndexedDynPreds;
	vector<NormalAtomPredString*> IndexedStaPreds;

	//each entry is a pair of pred instruction and its length
	vector<pair<char*, size_t> > unIndexedStaAtomPredInstructions;
	vector<pair<char*, size_t> > unIndexedDynAtomPredInstructions;

	size_t numStaPredInstructionBytes = 0;
	size_t numDynPredInstructionBytes = 0;
	for (AtomPredSet::iterator it = _atomPreds.begin();
		it != _atomPreds.end();
		++it) {
		AtomPredString* pAtomPred = *it;
		if (typeid(*pAtomPred) == typeid(DurationPredString)) {
			//duration pred is treated separately -- it is not compiled into
			//a byte program, but is put into the member fields of pNewCC 
			//instead
			DurationPredString* pPred 
				= dynamic_cast<DurationPredString*>(pAtomPred);
			DurationStruct* pDurStruct = pPred->createDurationStruct();
			if (pPred->_type == "EVENT") {
				pNewCC->_pEventDur = pDurStruct;
			}
			else {
				pNewCC->_pCompositeDur = pDurStruct;
			}
		}
		else {
			char* predInstruction = pAtomPred->compile(context);

			NormalAtomPredString* pPred 
				= dynamic_cast<NormalAtomPredString*>(pAtomPred);
			assert (pPred);
			
			if(typeid(*pPred) == typeid(StaAtomPredString))
			{
				//search pPred->_eventAttr in context.predIndices			
				bool bPredIndexed = false;
				for (vector<PredIndexSpecString>::const_iterator spec_it 
					= context._predIndices.begin();
					spec_it != context._predIndices.end(); ++spec_it) {
						if (spec_it->find(pPred->getEventAttr()) 
							&& pPred->canBeIndexed()) {
							bPredIndexed = true;
							break;
						}
				}	
	
				if (bPredIndexed) {
					//only hash index for equality predicate
					assert (pPred->getStringOp() == "=");
					IndexedStaPreds.push_back(pPred);
					pNewCC->addIndexedStaAtomPred(predInstruction, 
					context._predInstructionLength);
				}	
				else {
					unIndexedStaAtomPredInstructions.push_back(
									make_pair(predInstruction, 
									context._predInstructionLength));
					numStaPredInstructionBytes += 
										context._predInstructionLength;
				}
			}
			else {
				bool pPredIndexed = false;								
				if (typeid(*(context._pSourceNode)) != 
					typeid(IntermediateNode)) {
						//now complex predicate does not have to involve
						//node attributes. It can be a UDF on event attributes.
						//Just to be safe: we treate this predicate as
						//dynamic predicate, even though it could be static
						//one, which depends only one event attributes.
						unIndexedDynAtomPredInstructions.push_back(
									make_pair(predInstruction,
									context._predInstructionLength));
						numDynPredInstructionBytes +=
										context._predInstructionLength;
						continue;
				}
				if(context._edgeType == FILTER && 
				  context._isDynIndexed && 
				  ((IntermediateNode*)context._pSourceNode)->
							noDynIndexedClause())
				{
					if(context._dynFilterPredIndex.find(pPred->getEventAttr(),
								((DynAtomPredString*)pPred)->getNodeAttr())) {
						assert(pPred->getStringOp() == "=");						
						IndexedDynPreds.push_back(pPred);
						pNewCC->addIndexedDynAtomPred(predInstruction,
							context._predInstructionLength);\
						((IntermediateNode*)context._pSourceNode)->
									setDynIndexedClause(context._clauseNo);
						pPredIndexed = true;
					}				
				}
				if(!pPredIndexed)
				{
					unIndexedDynAtomPredInstructions.push_back(
									make_pair(predInstruction,
									context._predInstructionLength));
					numDynPredInstructionBytes +=
										context._predInstructionLength;
				}
			}
		}
	}

	//This Part sets the program for the unindexed Static Component
	char* staPredProgram = new char[g_sizeEVALInstruction+numStaPredInstructionBytes];
	size_t ip = 0;

	setValToProgram<ByteCode>(staPredProgram, ip, EVAL);
	
	setValToProgram<size_t>(staPredProgram, 
		ip,  
		unIndexedStaAtomPredInstructions.size());
	
	overwriteArrayFieldInEVALInstruction(staPredProgram, NULL, NULL);
	ip += 2*sizeof(char*);

	for (size_t i = 0; i < unIndexedStaAtomPredInstructions.size(); i++) {
		memcpy(staPredProgram+ip, 
			unIndexedStaAtomPredInstructions[i].first, 
			unIndexedStaAtomPredInstructions[i].second);

		ip += unIndexedStaAtomPredInstructions[i].second;
		delete[] unIndexedStaAtomPredInstructions[i].first;
	}
	pNewCC->setStaAtomPredProgram(staPredProgram, 
		g_sizeEVALInstruction+numStaPredInstructionBytes);

	//This Part sets the program for the unindexed Dynamic Component
	char* dynPredProgram = new char[g_sizeEVALInstruction+numDynPredInstructionBytes];
	ip = 0;

	setValToProgram<ByteCode>(dynPredProgram, ip, EVAL);
	
	setValToProgram<size_t>(dynPredProgram, 
		ip,  
		unIndexedDynAtomPredInstructions.size());
	
	overwriteArrayFieldInEVALInstruction(dynPredProgram, NULL, NULL);
	ip += 2*sizeof(char*);

	for (size_t i = 0; i < unIndexedDynAtomPredInstructions.size(); i++) {
		memcpy(dynPredProgram+ip, 
			unIndexedDynAtomPredInstructions[i].first, 
			unIndexedDynAtomPredInstructions[i].second);

		ip += unIndexedDynAtomPredInstructions[i].second;
		delete[] unIndexedDynAtomPredInstructions[i].first;
	}
	pNewCC->setDynAtomPredProgram(dynPredProgram, 
		g_sizeEVALInstruction+numDynPredInstructionBytes);

	//find the corresponding index spec to indexed preds
	if (!IndexedStaPreds.empty()) {
		//cout<<"SOME STATIC PREDS\n";
		const PredIndexSpecString* pFoundSpec = NULL;
		for (vector<PredIndexSpecString>::const_iterator spec_it 
			= context._predIndices.begin();
			spec_it != context._predIndices.end(); ++spec_it) {
				if (spec_it->specMatchesPred(IndexedStaPreds)) {
					pFoundSpec = &(*spec_it);
					break;
				}
		}
		assert (pFoundSpec != NULL);

		//insert filter or FR indexed pred
		insertIndexedPred(context, pFoundSpec, IndexedStaPreds);
	}
	else {
		//insert into a special "index" -- a sequential cluster 
		insertIndexedPred(context, NULL, IndexedStaPreds);
	}

	//copy the atomic predicates from this object into pNewCC,
	//so that pNewCC->write() can generate correct AIR code
	copyAtomPredsTo(pNewCC->_atomPreds);

	return pNewCC;
}

void ConjunctiveClauseString::copyAtomPredsTo(AtomPredSet& atomPreds) {
	for (AtomPredSet::iterator it = _atomPreds.begin();
		it != _atomPreds.end();
		++it) {
			AtomPredString* pAtomPred = *it;
			atomPreds.push_back(pAtomPred->clone());
	}
}

bool ConjunctiveClauseString::hasComplexPredicate() const {
	//return true;
	for (AtomPredSet::const_iterator it = _atomPreds.begin();
		it != _atomPreds.end(); ++it) {
			AtomPredString* pAtomPred = *it;
			if (typeid(*pAtomPred) == typeid(ComplexAtomPredString)) 
				return true;
	}

	return false;
}

void ConjunctiveClauseString::getReferredAttributes(
	set<string>& referredAttrs) const {
	for (AtomPredSet::const_iterator it = _atomPreds.begin();
		it != _atomPreds.end(); ++it) {
			AtomPredString* pAtomPred = *it;
			pAtomPred->getReferredAttributes(
				referredAttrs);
	}
}

void ConjunctiveClauseString::renameAttrRefInPredsToAttrIn(
	const SchemaMapString* schemaMap) {
	for (AtomPredSet::iterator it = _atomPreds.begin();
		it != _atomPreds.end(); ++it) {
			AtomPredString* pAtomPred = *it;
			AtomPredString* pNewAtomPred 
				= pAtomPred->renameAttrRefInPredsToAttrIn(
				schemaMap);
			if (pNewAtomPred) {
				delete pAtomPred;
				*it = pNewAtomPred;
			}
	}
}

void ConjunctiveClauseString::mergePredicate(
	const ConjunctiveClauseString* inputClause) {
	copy(inputClause->_atomPreds.begin(), inputClause->_atomPreds.end(),
		back_inserter(_atomPreds));
}
