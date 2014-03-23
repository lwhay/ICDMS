/**
 * @file FilterEvaluator.cpp
 * @date November 26, 2005
 * @brief Implementation file of FilterEvaluator class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "QueryProcessor/FilterEvaluator.h"
#include "QueryProcessor/QueryProcessorImpl.h"
using namespace Cayuga::QueryProcessor;

#include "AutomatonCompiler/Context.h"
#include "AutomatonCompiler/convertConstToByteArray.h"
#include "AutomatonCompiler/AtomPredString.h"
#include "AutomatonCompiler/StaAtomPredString.h"
#include "AutomatonCompiler/PredIndexSpecString.h"
using namespace Cayuga::AutomatonCompiler;



#include "BasicDataStructure/Automaton/Instance.h"
#include "BasicDataStructure/Automaton/IntermediateNode.h"
#include "BasicDataStructure/Automaton/ForwardEdge.h"
#include "BasicDataStructure/Predicate/Predicates_DNF.h"
#include "BasicDataStructure/Event/Event.h"
using namespace Cayuga::BasicDataStructure::Automaton;
using namespace Cayuga::BasicDataStructure::Predicate;
using namespace Cayuga::BasicDataStructure::EventNS;

#include "Index/HashIndex.h"

#include "SystemMgr/DataTypeMgr.h"
#include "SystemMgr/StreamSchemaMgr.h"
using namespace Cayuga::SystemMgr;

/////////////////
//FilterIndex
/////////////////

FilterIndex::FilterIndex() {
	pDefaultHashIndex = new HashIndexType(NULL, 0, 1); 
	_index.push_back(pDefaultHashIndex);
}

FilterIndex::~FilterIndex() {
	for (vector<HashIndexType*>::iterator it = _index.begin();
		it != _index.end(); ++it) {
			delete *it;
	}
	delete pDefaultHashIndex;

	for (HashIndexMap::iterator it = _predIDMap.begin();
		it != _predIDMap.end(); ++it) {
			delete it->second;
	}
}

void FilterIndex::addIndexSpec(const PredIndexSpecString& indexSpec, 
								   const Context& context) {
	_HashPredIndexSpec hashIndexSpec 
		= indexSpec.spec2InternalIndexSpec(context);

	HashIndexType* pIndex = _indexMap.find(hashIndexSpec);
	if (pIndex) return; //if the index exists, return

	//create one
	size_t indexInputLengthOut; //number of bytes
	char* indexInputPackProgram = indexSpec.compile(context,
		indexInputLengthOut);
	HashIndexType* pHashIndex = new HashIndexType
		(indexInputPackProgram, indexInputLengthOut, NUM_BUCKETS);

	/*bool ret =*/ _indexMap.insert(hashIndexSpec, pHashIndex);
	_indexSpec.push_back(hashIndexSpec);
	_index.push_back(pHashIndex);
}

void FilterEvaluator::addDynIndexSpec(const PredIndexSpecString& indexSpec,
						   const Context& context)
{	
	_HashDynPredIndexSpec hashIndexSpec 
		= indexSpec.spec2InternalDynIndexSpec(context);

	size_t indexInputLengthOut; //number of bytes
	char* indexInputPackProgram = indexSpec.compile(context,
		indexInputLengthOut);
	DynHashIndexType* pHashIndex = new DynHashIndexType
		(indexInputPackProgram, indexInputLengthOut, NUM_BUCKETS);
	
	((IntermediateNode*)context._pSourceNode)->setDynIndex(pHashIndex, 
									hashIndexSpec);
}

