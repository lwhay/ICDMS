/**
 * @file EdgeString.cpp
 * @date November 27, 2005
 * @brief Implementation file of EdgeString class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "AutomatonCompiler/EdgeString.h"

#include "AutomatonCompiler/Context.h"
#include "AutomatonCompiler/Predicates_DNFString.h"

#include "BasicDataStructure/Automaton/Edge.h"

using namespace Cayuga::AutomatonCompiler;

////////////////////////////////////////////////////////////////////////////////
// Foward declaration
////////////////////////////////////////////////////////////////////////////////

namespace Cayuga {
namespace QueryProcessor {
void insertIndexedPred(const Context& context, 
					   const PredIndexSpecString* pSpec, 
					   const vector<NormalAtomPredString*>& indexedPreds);
extern unsigned int NO_DNF;
} //QueryProcessor
} //Cayuga

using namespace Cayuga::QueryProcessor;

////////////////////////////////////////////////////////////////////////////////
// EdgeString member function definitions
////////////////////////////////////////////////////////////////////////////////

EdgeString::EdgeString() : _pDNF(NULL) {
}

EdgeString::~EdgeString() {
	delete _pDNF;
}

void EdgeString::setDNF(Predicates_DNFString* pDNF) {
	_pDNF = pDNF;
}


Edge* EdgeString::compile(Context& context) {
	Edge* pNewEdge = makeEdge(context._edgeType, context._nextForwardEdgeID);

	context._pCurrentEdge = pNewEdge;

	Predicates_DNF* pInternalDNF = (_pDNF 
		? _pDNF->compile(context) : NULL);

	if (_pDNF == NULL || _pDNF->isEmpty()) {
		//insert into a special "index" -- a sequential cluster 
		vector<NormalAtomPredString*> indexedPreds;
		//set to inform FREvaluator that this edge does
		//not have a DNF -- it is TRUE.
		context._clauseNo = NO_DNF; 
		insertIndexedPred(context, NULL, indexedPreds);
		pNewEdge->setDNF(NULL);
	}
	else { 
		pNewEdge->setDNF(pInternalDNF);
	}
	return pNewEdge;
}

void EdgeString::write(ostream& out) const {
	if (_pDNF) {
		_pDNF->write(out);
	}
}
