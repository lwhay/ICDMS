/**
 * @file FREvaluator.cpp
 * @date November 26, 2005
 * @brief Implementation file of FREvaluator class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "QueryProcessor/FREvaluator.h"
#include "QueryProcessor/Interpreter.h"
#include "QueryProcessor/QueryProcessorImpl.h"
using namespace Cayuga::QueryProcessor;

#include "AutomatonCompiler/ByteCodeUtils.h"
#include "AutomatonCompiler/convertConstToByteArray.h"
#include "AutomatonCompiler/Context.h"
#include "AutomatonCompiler/AtomPredString.h"
#include "AutomatonCompiler/StaAtomPredString.h"
using namespace Cayuga::AutomatonCompiler;

#include "BasicDataStructure/Automaton/Instance.h"
#include "BasicDataStructure/Automaton/IntermediateNode.h"
#include "BasicDataStructure/Automaton/ForwardEdge.h"
#include "BasicDataStructure/Predicate/Predicates_DNF.h"
#include "BasicDataStructure/Event/Event.h"
using namespace Cayuga::BasicDataStructure::Automaton;
using Cayuga::BasicDataStructure::OutputStreamType;

#include "CayugaSystem.h"
using Cayuga::CayugaSystem;

#include "Index/HashIndex.h"

#include "SystemMgr/DataTypeMgr.h"
#include "SystemMgr/StreamSchemaMgr.h"
#include "SystemMgr/TraceMgr.h"
#include "SystemMgr/Options.h"
using namespace Cayuga::SystemMgr;

#include "SAXHandler/ConfigMgr.h"
using namespace Cayuga::SAXHandler;

#undef max
#undef min
#include <limits>
#include <hash_set>
using namespace std;

namespace Cayuga {
	namespace QueryProcessor {
		size_t NO_DNF = numeric_limits<size_t>::max();
	}; //QueryProcessor
}; //Cayuga


////////////////////////////////////////////////////////////////////////////////
// FREvaluator member function definitions
////////////////////////////////////////////////////////////////////////////////

FREvaluator::FREvaluator() : _qpImpl(NULL) {
	PredIndexSpecString indexSpec;
	size_t indexInputLengthOut;
	Context context;
	char* indexInputPackProgram = indexSpec.compile(context,
		indexInputLengthOut);

	//indexSpec.write();

	assert (indexInputLengthOut == 0);
	pDefaultHashIndex = new HashIndexType(indexInputPackProgram, 0, 
		NUM_BUCKETS, true);
	_frIndices.push_back(pDefaultHashIndex);
}

FREvaluator::~FREvaluator() {
	for (vector<HashIndexType*>::iterator it = _frIndices.begin();
		it != _frIndices.end(); ++it) {
			delete *it;
	}
	delete pDefaultHashIndex;

	for (HashIndexMap::iterator it = _predIDMap.begin();
		it != _predIDMap.end(); ++it) {
			delete it->second;
	}
}

void FREvaluator::setQP(QueryProcessorImpl* qpImpl) {
	_qpImpl = qpImpl;
}

bool FREvaluator::evalRemainingPredAndMakeNewInstance(Node* pNode,
													  RebindEdge* pCurEdge, 
													  Instance* pInst, 
													   /*const*/ Event* ev) {
	char* srcInstanceArray = pInst ? pInst->getBody() : NULL;

	//evaluate remaining predicate
	if (pCurEdge->_pDNF != NULL && !pCurEdge->_pDNF->eval(ev, pInst))
		return false;

	//add this node to _qpImpl->_nodesTouchedInEpoch if it's not start node
	if (pNode->getNodeType() != typeid(StartNode)) {
		_qpImpl->_nodesTouchedInEpoch.insert(pNode->getID());
	}

	//create new instance with Interpreter byte code
	IntermediateNode* pDestNode = 
		pCurEdge->getEdgeType() == typeid(RebindEdge) 
		? (IntermediateNode*)pNode 
		: ((ForwardEdge*)pCurEdge)->_pDestNode;   

// 	//		luoluo 3.21
// 	//		don not need the new instance  created under the rebindedge
// 	if (pCurEdge->getEdgeType() == typeid(RebindEdge))return true;
    
	Instance* pNewInstance = Instance::allocInstance(pDestNode->getSchemaLen());
	//Instance* pNewInstance = 
	//	(Instance*)CY_Heap::allocMGeneric(sizeof(Instance) 
	//	+ pDestNode->getSchemaLen());
	//// placement new
	//pNewInstance = new(pNewInstance) Instance(pDestNode->getSchemaLen()); 

	if(!pCurEdge->hasDel())  {
		if (pInst) {
			pNewInstance->setStartTime(pInst->getStartTime());
		}
		else {
			pNewInstance->setStartTime(ev->getStartTime());
		}
	}
	else  {
		pNewInstance->setStartTime(ev->getEndTime());
	}

	pNewInstance->setEndTime(ev->getEndTime());

	//luoluo 8-18 
	//to get the num beyond to the instance.
	if (pInst)
	{
		//luoluo 8-7
		pNewInstance->num = pInst->num;
	// 	if (pInst->getID() == pNewInstance->getID())pNewInstance->num +=1;
	// 	else pNewInstance->num =1;
	switch (	pNode->getID()==pDestNode->getID())
	{
	case 0:
		pNewInstance->num =1;
		break;
	case 1:
		pNewInstance->num +=1;
		break;
		}
	}
	else pNewInstance->num =1;

	//luoluo 8-18
	//for the predicate window
	predWindow * ptr = ((IntermediateNode*)pNode) ->returnPredWindow();
	if (ptr &&  ((IntermediateNode*)pNode)->getNodeType() != typeid(StartNode))
	{
		long temp = ptr->getPredWinTemporal();
		long cnt	 = ptr->getPredWinCount();
		TimeStamp start = pNewInstance->getStartTime();
		TimeStamp end   =pNewInstance->getEndTime();
		long  calcu_tempral = end - start - (TimeStamp)temp;
		int  nums =pNewInstance->num;
		long  calcu_count	 = nums - cnt;

// 		cout<<"the node id:  "<<pNode->getID()<<"  the temp :  "<<temp<<
// 			"  the time: "<<start<<"     "<<end<<"calcu_tempral   "<<calcu_tempral<<endl;

		if (calcu_tempral > 0 || calcu_count >0)
		{
// 			cout<<"test******the node id:  "<<pNode->getID()<<"  the temp :  "<<temp<<
// 				"  the time: "<<start<<"     "<<end<<endl;
			Instance::freeInstance(pNewInstance);
			return false;
		}
	}



	//luoluo 4-7
	pNewInstance->SetCorrelatedEvent(ev->getID());


	if (pInst)
	{   
// 		assert(pInst->ReturnCorrelation().begin()==pInst->ReturnCorrelation().end());
// 		pNewInstance->ReturnCorrelation().insert(pInst->ReturnCorrelation().begin(),
// 			pInst->ReturnCorrelation().end());
// 		assert(pInst->ReturnCorrelation());
// 		   hash_set<const Event*>::iterator ite_s =pInst->ReturnCorrelation().begin();
// 		   hash_set<const Event*>::iterator ite_e =pInst->ReturnCorrelation().end();
		hash_set</*const*/ EventID>::iterator ite_s =pInst->ptr_event.begin();
		hash_set</*const*/EventID>::iterator ite_e =pInst->ptr_event.end();
		   while( ite_s !=ite_e){
//			pNewInstance->ReturnCorrelation().insert(* ite_s);
		   pNewInstance->SetCorrelatedEvent((* ite_s));
		   _qpImpl->_linkEvIns.insert(make_pair((*ite_s),pNewInstance));
			++ ite_s;
		   }
// 	//luoluo 8-7
// 		   pNewInstance->num = pInst->num;
// 		   if (pInst->getID() == pNewInstance->getID())pNewInstance->num +=1;
// 		   else pNewInstance->num =1;

	}
/*	else pNewInstance->num =1;*/

	_qpImpl->_linkEvIns.insert(make_pair(ev->getID(),pNewInstance));
 	//luoluo 5-4
	if (pNewInstance->getStartTime() ==pNewInstance->getEndTime())
			ev->setProcessOrnot();
/*	Event::_processOrnot=true;*/
	


	//CP num_chunks ev srcInstanceArray dest_array
	overwriteArrayFieldInCPInstruction(pCurEdge->_schemaMapProgram,
		ev->getBody(), 
		srcInstanceArray,
		pNewInstance->getBody());

	Interpreter::getInstance()->exec(pCurEdge->_schemaMapProgram);

	_qpImpl->_nodesTouchedInEpoch.insert(pDestNode->getID());
	pDestNode->addNewInstance(pNewInstance);

	//#ifdef _DEBUG
	if (ConfigMgr::getInstance()->getOptions()->_bVerbose) {
		CayugaSystem::getInstance()->debugOstream()<<"instance "
			<<pNewInstance->getID()
			<<" created under node "
			<<pDestNode->getID()
			<<" traversed edge "<<pCurEdge->getID()<<endl;
		//cout<<" since event ending at "<<ev->getEndTime();
		pDestNode->getNodeSchema()->printEvent(pNewInstance,
			CayugaSystem::getInstance()->debugOstream());
		CayugaSystem::getInstance()->debugOstream()<<endl;

// 		//luoluo 4-29
// 		for (hash_set<const Event *> ::iterator ite = pNewInstance->ptr_event.begin();
// 			ite != pNewInstance->ptr_event.end(); ++ ite)
// 		{
// 			cout<<"***---***"<<(**ite).getID()<<endl;
// 		}
 	}

	if (ConfigMgr::getInstance()->getOptions()->_bRecordTrace) {
		EventID srcInstID = pInst ? pInst->getID() : BAD_EVENT_ID; 
		TraceMgr::getInstance()->generateInstanceMsg(
			pNode->getID(), srcInstID, pDestNode, pNewInstance, 
			pNewInstance->getStartTime(),
			pNewInstance->getEndTime());
	}
	//#endif //_DEBUG

	//output generator here 
	//for monitoring match
	//if the node has output stream and has instances, output stuff
	assert (pDestNode->getNodeType() != typeid(StartNode));
	//new code for output sender
	if (((IntermediateNode*)pDestNode)->hasOutputStream()) {	
		_qpImpl->sendOutputEvent(
			(IntermediateNode*)pDestNode, 
			pNewInstance);
	}

	return true;
}

