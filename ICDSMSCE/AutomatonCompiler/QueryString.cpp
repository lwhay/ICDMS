/**
 * @file QueryString.cpp
 * @date November 27, 2005
 * @brief Implementation file of QueryString class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */


#include "AutomatonCompiler/QueryString.h"

#include "AutomatonCompiler/Context.h"
#include "AutomatonCompiler/NodeString.h"
#include "AutomatonCompiler/StartNodeString.h"
#include "AutomatonCompiler/IntermediateNodeString.h"
#include "AutomatonCompiler/WindowString.h"
#include "AutomatonCompiler/predWindowString.h"
using namespace Cayuga::AutomatonCompiler;

#include "BasicDataStructure/Automaton/Query.h"
using namespace Cayuga::BasicDataStructure::Automaton;

#include "BasicDataStructure/Automaton/node.h"
#include "BasicDataStructure/Automaton/IntermediateNode.h"
#include "BasicDataStructure/Window/predWindow.h"

#include "Utils/RAssert.h"
using namespace Cayuga::Utils;

QueryString::QueryString(){
	_thisWindow=NULL;
	_thisPredWindow = NULL;
}
Query* QueryString::compile() {
	Query* pInternalQ = new Query();

	Context context;
	if(_thisWindow){
/*		Window *pwindow=_thisWindow->compile(context);*/
		//luoluo 5-20
		inputWindow * inwindow = _thisWindow->recompile(context);
        pInternalQ->addinputWindow(inwindow);
// 
//         pInternalQ->addWindow(pwindow);
	}

// 	//luoluo 7-29
// 	//there must be some problems when writing like this.
// 	if (_thisPredWindow)
// 	{
// 		predWindow * ptrwin = _thisPredWindow->recompile(context);
// 		pInternalQ->addpredWindow(ptrwin);
// 	}

	context._nodeType = START;
	for (StartNodeSet::iterator node_it = _startNodes.begin();
		node_it != _startNodes.end();
		++node_it) {
			NodeString* pStringNode = *node_it;
			Node* pInternalNode = pStringNode->compile(context);
			pInternalQ->addNode(pInternalNode);
	}

	context._nodeType = INTERMEDIATE_OR_END;
	for (IntermediateNodeSet::iterator node_it = _intermediateNodes.begin();
		node_it != _intermediateNodes.end();
		++node_it) {
			NodeString* pStringNode = *node_it;
			Node* pInternalNode = pStringNode->compile(context);
			//luoluo 8-2
			//because of not  index the suitation of no predicatewindow ,the 
			//only way is to judge whether the intermediateNode have outputstream.
			if (!(*node_it)->whether_endNode())
			{
				predWindowString * predWinString = (*node_it)->returnPredWindowString();
				if (predWinString == NULL)predWinString = (*node_it)->getPredWind();			

				//luoluo 8-19
				if (predWinString ==NULL)((IntermediateNode*)pInternalNode)->setPredWindow(NULL);
				else
				{
					predWindow * PredWin = predWinString->recompile( context);
					((IntermediateNode*)pInternalNode)->setPredWindow(PredWin);
				}		
			}
			pInternalQ->addNode(pInternalNode);
	}

	//workset should be empty after the compilation
	assert (context._patchDestNode.empty());
	assert (context._patchSchemaMap.empty());
	return pInternalQ;
}

void QueryString::write(ostream& out) {
	out<<"<NFA>"<<endl;

	if (_thisWindow)
	{
		_thisWindow->write(out);
	}
	for (StartNodeSet::iterator it = _startNodes.begin();
		it != _startNodes.end();
		++it) {
			(*it)->write(out);
	}

	for (IntermediateNodeSet::iterator it = _intermediateNodes.begin();
		it != _intermediateNodes.end();
		++it) {
// 			(*it)->write(out);
			//luoluo 7-31
			(*it)->writeCommonBeforeSchema(out);
			if ((*it)->_inputStream !="NULL")
			{
// 				_thisPredWindow->write(out);
// 				(* it)->_thisPredWindow->write(out);
				predWindowString * expr = (* it)->returnPredWindowString();
				if (expr !=NULL)expr->write(out);
			}
			(*it)->write(out);
	}
	out<<"</NFA>"<<endl;
}

void QueryString::addNode(NodeString* pNode) {
	if (pNode->getNodeType() == "START") {
		_startNodes.push_back((StartNodeString*)pNode);
	}
	else {
		_intermediateNodes.push_back((IntermediateNodeString*)pNode);
	}
}


ForwardEdgeString* QueryString::getLastForwardEdge() {
	Assert (!_intermediateNodes.empty());
	Assert (_startNodes.size() == 1);

	if (_intermediateNodes.size() == 1) {
		return _startNodes.front()->_pForwards.front();
	}
	else {
		return _intermediateNodes[_intermediateNodes.size()-2]->
			_pForwards.front();
	}
}

ForwardEdgeString* QueryString::getFirstForwardEdge() {
	Assert (!_intermediateNodes.empty());
	Assert (_startNodes.size() == 1);

	return _startNodes.front()->_pForwards.front();
}


StartNodeString* QueryString::getStartNode() {
	Assert (!_intermediateNodes.empty());
	Assert (_startNodes.size() == 1);
	
	return _startNodes.front();
}

IntermediateNodeString* QueryString::getEndNode() {
	Assert (!_intermediateNodes.empty());
	Assert (_startNodes.size() == 1);
	
	return _intermediateNodes.back();
}

IntermediateNodeString* QueryString::getSecondNode() {
	Assert (!_intermediateNodes.empty());
	Assert (_startNodes.size() == 1);
	
	return _intermediateNodes.front();
}

IntermediateNodeString* QueryString::getSecondToEndNode() {
	Assert (_intermediateNodes.size() >= 2);
	Assert (_startNodes.size() == 1);
	
	return _intermediateNodes[_intermediateNodes.size()-2];
}

