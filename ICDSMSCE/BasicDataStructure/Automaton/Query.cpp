/**
 * @file Query.cpp
 * @date November 23, 2005
 * @brief Implementation file of Query class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "BasicDataStructure/Automaton/Query.h"
#include "BasicDataStructure/Automaton/IntermediateNode.h"
#include "BasicDataStructure/Automaton/ForwardEdge.h"
using namespace Cayuga::BasicDataStructure::Automaton;


#include "Index/HashIndex.h"
using namespace Cayuga::Index;

#include "QueryProcessor/FilterEvaluator.h"
#include "QueryProcessor/FREvaluator.h"
using namespace Cayuga::QueryProcessor;

#include "SAXHandler/ConfigMgr.h"
using Cayuga::SAXHandler::ConfigMgr;

#include "SystemMgr/Options.h"
#include "SystemMgr/StreamSchema.h"
using Cayuga::SystemMgr::Options;
using Cayuga::SystemMgr::StreamSchema;

#include <cassert>
using namespace std;

inputWindow * Query:: _inputwindows = inputWindow ::getInstance();

void Query::addNode(Node* pNode) {
	if (pNode->getNodeType() == typeid(StartNode)) {
		_startNodes.push_back(pNode);
	}
	else {
		_intermediateNodes.push_back(pNode);
	}
}
// void Query::addWindow(Window* pWindow){
// 	_windows.push_back(pWindow);
// }

void Query::addinputWindow(inputWindow * ptrWindow){
	_inputwindows=ptrWindow;
}

// void Query::addpredWindow(predWindow * ptrPredWindow)
// {
// // 	_predwindow = ptrPredWindow;
//    _predwindow.push_back(ptrPredWindow);
// }



/**
	The state merging process is non-trivial. We want to maintain the invariant
	that each node reads one input stream (all of its out-going edges read one 
	stream). Therefore existing automata structure in Cayuga is a forrest of
	DAG automata, denoted as Q. The automata structure corresponding to query q
	could be a forrest of DAG automaton as well, for example (S1;S2) U (S3;S4).

	We consider merging each DAG automaton, starting from node sq, in q 
	separately. This process of merging nodes is done recursively.

	In the merging procedure, we make one simplification on the automata 
	structure: No two forward edges can lead to the same node. This assumption
	helps simplify our recursive node merging procedure, where a node v in q can
	be deleted after it is merged with a node in Q. If v has multiple incoming
	forward edges, however, we cannot delete v after its first incoming forward
	edge is merged.

	Here is the formal algorithm.

	MergeQuery (Q, q) {
		for each start node sq in q
			locate "matching" start node in Q, denoted as sQ
			if no such sQ exists
				create a new DAG in Q starting from sq
			else 
				mergeNode (sQ, sq)
	}

	MergeNode (sQ, sq) {
		for each forward edge of sq leading to node vq
			find a forward edge of sQ leading to vQ, where vQ "matches" vq
			if vQ does not exist
				migrate the forward edge of sq to source node sQ
				insert all nodes reachable from sq to the automata forest
			else
				delete index entries for filter, rebind edges of sq
				delete index entries for incoming forward edge of sq
				if vq is not end node
					MergeNode (vQ, vq)
				else 
					move all output streams of vq to vQ
					delete vq

		move all index specs associated with sq to sQ, if they are not empty

		//here is where the simplification helps : we can delete sq now since
		//we know the only incoming forward edge of sq has been handled properly
		delete sq
	}

	Two start nodes vQ and vq match, if
		* they read the same stream

	Two nodes vQ and vq match, if
		* they read the same stream
		* they have the same schema
		* they have the same filter, rebind edges
		* the forward edge leading to them in the merging process is the same. 
		  With the simplification we made, which says each node has only one 
		  incoming forward edge, the above two rules are equivalent 
		  to saying that the two nodes have the same set of incoming edges.

*/
void Query::mergeQuery(Query* q) {
	//This segment is commented out, since it is a special case that will be 
	//handled in the general merging procedure below
	//if the query DAG is empty, set it to q, and done!
	//if (_startNodes.empty()) {
	//	_startNodes = q->_startNodes;
	//	_intermediateNodes = q->_intermediateNodes;
	//	return;
	//}

	bool bMergeStates = ConfigMgr::getInstance()->getOptions()->_bMergeStates;
	for (size_t sq_idx = 0; sq_idx < q->_startNodes.size(); ++sq_idx) {
		StartNode* pQueryNode = (StartNode*)q->_startNodes[sq_idx];
			
		//locate a matching start node in Q
		StartNode* pDAGNode = NULL;
		bool bMerge = false;
		if (bMergeStates) {
			//try merging starts states only if the config option MergeStates
			//is set to true
			for (size_t sQ_idx = 0; sQ_idx < _startNodes.size(); ++sQ_idx) {
				pDAGNode = (StartNode*)_startNodes[sQ_idx];
				if (*pQueryNode == *pDAGNode) {
					bMerge = true;
					break;
				}
			}
		}

		//found a matching start node
		if (bMerge) {
			mergeNode(pQueryNode, pDAGNode);
		}
		else {
			_startNodes.push_back(pQueryNode);

			FilterEvaluator::getInstance()->indexStartNode(pQueryNode);

			//need to handle intermediateNodes some where
			for (size_t edge_idx = 0; 
				edge_idx < pQueryNode->_pForwards.size(); ++edge_idx) {
					pushback(pQueryNode->_pForwards[edge_idx]->_pDestNode);
			}
		}
	}
// 	//luoluo 4-9
// 	//this statement must be modified if there are not only one window .
// 	//so there must be many question should be resolved , such as multi_window.
// 	_windows.push_back(*(q->_windows.begin()));
	//luoluo 5.21
/*	_inputwindows.push_back(*(q->_inputwindows.begin()));*/
	//luoluo 5-31
//	_inputwindows = q->_inputwindows;
	inputWindow* ptr = inputWindow::getInstance();
	* ptr =*(q->_inputwindows);

// 	//luoluo 5-25
// 	inputWindow::getInstance()->_windowevents.
	//luoluo 7-29
// 	_predwindow.push_back(*(q->_predwindow.begin()));
	

}