bool FREvaluator::evalStaticForwardPred(StartNode* pNode, /*const*/ Event* ev) {
	///@invariant Since here we we evaluate rebind edge and forward edge(s) at
	///unspecified order, the AIR file should guarantee that forward edge 
	///predicate is TRUE implies rebind predicate is TRUE as well

	//we should have set the head sword edge to NULL during the processing of
	//the last event, by calling pNode->clearSwordClauses() at 
	//FREvaluator::eval().
	assert (pNode->_pHeadSword == NULL);
	RebindEdge* pCurEdge = NULL;

	hash_set<Edge*> outputEdges;
	//for (vector<HashIndexType*>::iterator hash_it = _frIndices.begin();
	//	hash_it != _frIndices.end(); ++hash_it) {
	
	bool bHasRebindEdge = false;
	if (typeid(*pNode) == typeid(IntermediateNode)) {
		if (((IntermediateNode*)pNode)->_pRebind)
			bHasRebindEdge = true;
	}
	bool bRebindEdgeSatisfied = false;

	for (Node::PredicateIndex::iterator hash_it = pNode->_index.begin();
		hash_it != pNode->_index.end(); ++hash_it) {
		hash_set<HashIndexIDType*>* pIDSet;
		(hash_it->second)->eval(ev->getBody(), pIDSet, pNode);

		if (pIDSet) {
			for (hash_set<HashIndexIDType*>::iterator it = pIDSet->begin();
				it != pIDSet->end(); ++it) {
				HashIndexIDType* pThisID = *it;
				
				//RebindEdge* pThisEdge = (RebindEdge*)pThisID->first;
				RebindEdge* pThisEdge = (pThisID->first == REBIND_EDGE_ID) ? 
					((IntermediateNode*)pNode)->_pRebind
					: ((IntermediateNode*)pNode)->_pForwards[pThisID->first];

				assert (pThisEdge->getEdgeType() == typeid(RebindEdge) 
					|| pThisEdge->getEdgeType() == typeid(ForwardEdge));

				if (pThisEdge->getEdgeType() == typeid(RebindEdge)) {
					bRebindEdgeSatisfied = true;
				}

				//remaining static as well as dynamic predicates in 
				//clause pThisID->second will be evaluated on the per
				//instance basis.
				//set the DNF clause there
				if (pThisID->second != NO_DNF) {
					//this means the edge has TRUE predicate
					pThisEdge->setSwordDNFClause(pThisID->second);
				}

				//if the edge is already in output, 
				//skip the edge linking
				if(outputEdges.find(pThisEdge) 
					!= outputEdges.end()) {
					continue;
				}
				else {
					if (pNode->_pHeadSword == NULL) {
						//first sword edge under this node
						pNode->_pHeadSword = pCurEdge = pThisEdge;
						pNode->_pHeadSword->setNextSwordEdge(NULL);
					}
					else {
						if (pThisEdge->getEdgeType() == typeid(RebindEdge)) {
							//always put rebind edge ahead of all forward edges
							//since pNode->_pHeadSword is not NULL, do not need
							//to change pCurEdge
							pThisEdge->setNextSwordEdge(pNode->_pHeadSword);
							pNode->_pHeadSword = pThisEdge;
						}
						else {
							pCurEdge->setNextSwordEdge(pThisEdge);
							pCurEdge = pThisEdge;
						}
					}
				}
			}
		}
	}

	//if this node has a rebind edge but the static indexed component of that 
	//rebind predicate is not satisfied, will just return false
	if (bHasRebindEdge && !bRebindEdgeSatisfied) {
		return false;
	}

	return true;
}