void 
FilterIndex::insertPred(const Context& context, 
							const vector<AttributeID>* pHashIndexSpec, 
							const vector<NormalAtomPredString*>& indexedPreds) {
	HashIndexType* pHashIndex;
	if (pHashIndexSpec) {
		pHashIndex = _indexMap.find(*pHashIndexSpec);
		assert (pHashIndex != NULL);
	}
	else {
		pHashIndex = pDefaultHashIndex;
	}

	//convert pred to the input of hash index
	char* strPred = new char[pHashIndex->getInputSize()];
	size_t offset = 0;

	const StreamSchema* pSchema 
		= StreamSchemaMgr::getInstance()->getStreamSchema(context._streamID);
	for (vector<NormalAtomPredString*>::const_iterator it = indexedPreds.begin();
		it != indexedPreds.end(); ++it) {	
			//SHORT CUT: for now only deal with static predicates
			StaAtomPredString* pPred = dynamic_cast<StaAtomPredString*>(*it);
			assert (pPred != NULL);
			AttributeID eventAttrID = pSchema->getID(pPred->getEventAttr());
			size_t attrSize = pSchema->
				getInternalAttributeSize(eventAttrID);
			assert (attrSize != VARIABLE_LENGTH);
				
			AttrTypeID attrType = pSchema->getAttributeType(eventAttrID);
			string attrTypeName = DataTypeMgr::getInstance()->getName(attrType);
			//size_t size_rhs = DataTypeMgr::getInstance()->
			//	getInternalLength(attrTypeName);

			//SHORT CUT
			///@bug Assume we are only precessing static predicate
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

	Node* pNode = context._pSourceNode;
	HashIndexIDType* pNewID = new HashIndexIDType(make_pair(pNode->getID(), 
		context._clauseNo));

	pHashIndex->insertPred(strPred, pNewID, pNewID);
	_predIDMap.insert(make_pair(pNode->getID(), pNewID));
}

void FilterIndex::deletePredOnNode(IntermediateNode* pOldNode) {
	if (!pOldNode->_pFilter) {
		return;
	}

	assert (pOldNode->_pFilter);
	size_t numClauses = pOldNode->_pFilter->_pDNF ? 
		pOldNode->_pFilter->_pDNF->getNumClauses() : 1;
	for (size_t i = 0; i < numClauses; ++i) {
		HashIndexMap::iterator find_it = _predIDMap.find(pOldNode->getID());
		assert (find_it != _predIDMap.end());
		HashIndexIDType* pOldID = find_it->second;
		bool ret = false;
		for (vector<HashIndexType*>::iterator hash_it = _index.begin();
			hash_it != _index.end(); ++hash_it) {
				HashIndexType* pHashIndex = *hash_it;
				ret = pHashIndex->deletePred(pOldID);
				if (ret) {
					delete pOldID;
					_predIDMap.erase(find_it);
					break;
				}
		}
		assert (ret == true);
	}

}


void FilterIndex::eval(const Event* ev, hash_set<NodeID>& affectedNodes) {
	for (vector<HashIndexType*>::iterator hash_it = _index.begin();
		hash_it != _index.end(); ++hash_it) {
		hash_set<HashIndexIDType*>* pIDSet;
		hash_map<EventID, Instance*>* pInstSet;
		(*hash_it)->eval(ev->getBody(),pIDSet);

		if (pIDSet) {
			for (hash_set<HashIndexIDType*>::iterator it = pIDSet->begin();
				it != pIDSet->end(); ++it) {
				HashIndexIDType* pThisID = *it;

				Node* pCurNode = Node::_nodes[pThisID->first];

				//assert (pCurNode->getNodeType() != typeid(StartNode));
				if (pCurNode->getNodeType() == typeid(StartNode)) {
					affectedNodes.insert(pCurNode->getID());
					continue;
				}

				IntermediateNode* pCurIntNode 
					= (IntermediateNode*)pCurNode;
				//if the node is not active, skip it
				if(!pCurIntNode->hasInstance()) {
					continue;
				}

				if(pCurIntNode->_pFilter->_pDNF) {
					//evaluate the unindexed static predicates in clause 
					//pThisID->second 
					bool staSatisfied = pCurIntNode->_pFilter->_pDNF->
										_clauses[pThisID->second]->
										evalStatic(ev);
					if(!staSatisfied) {
						continue;
					}
				
					//evaluate the dynamic predicates.
					//first check whether there is dynamic index
					bool isDynIndexed = pCurIntNode->isClauseDynIndexed
						(pThisID->second);
					if(isDynIndexed) {					
						pCurIntNode->getDynFilterPredIndex()->
								eval(ev->getBody(), pInstSet);
						for (hash_map<EventID, Instance*>::iterator inst_it 
							= pInstSet->begin();
							inst_it != pInstSet->end(); 
							++inst_it) {
							Instance* pInst = inst_it->second;
							assert (pInst->getEndTime() < ev->getEndTime());

							//if this event overlaps with the time interval of the 
							//instance, cannot concatenate them
							if (pInst->getEndTime() >= ev->getStartTime()) {
								continue;
							}

							bool ret;											
							ConjunctiveClause* pThisClause = pCurIntNode->_pFilter
											->_pDNF->_clauses[pThisID->second];
							ret = pThisClause->evalDynamic(ev, pInst);					
							if (!ret) {
								continue;
							}
							else {						
								affectedNodes.insert(pCurNode->getID());
								pCurIntNode->
									_affectedInstances.insert(
									make_pair(inst_it->first, pInst));
							}
						}				
						continue;
					}
				}

				//if there is no dynamic predicate index. 
				//sequentially evaluate the dynamic filter predicate on
				//each instance under the node
				for (list<Instance*>::iterator inst_it 
					= pCurIntNode->_instances.begin();
					inst_it != pCurIntNode->_instances.end(); 
				++inst_it) {
					Instance* pInst = *inst_it;
					//luoluo 4-11
					// the following  statement maybe have some logic problem.
					//write just for processing .
					if (pInst->getStartTime()>ev->getStartTime())break;


					//Bug fix on 06.06.07
					//According to the way we process events (by epoches), 
					//the end time of the instance should always be less 
					//than the end time of the current event, since 
					//otherwise the instance should not be visible by the 
					//current event -- that instance will be put in pending 
					//list,not current list of its associated state.
	//				assert (pInst->getEndTime() < ev->getEndTime());
					//luoluo 4-13
					//the above sentence  have been changed .(be deleted )

					//if this event overlaps with the time interval of the 
					//instance, cannot concatenate them
					if (pInst->getEndTime() >= ev->getStartTime()) {
						continue;
					}

					bool ret;
					if (pCurIntNode->_pFilter->_pDNF == NULL) {
						ret = true;
					}
					else {
						ConjunctiveClause* pThisClause = pCurIntNode->_pFilter
							->_pDNF->_clauses[pThisID->second];
						ret = pThisClause->evalDynamic(ev, pInst);
					}
					if (!ret) {
						continue;
					}
					else {						
						affectedNodes.insert(pCurNode->getID());
						pCurIntNode->
							_affectedInstances.insert(
							make_pair(pInst->getID(), pInst));
					}
				}
			}
		}
	}

}

#ifdef _DEBUG
void FilterIndex::outputStats(ostream& out) {
	for (vector<HashIndexType*>::iterator index_it = _index.begin();
		index_it != _index.end(); ++index_it) {
			(*index_it)->writeStats(out);
/*	out<<"***---"<<(* index_it)->getID()<<"---***"<<endl;  */
/*	PredIndexSpecString::getInstance()->write();*/
// 	PredIndexSpecString  * ptr;
// 	ptr->write();
	}

}
#endif //_DEBUG


/////////////////
//FilterEvaluator
/////////////////


FilterEvaluator::FilterEvaluator() : _qpImpl(NULL) {
}

FilterEvaluator::~FilterEvaluator() {
	for (hash_map<StreamID, FilterIndex*>::iterator it = _filterIndices.begin();
		it != _filterIndices.end(); ++it) {
			delete it->second;
	}
}

void FilterEvaluator::setQP(QueryProcessorImpl* qpImpl) {
	_qpImpl = qpImpl;
}

void FilterEvaluator::indexStartNode(Node* startNode) {
	Context context;
	context._clauseNo = 0; //this is not useful
	vector<NormalAtomPredString*> indexedPreds;

	context._streamID = startNode->getInputStream();
	context._pSourceNode = startNode;
	insertPred(context, 
		NULL, 
		indexedPreds);

}

FilterIndex* FilterEvaluator::getFilterIndex(const StreamID streamID) {
	hash_map<StreamID, FilterIndex*>::iterator find_it 
		= _filterIndices.find(streamID);

	if (find_it == _filterIndices.end()) {
		//create a new FilterIndex for this stream
		FilterIndex* pFilterIndex = new FilterIndex();
		_filterIndices.insert(make_pair(streamID, pFilterIndex));
		return pFilterIndex;
	}
	else {
		return find_it->second;
	}
}

void FilterEvaluator::addIndexSpec(const PredIndexSpecString& indexSpec, 
								   const Context& context) {
	FilterIndex* pFilterIndex = getFilterIndex(context._streamID);
	pFilterIndex->addIndexSpec(indexSpec, context);
}

void 
FilterEvaluator::insertPred(const Context& context, 
							const vector<AttributeID>* pHashIndexSpec, 
							const vector<NormalAtomPredString*>& indexedPreds) {
	FilterIndex* pFilterIndex = getFilterIndex(context._streamID);
	pFilterIndex->insertPred(context, pHashIndexSpec, indexedPreds);
}

void FilterEvaluator::deletePredOnNode(IntermediateNode* pOldNode) {
	FilterIndex* pFilterIndex = getFilterIndex(pOldNode->getInputStream());
	pFilterIndex->deletePredOnNode(pOldNode);
}

void FilterEvaluator::eval(/*const*/ Event* ev) {	
	assert (_qpImpl->_affectedNodes.empty());

	FilterIndex* pFilterIndex = getFilterIndex(ev->getStreamID());
	pFilterIndex->eval(ev, _qpImpl->_affectedNodes);
}

#ifdef _DEBUG
void FilterEvaluator::outputStats(ostream& out) {
	out<<"stats of hash indices in FilterEvaluator"<<endl;
	for (hash_map<StreamID, FilterIndex*>::iterator it = _filterIndices.begin();
		it != _filterIndices.end(); ++it) {
			out<<"on stream "<<it->first<<endl;
			it->second->outputStats(out);
	}


}
#endif //_DEBUG

void FilterEvaluator::indexInstance(IntermediateNode* currNode,									
									list<Instance*>& Instances)
{
	//convert pred to the input of hash index
	DynHashIndexType* pHashIndex = currNode->getDynFilterPredIndex();
	if(!pHashIndex)
		return;
	_HashDynPredIndexSpec pIndexSpec = currNode->getDynIndexSpec();
	char* strPred;
	size_t offset = 0;
	vector<pair<size_t, size_t> > sizeoffset;
	const StreamSchema* pSchema 
		= currNode->getNodeSchema();

	for(vector<pair<AttributeID, AttributeID> >::const_iterator spec_it
			= pIndexSpec.begin(); spec_it != pIndexSpec.end(); ++spec_it) {
		sizeoffset.push_back(pair<size_t, size_t>
				( pSchema->getInternalAttributeOffset(spec_it->second),
				pSchema->getInternalAttributeSize(spec_it->second)));
	}
				
	for (list<Instance*>::const_iterator it = Instances.begin();
		it != Instances.end(); ++it) {			
			Instance* pInst = *it;
			strPred = new char[pHashIndex->getInputSize()];			
			offset = 0;
			void* body = pInst->getBody();
			for(vector<pair<AttributeID, AttributeID> >::const_iterator spec_it
				= sizeoffset.begin(); spec_it != sizeoffset.end(); ++spec_it) {
				memcpy((char*)(strPred) + offset, ((char*)body + (spec_it->first)),
						spec_it->second);				
				offset += spec_it->second;
			}	
			pHashIndex->insertPred(strPred, pInst->getID(), 
				make_pair(pInst->getID(), pInst));
	}		
}

void FilterEvaluator::deleteInstance(IntermediateNode* pNode, 
									 Instance* pInst) {
	DynHashIndexType* pHashIndex = pNode->getDynFilterPredIndex();
	if(pHashIndex)
		pHashIndex->deletePred(pInst->getID());
}