//push the successor nodes of q all to intermediateNodes
void Query::pushback(IntermediateNode* q) {
	_intermediateNodes.push_back((Node*)q);
	for (size_t edge_idx = 0; 
		edge_idx < q->_pForwards.size(); ++edge_idx) {
			pushback(q->_pForwards[edge_idx]->_pDestNode);
	}
}


void Query::mergeNode(StartNode* q, StartNode* Q) {
	for (size_t edge_idx = 0; edge_idx < q->_pForwards.size(); ++edge_idx) {
		IntermediateNode* pNextQueryNode = q->_pForwards[edge_idx]->_pDestNode;
		bool bNodeMerged = false;
		vector<ForwardEdge*>::iterator edge_it = Q->_pForwards.begin();
		for (;edge_it != Q->_pForwards.end(); ++edge_it) {
			IntermediateNode* pNextDAGNode = (*edge_it)->_pDestNode;
			if (*q->_pForwards[edge_idx] == **edge_it 
				&& *pNextDAGNode == *pNextQueryNode) {
					//since the query node will be merged,
					//delete predicates on the edges of the query node
					FilterEvaluator::getInstance()->
						deletePredOnNode(pNextQueryNode);

					//we should delete the rebind edge on the next level,
					//but delete the forward edge on the current level
					FREvaluator::getInstance()->
						deleteRebindEdgeOnNode(pNextQueryNode);
					FREvaluator::getInstance()->
						deleteForwardEdgeOnNode(q);

					if (!pNextQueryNode->hasOutputStream()) {
						mergeNode(pNextQueryNode, pNextDAGNode);
					}
					else {
						//pNextQueryNode is end node
						assert (pNextQueryNode->_inputStream == BAD_STREAM_ID);
						
						//also merge output stream names, and set the generating
						//nodes of output stream schema to the existing node
						//pNextDAGNode
						//for (vector<OutputStreamType>::iterator it
						//	= pNextQueryNode->_outputStreams.begin(); 
						//	it != pNextQueryNode->_outputStreams.end(); ++it) {
						//		pNextQueryNode->getNodeSchema()->
						//			setGeneratingNode(pNextDAGNode);
						//}

						//bug: temporary fix
						//we assume pNextQueryNode has only one output stream
						assert (pNextQueryNode->_outputStreams.size() == 1);
						pNextQueryNode->getNodeSchema()->
									setGeneratingNode(pNextDAGNode);

						pNextDAGNode->_outputStreams.insert(
							pNextDAGNode->_outputStreams.begin(),
							pNextQueryNode->_outputStreams.begin(),
							pNextQueryNode->_outputStreams.end());

						delete pNextQueryNode;
						//recursion ends here
					}

					bNodeMerged = true;
					break;
			}
		}

		//if pNextQueryNode can not be merged with any node pointing to by 
		//forward edges of this (sQ) node, we add a new forward edge that goes 
		//from this node to pNextQueryNode, insert all reachable nodes from 
		//pNextQueryNode into the intermediate node set, and we are done
		if (!bNodeMerged) {
			//since this node and q are merged,
			//we need to migrate the forward edge on q to this
			ForwardEdge* pEdge = q->_pForwards[edge_idx];
			assert ((EdgeID)edge_idx == pEdge->getID());
			//step 1: since the last part of the pred key of each forward edge 
			//pred is the source node of that foward edge, we need to change 
			//the pred key of the forward edge on q to this node.
			//The pred ID, however, consists of the edge pointer and the 
			//clause number, neither of which is changed during this migration 
			//process, and therefore does not need to be changed.
			Q->addEdge(pEdge);
			
			FREvaluator::getInstance()->
				changeForwardPredKey(q, pEdge, edge_idx, Q, pEdge->getID());

			//step 2: migrate that edge
			q->_pForwards[edge_idx] = NULL;

			//push the successor nodes of q all to intermediateNodes
			pushback(pNextQueryNode);
		}
	}

	for (Node::PredicateIndex::iterator it 
		= q->_index.begin(); it != q->_index.end(); ++it) {
			pair<size_t, Node::HashIndexType*> curIndex = *it;
			if (!curIndex.second->empty()) {
				Q->addIndexSpec(curIndex.second);
			}
			else {
				//do not delete this index from the system, since queries added
				//later could still use it
			}
	}
	delete q;
}

