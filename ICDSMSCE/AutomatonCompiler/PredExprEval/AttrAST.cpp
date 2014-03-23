/**
 * @file AttrAST.cpp
 * @date November 30, 2005
 * @brief This file defines class AttrAST.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "AutomatonCompiler/PredExprEval/AttrAST.h"
using namespace Cayuga::AutomatonCompiler::PredExprEval;

#include "AutomatonCompiler/ByteCodeUtils.h"
#include "AutomatonCompiler/Context.h"
#include "AutomatonCompiler/SchemaMapString.h"
using namespace Cayuga::AutomatonCompiler;

#include "BasicDataStructure/Automaton/IntermediateNode.h"
using namespace Cayuga::BasicDataStructure::Automaton;

#include "SystemMgr/DataTypeMgr.h"
#include "SystemMgr/FuncMgr.h"
#include "SystemMgr/StreamSchemaMgr.h"
using namespace Cayuga::SystemMgr;

#include "Utils/RAssert.h"
using namespace Cayuga::Utils;

#include <string>
using namespace std;


void AttrAST::initialize( RefToken t ) {
	BasicAST::initialize(t);
	//_attrName = t->getText();

	_AttrSource = ByteCodeNS::INVALID;
}

RefAST AttrAST::clone( void ) {
	RefAST ret = new AttrAST(*this);
	return ret;
}

RefAST AttrAST::factory( void ) {
	RefAST ret = new AttrAST();
	return ret;
}

void AttrAST::_getAttrSource(Context& context) {
	//no source annotation: will have to try our luck
	if (getFirstChild() == NULL)  {
		try {
			StreamSchemaMgr::getInstance()->
				getStreamSchema(context._streamID)->getID(_getAttrName());
			_AttrSource = ByteCodeNS::EVENT; //could be node, will edit later
			return;
		}
		catch( ... ) {
			//not in the event stream schema, so must be in the source node
			//schema!
			Assert (typeid(*context._pSourceNode) == typeid(IntermediateNode));
			_AttrSource  = ByteCodeNS::NODE; //could be node, will edit later
			return;
		}
	}

	//have source annotation
	if (getText() == "EV") {
		_AttrSource = ByteCodeNS::EVENT;
	}
	else if (getText() == "NODE") {
		_AttrSource = ByteCodeNS::NODE;
	}
	else {
		throw string(
			"attribute source annotation incorrect in complex pred");
	}
}

string AttrAST::_getAttrName() {
	//skip annotation
	if (getFirstChild() != NULL) {
		RefAST childAST = getFirstChild();
		Assert (childAST && childAST->getText() == ".");

		childAST = childAST->getNextSibling();
		return childAST->getText();
	}

	return getText();
}

void AttrAST::_setAttrName(const string& attrName) {
	//skip annotation
	if (getFirstChild() != NULL) {
		RefAST childAST = getFirstChild();
		Assert (childAST && childAST->getText() == ".");

		childAST = childAST->getNextSibling();
		childAST->setText(attrName);
		return;
	}
	
	setText(attrName);
}

void AttrAST::getAttrTypeFromSchema(Context& context) {
	_getAttrSource(context);

	const StreamSchema* pStreamSchema;
	if (_AttrSource == ByteCodeNS::EVENT) {
		pStreamSchema = StreamSchemaMgr::getInstance()->
			getStreamSchema(context._streamID);
	}
	else {
		Assert (_AttrSource == ByteCodeNS::NODE);
		Assert (typeid(*context._pSourceNode) == typeid(IntermediateNode));
		pStreamSchema = ((IntermediateNode*)context._pSourceNode)
			->getNodeSchema();
	}

	_attrID = pStreamSchema->getID(_getAttrName());
	_offset = pStreamSchema->
		getInternalAttributeOffset(_attrID);
	AttrTypeID attrTypeID = pStreamSchema->getAttributeType(_attrID);
	_attrTypeName = DataTypeMgr::getInstance()->getName(attrTypeID);
}

bool AttrAST::_isFuncNode() {
	return getFirstChild() && getFirstChild()->getText() == "(";
}

void AttrAST::compile(Context& context,
					  vector<pair<char*, size_t> >& programOut) {
	if (_isFuncNode()) {
	  //if the node has children, and the first child is (, it is a func
	  compileFunction(context, programOut);
	}
	else {
	  compileAttribute(context, programOut);
	}
}

void AttrAST::compileFunction(Context& context,
							  vector<pair<char*, size_t> >& programOut) {
	string funcName = getText();
	//cout<<"compiling function"<<endl;

	//for (RefAST childAST = getFirstChild(); childAST; 
	//	childAST = childAST->getNextSibling()) {
	//	 (RefBasicAST(childAST))->compile(context, programOut);
	// }

	//To distinguish ATTR and FUNC node, we put the ( as the first child
	//of FUNC, and ATTR does not have any child
	RefAST childAST = getFirstChild();
	Assert (childAST && childAST->getText() == "(");

	childAST = childAST->getNextSibling();
	//It is possible that the function does not have any input argument
	if (childAST) {
	  (RefBasicAST(childAST))->compile(context, programOut);
	}

	//add only new instruction for the function evaluation
	//Format: CALL funcPtr
	size_t szInstr;
	size_t ip = 0;
	char* pInstruction = makeUDFProgram(getText(), szInstr, ip);

	programOut.push_back(make_pair(pInstruction, szInstr));

	if (getNextSibling()) {
	  (RefBasicAST(getNextSibling()))->compile(context, programOut);
	}

}

void AttrAST::compileAttribute(Context& context,
							   vector<pair<char*, size_t> >& programOut) {
	getAttrTypeFromSchema(context);

	/*
	#ifdef _DEBUG
	cout<<_attrTypeName;
	cout<<" ATTR"<<endl;
	#endif //_DEBUG
	*/

	ByteCodeNS::ByteCode attrType = 
		convertDataTypeNameToByteCode(_attrTypeName);

	//source + offset in the array
	size_t szInstr = sizeof(ByteCodeNS::ByteCode)*2 + sizeof(unsigned); 
	char* pInstruction = new char[szInstr];
	size_t ip = 0;
	makeAttrInstruction(pInstruction, ip, _AttrSource, attrType, _offset);

	programOut.push_back(make_pair(pInstruction, szInstr));

	if (getNextSibling()) {
	   (RefBasicAST(getNextSibling()))->compile(context, programOut);
	}
}