void FREvaluator::eval(/*const*/ Event* ev, Node* pNode) {
	//This function marks all forward/rebind edges associated with the input 
	// node where indexed static predicates are satisfied by the current event.
	//For each edge, the conjunctive clauses whose index static predicates are
	//marked as well.
	bool bContinueEval = evalStaticForwardPred((StartNode*)pNode, ev);

	RebindEdge* pCurEdge;

	if (pNode->getNodeType() == typeid(StartNode)) {
		pCurEdge = ((StartNode*)pNode)->_pHeadSword;
		while (pCurEdge != NULL) {
			evalRemainingPredAndMakeNewInstance(pNode, pCurEdge, NULL, ev);
			pCurEdge = pCurEdge->getNextSwordEdge();
		}
		((StartNode*)pNode)->clearSwordEdges();
	}
	else {
		//For each instance, evaluate remaining predicates in marked clausesa
		IntermediateNode* pCurIntNode = (IntermediateNode*)pNode;
		assert (pCurIntNode->hasInstance());
		//while (PInst != NULL) {
		for (hash_map<EventID, Instance*>::iterator inst_it 
			= pCurIntNode->_affectedInstances.begin();
			inst_it != pCurIntNode->_affectedInstances.end() && bContinueEval; 
			++inst_it) {
				Instance* pInst = inst_it->second;
				pInst->setTouched();

				//for this instance pInst, evaluate all sword edges
				pCurEdge = pCurIntNode->_pHeadSword;
				while (pCurEdge != NULL) {
					//pCurIntNode->getNodeSchema()->printEvent(pInst);
					bool bSatisfied =
						evalRemainingPredAndMakeNewInstance(pNode, 
						pCurEdge, 
						pInst, 
						ev);

					//if the rebind edge is not satisfied, we do not need to 
					//continue to evaluate forward edges
					if (!bSatisfied && typeid(*pCurEdge) == typeid(RebindEdge))
						break;

					pCurEdge = pCurEdge->getNextSwordEdge();
				}
		}
		pCurIntNode->_affectedInstances.clear();
		pCurIntNode->clearSwordEdges();
	}
}