size_t Query::getSize() {
	return _startNodes.size() + _intermediateNodes.size();
}

void Query::write(ostream& out) const {
	out<<"<NFA>"<<endl;
	
	//if there are no automata, just return.
	if (_startNodes.empty()) {
		assert (_intermediateNodes.empty());
		out<<"</NFA>"<<endl;
		return;
	}

// 	//luoluo 4-14
// 	//write the window information from the AIR into the screen.
// 	for (vector<Window*> ::const_iterator it = _windows.begin();
// 		it != _windows.end() ; ++it){
// 		(*it)->write(out);
// 	}
// 		//luoluo 5-20
// 	for (vector<inputWindow*>::const_iterator it = _inputwindows.begin();
// 			it != _inputwindows.end(); ++it)
// 		{
// 			(* it)->write(out);
// 		}
			//luoluo 5-28
		_inputwindows->write(out);


	for (NodeSet::const_iterator it = _startNodes.begin();
		it != _startNodes.end();
		++it) {
			(*it)->write(out);
	}

	for (NodeSet::const_iterator it = _intermediateNodes.begin();
		it != _intermediateNodes.end();
		++it) {
			(*it)->write(out);
	}
	out<<"</NFA>"<<endl;
}

//void Query::printOccupancy(ostream& out)
//{	
//	for(vector<Node*>::iterator node_it = _intermediateNodes.begin();
//		node_it != _intermediateNodes.end(); ++node_it)
//	{
//		IntermediateNode* pNode = (IntermediateNode*) *node_it;
//		out<<"\nNode ID = "<<pNode->getID()<<"\tOccupancy = "<<pNode->getCounter();
//	}
//	out<<endl;
//}
