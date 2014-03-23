/**
 * @file EdgeBuilder.cpp
 * @date November 27, 2005
 * @brief Implementation file of EdgeBuilder class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "AutomatonCompiler/EdgeBuilder.h"

#include "AutomatonCompiler/StaAtomPredString.h"
#include "AutomatonCompiler/DynAtomPredString.h"
#include "AutomatonCompiler/DurationPredString.h"
#include "AutomatonCompiler/ComplexAtomPredString.h"

#include "AutomatonCompiler/ConjunctiveClauseString.h"
#include "AutomatonCompiler/ForwardEdgeString.h"
#include "AutomatonCompiler/Predicates_DNFString.h"
#include "AutomatonCompiler/SchemaMapString.h"
using namespace Cayuga::AutomatonCompiler;

#include "Utils/AttrMap.h"

#include <string>
using namespace std;


EdgeBuilder::EdgeBuilder() {
	reset();
}

void EdgeBuilder::startElement(const std::string& namespaceURI, 
							   const std::string& localName,
							   const std::string& qName, 
							   const AttributesString& atts) {
	try {
		StringMap attrMap;
		assert(localName.length());
		if (_state == INIT) {
			_state = IN_EDGE;

			createAttrMap(atts, attrMap);
			createEdge(localName, attrMap);
		}
		else {
			//StreamSchema newSchema;
			//string streamName;
			switch (_state) {
case IN_EDGE:
	if (localName == "EvalFunc") {
		_state = IN_EVAL_FUNC;
		createNewDNF();
	}
	else if (localName == "SchemaMap") {
		_state = IN_SCHEMA_MAP;
		assert (getEdgeType() == typeid(RebindEdgeString) 
			|| getEdgeType() == typeid(ForwardEdgeString));
	}
	break;
case IN_EVAL_FUNC:
	if (localName == "PredClause") {
		_state = IN_PRED_CLAUSE;
		createNewClause();
	}
	else {
		assert(localName == "UDA"); //or UDP maybe?
		//TODO: store UDA's name and do the packing
	}

	break;
case IN_PRED_CLAUSE:
	assert (localName == "StaticPred" 
		|| localName == "DynamicPred" 
		|| localName == "DurationPred"
		|| localName == "ComplexPred");
	createAttrMap(atts, attrMap);

	if (localName == "StaticPred" || localName == "DynamicPred") {
		string attrName = getParameterValue<string>(attrMap, "EventAttr");
		string strOP = getParameterValue<string>(attrMap, "OP");

		AtomPredString* pAtomPred;
		string rhs;
		if (localName == "StaticPred") {
			rhs = getParameterValue<string>(attrMap, "Const");

			pAtomPred 
				= new StaAtomPredString(attrName, strOP, rhs);
		}
		else {
			rhs = getParameterValue<string>(attrMap, "NodeAttr");

			pAtomPred 
				= new DynAtomPredString(attrName, strOP, rhs);
		}
		//pAtomPred = new ComplexAtomPredString(attrName + strOP + rhs);
		addAtomPred(pAtomPred);

	}
	else if (localName == "DurationPred") {

		 //int nDurNum = getParameterValue<int>(attrMap, "Num");
		string type = getParameterValue<string>(attrMap, "Type");

		 string rhs = getParameterValue<string>(attrMap, "Const");
		 string strOP = getParameterValue<string>(attrMap, "OP");

		 AtomPredString* pAtomPred 
			 = new DurationPredString(type, strOP, rhs);
		 addAtomPred(pAtomPred);
	}
	else if (localName == "ComplexPred") {
		string expr = getParameterValue<string>(attrMap, "Expr");
		AtomPredString* pAtomPred 
			= new ComplexAtomPredString(expr);
		addAtomPred(pAtomPred);

	}
	break;
case IN_UDA:
	break;
case IN_SCHEMA_MAP:
	assert (localName == "OldNewAttrPair" 
		|| localName == "Aggregate"
		|| localName == "UDF");
	createAttrMap(atts, attrMap);

	if (localName == "OldNewAttrPair") {
		string oldAttrSource = getParameterValue<string>(attrMap, "OldAttrSource");
		string oldAttrName = getParameterValue<string>(attrMap, "OldAttr");
		string newAttrName = getParameterValue<string>(attrMap, "NewAttr");
		addOldNewAttrPair(OldNewAttrPair(oldAttrSource, 
			oldAttrName, 
			newAttrName));
	}
	else if (localName == "Aggregate") {
		string aggName = getParameterValue<string>(attrMap, "Name");
		assert (SchemaMapString::isSDF(aggName));
			
		string eventAttrName = getParameterValue<string>(attrMap, "EventAttr");
		string nodeAttrName = getParameterValue<string>(attrMap, "NodeAttr");
		addOldNewAttrPair(OldNewAttrPair(aggName, 
			eventAttrName, 
			nodeAttrName,
			true));
	}
	else {
		string expr = getParameterValue<string>(attrMap, "Expr");
		string newAttrName = getParameterValue<string>(attrMap, "Attr");
		addOldNewAttrPair(OldNewAttrPair(expr, 
			"", 
			newAttrName,
			true));
	}
	break;
default:
	//cerr<<"seriously broken..."<<endl;
	assert(false);
			}	
		}
	}
	catch (XMLException& e) {
		cerr<<e.getMessage();
	}
	
}

void EdgeBuilder::endElement(const std::string& namespaceURI, 
							 const std::string& localName,
							 const std::string& qName) {
	assert(localName.length());
	if (localName == "EvalFunc") {
		_state = IN_EDGE;
	}
	else if (localName == "SchemaMap") {
		_state = IN_EDGE;
	}
	else if (localName == "Destination") {
		_state = IN_EDGE;
	}
	else if (localName == "PredClause") {
		_state = IN_EVAL_FUNC;
		addClause();
		
	}
	else if (localName == "UDA") {
		_state = IN_EVAL_FUNC;
	}
}

const type_info& EdgeBuilder::getEdgeType() {
	return typeid(*_pThisEdge);
}

void EdgeBuilder::createEdge(const string& edgeType, const StringMap& attrMap) {
	if (edgeType == "FilterEdge") {
		//_edgeType = FILTER;
		_pThisEdge = new FilterEdgeString();
	}
	else if (edgeType == "RebindEdge") {
		//_edgeType = REBIND;
		_pThisEdge = new RebindEdgeString();
		//Determines whether the edge has a DEL operator or not
		string doesDel = getParameterValue<string>(attrMap, "Type", "SIMPLE");
		bool isDel = (doesDel == "DEL");
		((RebindEdgeString*)_pThisEdge)->setDel(isDel);
	}
	else {
		assert (edgeType == "ForwardEdge");
		//_edgeType = FORWARD;
		_pThisEdge = new ForwardEdgeString();
		NodeID destNodeID = getParameterValue<NodeID>(attrMap, "Destination");
		//There is some duplication of code here. One could possibly use
		//use inheritance to remove this (del is a rebind class member).
		string doesDel = getParameterValue<string>(attrMap, "Type", "SIMPLE");
		bool isDel = (doesDel == "DEL");		
		((ForwardEdgeString*)_pThisEdge)->setDel(isDel);
		setDestNodeID(destNodeID);
	}
}

void EdgeBuilder::setDestNodeID(NodeID nid) {
	//_destNodeID can only be set once
	const type_info& edgeType = getEdgeType();
	assert (_destNodeID == BAD_NODE_ID && edgeType == typeid(ForwardEdgeString));
	_destNodeID = nid;
	((ForwardEdgeString*)_pThisEdge)->setDestNodeID(_destNodeID);
}

void EdgeBuilder::createNewDNF() {
	_pDNF = new Predicates_DNFString();
}

void EdgeBuilder::addClause() {
	_pDNF->addClause(_pClause);
}

void EdgeBuilder::createNewClause() {
	_pClause = new ConjunctiveClauseString();
}

void EdgeBuilder::addAtomPred(AtomPredString* pAtomPred) {
	_pClause->addAtomPred(pAtomPred);
}

void EdgeBuilder::addOldNewAttrPair(const OldNewAttrPair& attrPair) {
	assert (getEdgeType() == typeid(RebindEdgeString) 
		|| getEdgeType() == typeid(ForwardEdgeString));
	((RebindEdgeString*)_pThisEdge)->appendAttrInSchema(attrPair);
}

EdgeString* EdgeBuilder::getEdge() {
	setDNF();
	EdgeString* pEdge = _pThisEdge;
	reset();
	return pEdge;
}

void EdgeBuilder::setDNF() {
	_pThisEdge->setDNF(_pDNF);
}

void EdgeBuilder::reset() {
	_state = INIT;
	_pThisEdge = NULL;
	_destNodeID = BAD_NODE_ID;
	_pDNF = NULL;
	_pClause = NULL;
	}