void FREvaluator::addIndexSpec(const PredIndexSpecString& indexSpec, 
							   const Context& context) {
	_HashPredIndexSpec hashIndexSpec 
		= indexSpec.spec2InternalIndexSpec(context);

	HashIndexType* pIndex = _indexMap.find(hashIndexSpec);
	if (pIndex) {
		context._pSourceNode->addIndexSpec(pIndex);
		return; //if the index exists, return
	}

	//create one
	size_t indexInputLengthOut; //number of bytes
	char* indexInputPackProgram = indexSpec.compile(context,
		indexInputLengthOut);
	HashIndexType* pHashIndex = new HashIndexType
		(indexInputPackProgram, indexInputLengthOut, NUM_BUCKETS, true);

	bool ret = _indexMap.insert(hashIndexSpec, pHashIndex);
	assert (ret);
	_indexSpec.push_back(hashIndexSpec);
	_frIndices.push_back(pHashIndex);
	context._pSourceNode->addIndexSpec(pHashIndex);
}

void
FREvaluator::insertPred(const Context& context, 
						const vector<AttributeID>* pHashIndexSpec, 
						const vector<NormalAtomPredString*>& indexedPreds) {
	HashIndexType* pHashIndex;
	if (pHashIndexSpec == NULL) {
		pHashIndex = pDefaultHashIndex;

		//For default index, associate it with the source node of this 
		//dummy predicate to index
		context._pSourceNode->addIndexSpec(pHashIndex);
	}
	else {
		pHashIndex = _indexMap.find(*pHashIndexSpec);
		assert (pHashIndex != NULL);
	}

	//pred
	char* strPred = new char[pHashIndex->getInputSize()];
	size_t offset = 0;
	const StreamSchema* pSchema 
		= StreamSchemaMgr::getInstance()->getStreamSchema(context._streamID);
	for (vector<NormalAtomPredString*>::const_iterator it 
		= indexedPreds.begin();
		it != indexedPreds.end(); ++it) {
			//SHORT CUT
			///@bug For now only deal with static predicates
			StaAtomPredString* pPred = dynamic_cast<StaAtomPredString*>(*it);
			assert (pPred != NULL);
			AttributeID eventAttrID = pSchema->getID(pPred->getEventAttr());
			size_t attrSize = pSchema->
				getInternalAttributeSize(eventAttrID);
			assert (attrSize != VARIABLE_LENGTH);

			AttrTypeID attrType = pSchema->getAttributeType(eventAttrID);
			string attrTypeName = DataTypeMgr::getInstance()->getName(attrType);

			convertConstToByteArray(
				attrTypeName,
				pPred->getConst(), 
				attrSize, 
				strPred+offset,
				true,		//ping string constants from queries/predicates
				true		//put strings into external space
				);

			offset += attrSize;
	}

	//make node pointer as the last part of the index key here
	assert (offset == pHashIndex->getInputSize() - 4);
	//*(Node**)(strPred+offset) = context._pSourceNode;
	*(NodeID*)(strPred+offset) = context._pSourceNode->getID();

	HashIndexIDType* pNewID = new HashIndexIDType(
		make_pair(context._pCurrentEdge->getID(), context._clauseNo));
	pHashIndex->insertPred(strPred, pNewID, pNewID);
	//_predIDMap.insert(make_pair(context._pCurrentEdge->getID(), pNewID));
	_predIDMap.insert(make_pair(
		PredKey(context._pSourceNode->getID(), 
			context._pCurrentEdge->getID(), context._clauseNo),
		pNewID));
	
}

