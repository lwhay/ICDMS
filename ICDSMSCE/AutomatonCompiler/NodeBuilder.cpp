/**
 * @file NodeBuilder.cpp
 * @date November 22, 2005
 * @brief Implementation file of NodeBuilder class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "AutomatonCompiler/NodeBuilder.h"
#include "AutomatonCompiler/IntermediateNodeString.h"
using namespace Cayuga::AutomatonCompiler;

#include "BasicDataStructure/Automaton/EdgeType.h"
using namespace Cayuga::BasicDataStructure::Automaton;

#include "Utils/AttrMap.h"

#include <string>
using namespace std;

NodeBuilder::NodeBuilder() : _state(INIT) {
	_isDynIndexed = false;
	_PredWindowBuilder = new predWindowBuilder();
}

void NodeBuilder::startElement(const std::string& namespaceURI, 
							   const std::string& localName,
							   const std::string& qName, 
							   const AttributesString& atts) {
	try {
		assert(localName.length());
		StringMap attrMap;
		//used for PredIndexSpecString
		string indexAttrType, eventAttrName, nodeAttrName;
		string attrName, attrType;
		switch(_state) {
			case INIT:
				assert (localName == "Node");
				_state = IN_NODE;
				createAttrMap(atts, attrMap);
				createNode(attrMap); 
				break;
			case IN_NODE:
				if (localName == "Schema") {
					_state = IN_SCHEMA;
				}
				else if (localName == "OutputStreamName") {
					createAttrMap(atts, attrMap);
					string streamName = getParameterValue<string>(attrMap, "Name");
					string streamType = getParameterValue<string>(attrMap, "Type", 
						               "OUT");
					bool isOut = (streamType == "OUT");
					addOutputStream(streamName, isOut);					
					break;
				}
				else if (localName == "FilterPredIndex" 
					|| localName == "FRPredIndex" || 
					  localName == "DynFilterPredIndex") {
					createAttrMap(atts, attrMap);
					_predIndexSpec.setIndexType(
						getParameterValue<string>(attrMap, "IndexType"));
					if(localName == "FilterPredIndex") {
						_state = IN_FILTER_INDEX;
					}
					else if(localName == "FRPredIndex") {
						_state = IN_FR_INDEX;
					}
					else {						
						_state = IN_DYN_FILTER_INDEX;
						_isDynIndexed = true;
					}
				}
				//luoluo 8-4
				else if (localName =="PREDWINDOW")
				{
					_PredWindowBuilder->startElement(namespaceURI,localName,qName,atts);
				}

				else {
					_state = IN_EDGE;
					_eBuilder.startElement(namespaceURI, 
						localName, 
						qName, 
						atts);
				}
				break;
			case IN_FILTER_INDEX:
			case IN_FR_INDEX:
			case IN_DYN_FILTER_INDEX:
				createAttrMap(atts, attrMap);
				indexAttrType = getParameterValue<string>(attrMap, 
					"Type");
				eventAttrName = getParameterValue<string>(attrMap, 
					"EvAttrName");
				nodeAttrName = getParameterValue<string>(attrMap, 
					"NodeAttrName", 
					"");				
				addPredIndexSpecEntry(indexAttrType, 
					eventAttrName, 
					nodeAttrName);

				break;

			case IN_SCHEMA:
				assert (localName == "AttrName");
				createAttrMap(atts, attrMap);

				attrName = getParameterValue<string>(attrMap, "Name");
				attrType = getParameterValue<string>(attrMap, "Type");
				addSchemaAttr(attrName, attrType);
				break;
			case IN_EDGE:
				_eBuilder.startElement(namespaceURI, localName, qName, atts);
				break;	
		}
	}
	catch (XMLException& e) {
		cerr<<e.getMessage();
	}
	
}

void NodeBuilder::endElement(const std::string& namespaceURI, 
							 const std::string& localName,
							 const std::string& qName) {
	assert(localName.length());
	if (_state == IN_SCHEMA && localName == "Schema") {
		_state = IN_NODE;
	}
	else if (_state == IN_FILTER_INDEX && localName == "FilterPredIndex") {
		addOnePredIndexSpec(FILTER, STATIC);
		_state = IN_NODE;
	}
	else if (_state == IN_FR_INDEX && localName == "FRPredIndex") {
		//actually stands for FR here
		addOnePredIndexSpec(FORWARD, STATIC); 
		_state = IN_NODE;
	}
	else if (_state == IN_DYN_FILTER_INDEX && localName == "DynFilterPredIndex") {		
		addOnePredIndexSpec(FILTER, DYNAMIC);
		_state = IN_NODE;
	}
	else if (localName == "FilterEdge" 
		|| localName == "RebindEdge" 
		|| localName == "ForwardEdge") {
		EdgeString* pEdge = _eBuilder.getEdge();
		addEdge(pEdge);
		_state = IN_NODE;
	}
	//luoluo 8-4
	else if (localName == "PREDWINDOW")
	{
		predWindowString * ptr = _PredWindowBuilder->getPredWindow();
		_pThisNode->setRationalPredWindow(ptr);
        _state = IN_NODE;
	}

	else _eBuilder.endElement(namespaceURI, localName, qName);
}


void NodeBuilder::createNode(const StringMap& attrMap) {
	string strNodeType = getParameterValue<string>(attrMap, "NodeType");
	if (strNodeType == "START") {
		_pThisNode = new StartNodeString(attrMap);
	}
	else {
		assert (strNodeType == "INTERMEDIATE_OR_END");
		_pThisNode = new IntermediateNodeString(attrMap);
	}
}

void NodeBuilder::addOutputStream(const string& streamName, const bool isOut) {
	_pThisNode->addOutputStream(streamName, isOut);
}


void NodeBuilder::addSchemaAttr(const string& attrName,
								const string& attrType) {
	_pThisNode->addSchemaAttr(attrName, attrType);
}

void NodeBuilder::addEdge(EdgeString* pEdge) {
	_pThisNode->addEdge(pEdge);
}

void NodeBuilder::addPredIndexSpecEntry(const string& indexType,
										const string&  attr,
										const string&  nodeAttr) {
	_predIndexSpec.push_back(PredIndexSpecEntryString(indexType, 
		attr, 
		nodeAttr));
}


void NodeBuilder::addOnePredIndexSpec(const EdgeType& eType,
									  const IndexType& iType) {
	if (eType == FILTER) {
		if(iType == STATIC) {
			_filterPredIndices.push_back(_predIndexSpec);
		}
		else
		{
			_dynFilterPredIndex = _predIndexSpec;			
		}
	}
	else {
		assert (eType == FORWARD);
		assert (iType == STATIC);
		_frPredIndices.push_back(_predIndexSpec);
	}
	_predIndexSpec.clear();
}

void NodeBuilder::reset() {
	_pThisNode = NULL;
	_isDynIndexed=false;
	_filterPredIndices.clear();
	_frPredIndices.clear();
	_predIndexSpec.clear();
	_dynFilterPredIndex.clear();
}


NodeString* NodeBuilder::getNode() {
	_state = INIT;
	addPredIndexSpec();
	NodeString* pNode = _pThisNode;
	reset();
	return pNode;
}

void NodeBuilder::addPredIndexSpec() {
	_pThisNode->addPredIndexSpec(_filterPredIndices, _frPredIndices,
								_dynFilterPredIndex, _isDynIndexed);
}