ByteCodeNS::ByteCode AttrAST::getType(Context& context) {
	//attr node
	if (!_isFuncNode()) {
		getAttrTypeFromSchema(context);

		ByteCodeNS::ByteCode lhsType 
			= convertDataTypeNameToByteCode(_attrTypeName);

		if (!getNextSibling()) {
			return lhsType;
		}

		ByteCodeNS::ByteCode rhsType 
			= (RefBasicAST(getNextSibling()))->getType(context);

		return combineDataType(lhsType, rhsType);
	}
	else {
		//func node
		return FuncMgr::getInstance()->getReturnType(getText());

	}
}

void AttrAST::renameAttrRef(const SchemaMapString* schemaMap) {
	//passing uninitialized context is incorrect in general, 
	//but since this function will only be invoked if this complex pred is 
	//generated by CEL compiler, we know each UDF attribute is annotated with
	//attribute source. Therefore this should be fine.
	if (!_isFuncNode()) {
		Context context;
		_getAttrSource(context);
		string attrName = _getAttrName();
		bool bFromEvent;
		string newAttrName;
		bool success = schemaMap->renameAttrRefAndSource(
			attrName, newAttrName, bFromEvent);

		if (_AttrSource == ByteCodeNS::EVENT) {
			if (success) {
				_setAttrName(newAttrName);
				_AttrSource = (bFromEvent) ? 
					ByteCodeNS::EVENT : ByteCodeNS::NODE;
			}
		}
		else {
			Assert (_AttrSource == ByteCodeNS::NODE);
			//schema map cannot change attributes coming from node
			Assert (success == false);
		}
	}
	else {
		//handle function node
		RefAST childAST = getFirstChild();
		Assert (childAST && childAST->getText() == "(");

		childAST = childAST->getNextSibling();
		//It is possible that the function does not have any input argument
		while (childAST) {
			(RefBasicAST(childAST))->renameAttrRef(schemaMap);
			childAST = childAST->getNextSibling();
		}
	}

	//if (getNextSibling()) {
	//	(RefBasicAST(getNextSibling()))->renameAttrRef(schemaMap);
	//}
}

void AttrAST::getReferredAttributes(set<string>& referredAttrs) {
	if (!_isFuncNode()) {
		referredAttrs.insert(_getAttrName());
	}
	else {
		//handle function node
		RefAST childAST = getFirstChild();
		Assert (childAST && childAST->getText() == "(");

		childAST = childAST->getNextSibling();
		//It is possible that the function does not have any input argument
		while (childAST) {
			(RefBasicAST(childAST))->getReferredAttributes(referredAttrs);
			childAST = childAST->getNextSibling();
		}
		//if (getNextSibling()) {
		//	(RefBasicAST(getNextSibling()))->
		//		getReferredAttributes(referredAttrs);
		//}
	}

	//if (getNextSibling()) {
	//	(RefBasicAST(getNextSibling()))->
	//		getReferredAttributes(referredAttrs);
	//}
}

void AttrAST::write(ostream& out) {
	if (!_isFuncNode()) {
		Context context;
		string attrName = _getAttrName();
		if (_AttrSource == ByteCodeNS::EVENT) {
			out<<"EV.";
		}
		else {
			Assert (_AttrSource == ByteCodeNS::NODE);
			out<<"NODE.";
		}
		out<<attrName;
	}
	else {
		//write function name
		out<<getText();
		out<<"(";

		//handle function node
		RefAST childAST = getFirstChild();
		Assert (childAST && childAST->getText() == "(");

		childAST = childAST->getNextSibling();
		//It is possible that the function does not have any input argument
		while (childAST) {
			(RefBasicAST(childAST))->write(out);
			childAST = childAST->getNextSibling();
			if (childAST) {
				out<<", ";
			}
		}
		
		out<<")";

	}
}