void FREvaluator::changeForwardPredKey(StartNode* pOldNode,
									   ForwardEdge* pEdge,
									   EdgeID oldEdgeID, 
									   StartNode* pNewNode,
									   EdgeID newEdgeID) {
	if (pEdge->getDNF()) {
		for (size_t i = 0; i < pEdge->getNumClauses(); i++) {
			HashIndexMap::iterator find_it 
				//= _predIDMap.find(oldEdgeID);
				= _predIDMap.find(PredKey(pOldNode->getID(), 
				oldEdgeID, i));
			assert (find_it != _predIDMap.end());
			HashIndexIDType* pOldID = find_it->second;
			for (Node::PredicateIndex::iterator hash_it 
				= pOldNode->_index.begin();
				hash_it != pOldNode->_index.end(); ++hash_it) {
					HashIndexType* pHashIndex = hash_it->second;

					char* pOldKey = pHashIndex->getPredKey(pOldID);
					if (pOldKey) {
						bool ret = pHashIndex->deletePred(pOldID);
						assert (ret == true);

						//insert the new pred here
						char* pNewKey = new char[pHashIndex->getInputSize()];
						memcpy(pNewKey, pOldKey, pHashIndex->getInputSize());
						size_t offset = pHashIndex->getInputSize() - 4;
						//*(Node**)(pNewKey+offset) = pNewNode;
						*(NodeID*)(pNewKey+offset) = pNewNode->getID();

						*(EdgeID*)(pOldID) = newEdgeID;
						pHashIndex->insertPred(pNewKey, pOldID, pOldID);
	
						_predIDMap.erase(find_it);
						_predIDMap.insert(make_pair(PredKey(pNewNode->getID(), 
							newEdgeID, i), pOldID));
						break;
					}
			}
		}
	}
	else {
		HashIndexMap::iterator find_it 
			= _predIDMap.find(PredKey(pOldNode->getID(), 
			oldEdgeID, NO_DNF));
		assert (find_it != _predIDMap.end());
		HashIndexIDType* pOldID = find_it->second;

		//edges and managed by the special list "index"
		HashIndexType* pHashIndex = pDefaultHashIndex;
		char* pOldKey = pHashIndex->getPredKey(pOldID);
		assert (pOldKey);
		bool ret = pHashIndex->deletePred(pOldID);
		assert (ret == true);

		//insert the new pred here
		char* pNewKey = new char[pHashIndex->getInputSize()];
		memcpy(pNewKey, pOldKey, pHashIndex->getInputSize());
		size_t offset = pHashIndex->getInputSize() - 4;
		//*(Node**)(pNewKey+offset) = pNewNode;
		*(NodeID*)(pNewKey+offset) = pNewNode->getID();
		
		//*(EdgeID*)(pOldID) = newEdgeID;
		pOldID->first = newEdgeID;
		size_t clauseNo = pOldID->second;
		//size_t clauseNo = *(size_t*)(pOldID + sizeof(EdgeID));
		assert (clauseNo == NO_DNF);
		pHashIndex->insertPred(pNewKey, pOldID, pOldID);
		
		_predIDMap.erase(PredKey(pOldNode->getID(), 
			oldEdgeID, NO_DNF));
		_predIDMap.insert(make_pair(PredKey(pNewNode->getID(), 
				newEdgeID, NO_DNF), pOldID));}
}

void FREvaluator::deletePredOnEdge(StartNode* pOldNode,
								   Edge* pOldEdge) {
	//Bug caught by Varun on 06.06.07: pOldEdge could have a NULL
	//pointer to the DNF predicate associated with it, if the forward predicate
	//is TRUE. So we commend out the following assertion. Note that in this case
    //pOldEdge->getNumClauses() will return 0, and so the invocation of this 
    //function is a no-op.
	//assert (pOldEdge->getDNF());
	for (size_t i = 0; i < pOldEdge->getNumClauses(); i++) {
		HashIndexMap::iterator find_it = _predIDMap.find(
			PredKey(pOldNode->getID(), 
			pOldEdge->getID(), i));
		assert (find_it != _predIDMap.end());
		HashIndexIDType* pOldID = find_it->second;
		bool ret = false;
		for (Node::PredicateIndex::iterator hash_it 
			= pOldNode->_index.begin();
			hash_it != pOldNode->_index.end(); ++hash_it) {
				HashIndexType* pHashIndex = hash_it->second;
				ret = pHashIndex->deletePred(pOldID);
				if (ret) {
					delete pOldID;
					_predIDMap.erase(find_it);
					break;
				}
		}
		assert (ret);
	}
}

//void FREvaluator::changeForwardPredKey(StartNode* pOldNode,
//									   ForwardEdge* pOldEdge, 
//									   StartNode* pNewNode) {
//	//assert (pOldNode->_pForwards.size() == 1);
//	//ForwardEdge* pOldEdge = pOldNode->_pForwards[0];
//	//ForwardEdge* pNewEdge = pNewNode->findSameForwardEdge(pOldEdge);
//	//assert (pNewEdge);
//	changePredKey(pOldNode, pOldEdge, pNewNode);
//}


void FREvaluator::deleteForwardEdgeOnNode(StartNode* pOldNode) {
	//assert (pOldNode->_pForwards.size() == 1);
	for (size_t edge_idx = 0; 
		edge_idx < pOldNode->_pForwards.size(); ++edge_idx) {
		ForwardEdge* pOldEdge = pOldNode->_pForwards[edge_idx];
		deletePredOnEdge(pOldNode, pOldEdge);
	}
}

void FREvaluator::deleteRebindEdgeOnNode(IntermediateNode* pOldNode) {
	if (pOldNode->_pRebind && pOldNode->_pRebind->_pDNF) {
		deletePredOnEdge(pOldNode, pOldNode->_pRebind);
	}
}

#ifdef _DEBUG
void FREvaluator::outputStats(ostream& out) {
	out<<"stats of hash indices in FREvaluator"<<endl;
	for (vector<HashIndexType*>::iterator index_it = _frIndices.begin();
		index_it != _frIndices.end(); ++index_it) {
			(*index_it)->writeStats(out);
/*			out <<"***********"<<(* index_it)->getID()<<"******"<<endl;*/
	}
}
#endif //_DEBUG
