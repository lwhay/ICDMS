/**
 * @file ByteCodeUtil.cpp
 * @date April 15, 2006
 * @brief Defines a few utility functiosn for byte codes.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "BasicDataStructure/ByteCode.h"
using namespace Cayuga::BasicDataStructure::ByteCodeNS;

#include "BasicDataStructure/Predicate/AttributeID.h"
using namespace Cayuga::BasicDataStructure::Predicate;

#include "BasicDataStructure/Automaton/IntermediateNode.h"
#include "BasicDataStructure/Automaton/ForwardEdge.h"

#include "AutomatonCompiler/ByteCodeUtils.h"
using namespace Cayuga::AutomatonCompiler;

#include "SystemMgr/DataTypeMgr.h"
#include "SystemMgr/StreamSchemaMgr.h"
#include "SystemMgr/FuncMgr.h"
using namespace Cayuga::SystemMgr;

#include "Utils/RAssert.h"
using namespace Cayuga::Utils;

#include <memory.h>
#include <cassert>
#include <iostream>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Definition of variables
////////////////////////////////////////////////////////////////////////////////


/**
* This variable stores the size of a CP instruction.
*/
const size_t Cayuga::AutomatonCompiler::g_sizeCPInstruction = 
	sizeof(ByteCode)+sizeof(size_t)+3*sizeof(char*);

/**
* This variable stores the size of a PARAM instruction.
*/
const size_t Cayuga::AutomatonCompiler::g_sizePARAMInstruction = 
	sizeof(ByteCode)+sizeof(ByteCode)+2*sizeof(size_t);

/**
* This variable stores the size of an EVAL instruction.
*/
const size_t Cayuga::AutomatonCompiler::g_sizeEVALInstruction 
= sizeof(ByteCode)+sizeof(size_t) + 2*sizeof(const char*);


////////////////////////////////////////////////////////////////////////////////
// Definition of utility functions
////////////////////////////////////////////////////////////////////////////////

ByteCodeNS::ByteCode Cayuga::AutomatonCompiler::combineDataType(
	ByteCode lhs, ByteCode rhs) {
	if (lhs == ByteCodeNS::STRING) {
		Assert (rhs == ByteCodeNS::STRING);
		return ByteCodeNS::STRING;
	}
	if (lhs == ByteCodeNS::INT) {
		Assert (rhs == ByteCodeNS::INT || rhs == ByteCodeNS::FLOAT);
		return rhs;
	}
	else {
		Assert (lhs == ByteCodeNS::FLOAT);
		Assert (rhs == ByteCodeNS::INT || rhs == ByteCodeNS::FLOAT);
		return ByteCodeNS::FLOAT;
	}
}

string Cayuga::AutomatonCompiler::convertByteCodeTypeToString(
	ByteCodeNS::ByteCode type) {
	if (type == ByteCodeNS::INT)
			return "int";
	else if (type == ByteCodeNS::FLOAT)
		return "float";
	else {
		Assert (type == ByteCodeNS::STRING);
		return "string";
	}
}


ByteCodeNS::ByteCode Cayuga::AutomatonCompiler::convertStringToByteCodeType(
	const string& str) {
	if (str == "int")
		return ByteCodeNS::INT;
	else if (str == "float")
		return ByteCodeNS::FLOAT;
	else if (str == "string") 
		return ByteCodeNS::STRING;

	Assert(false);
	return ByteCodeNS::INVALID;
}

void Cayuga::AutomatonCompiler::overwriteArrayFieldInCPInstruction(
	char* program, 
	const char* ev, 
	const char* srcInstanceArray, 
	char* destInstanceArray) {
	//can rewrite the logic in overwriteArrayFieldInEVALInstruction,
	//since the prefix structure of both instructions is the same
	//(i.e., offset of ev and srcInstanceArray)
	overwriteArrayFieldInEVALInstruction(program, ev, srcInstanceArray);

	size_t offset = sizeof(ByteCode)+sizeof(size_t)+2*sizeof(char*);
	setValToProgram<char*>(program, offset, destInstanceArray);
}


void Cayuga::AutomatonCompiler::overwriteArrayFieldInEVALInstruction(
	char* program, 
	const char* ev, 
	const char* srcInstanceArray) {
	size_t offset = sizeof(ByteCode)+sizeof(size_t);
	setValToProgram<const char*>(program, offset, ev);
	setValToProgram<const char*>(program, offset, srcInstanceArray);
}

ByteCode Cayuga::AutomatonCompiler::convertDataTypeNameToByteCode(
	const string& typeName) {
	ByteCode dataType;
	if (typeName == "int") {
		dataType = INT;
	}
	else if (typeName == "float") {
		dataType = FLOAT;
	}
	else if (typeName == "string") {
		dataType = STRING;
	}
	else if (typeName == "shortstring") {
		dataType = SHORT_STRING;
	}
	else if (typeName == "SAXEventType") {
		dataType = SAX_EVENT_TYPE;
	}
	else {
		assert (false);
	}
	return dataType;
}


size_t Cayuga::AutomatonCompiler::g_szAttrInstruction = 
	sizeof(ByteCode)*2 + sizeof(size_t);
size_t Cayuga::AutomatonCompiler::g_szOpInstruction = 
	sizeof(ByteCode)*2;
size_t Cayuga::AutomatonCompiler::g_szIntConstInstruction = 
	sizeof(ByteCode)*2 + sizeof(int);


void Cayuga::AutomatonCompiler::makeAttrInstruction(char* instruction, 
						 size_t& offset,
						 ByteCode attrSrc,
						 ByteCode attrType,
						 size_t attrOffset) {
	setValToProgram<ByteCode>(instruction, offset, attrSrc);
	setValToProgram<ByteCode>(instruction, offset, attrType);
	setValToProgram<size_t>(instruction, offset, attrOffset);
	
}

void Cayuga::AutomatonCompiler::makeOpInstruction(char* instruction, 
						 size_t& offset,
						 ByteCode opType,
						 ByteCode dataType) {
	setValToProgram<ByteCode>(instruction, offset, opType);
	setValToProgram<ByteCode>(instruction, offset, dataType);	
}

template<typename T>
void Cayuga::AutomatonCompiler::makeConstInstruction(char* instruction, 
						 size_t& offset,
						 ByteCodeNS::ByteCode dataType,
						 const T& val) {
	setValToProgram<ByteCodeNS::ByteCode>(instruction, offset, 
		ByteCodeNS::CONSTANT);
	setValToProgram<ByteCodeNS::ByteCode>(instruction, offset, dataType);
	setValToProgram<T>(instruction, offset, val);
}

bool Cayuga::AutomatonCompiler::isSameProgram(char* lhs, char* rhs) {
	ByteCode lhs_bcc = *(ByteCode*)lhs;
	ByteCode rhs_bcc = *(ByteCode*)rhs;

	if (lhs_bcc != rhs_bcc) {
		return false;
	}

	assert (lhs_bcc == CP);
	size_t lhs_num_chunks = *(size_t*)(lhs+sizeof(ByteCode));
	size_t rhs_num_chunks = *(size_t*)(rhs+sizeof(ByteCode));
	
	if(lhs_num_chunks != rhs_num_chunks) {
		return false;
	}

	size_t paramInstructionLen = lhs_num_chunks*g_sizePARAMInstruction;

	//compare the param instructions
	if (memcmp(lhs+g_sizeCPInstruction, 
		rhs+g_sizeCPInstruction, 
		paramInstructionLen)) {
		return false;
	}

	return true;
} 


char* Cayuga::AutomatonCompiler::makePARAMInstruction(
	const ByteCode& attrSrc, 
						   const size_t offset, 
						   const size_t len) {
	char* instruction = new char[g_sizePARAMInstruction];
	size_t ip = 0; //instruction pointer
	
	setValToProgram<ByteCode>(instruction, ip, PARAM);
	setValToProgram<ByteCode>(instruction, ip, attrSrc);
	setValToProgram<size_t>(instruction, ip, offset);
	setValToProgram<size_t>(instruction, ip, len);

	return instruction;
}


void Cayuga::AutomatonCompiler::getEventAttrInfo(const StreamID& streamID,
				const string& eventAttr,
				size_t& attrOffset,
				size_t& attrLen,
				ByteCode& attrDataType) {
	AttributeID srcAttrID = StreamSchemaMgr::getInstance()->
		getStreamAttrID(streamID, eventAttr);

	attrOffset = StreamSchemaMgr::getInstance()->
		getInternalAttributeOffset(streamID, srcAttrID);

	AttrTypeID srcAttrTypeID = StreamSchemaMgr::getInstance()->
		getStreamAttrType(streamID, srcAttrID);

	const string& attrTypeName 
		= DataTypeMgr::getInstance()->getName(srcAttrTypeID);
	attrDataType = convertDataTypeNameToByteCode(attrTypeName);

	attrLen = DataTypeMgr::getInstance()->
		getInternalLength(srcAttrTypeID);
}

char* Cayuga::AutomatonCompiler::eventAttr2Instruction(const StreamID& streamID,
							const string& eventAttr,
							size_t& indexInputLengthOut) {
	
	size_t srcAttrOffset, srcAttrLen;
	ByteCode notUsed;
	getEventAttrInfo(streamID, eventAttr, srcAttrOffset, srcAttrLen, notUsed);
		
	indexInputLengthOut += srcAttrLen;
	ByteCode attrSrc = EVENT;

	return makePARAMInstruction(attrSrc, srcAttrOffset, srcAttrLen);
}


void Cayuga::AutomatonCompiler::getNodeAttrInfo(const Node* pSourceNode,
					  const string& attrName,
					  size_t& attrOffset, 
					  size_t& attrLen) {
	  IntermediateNode* pNewSourceNode = 
		  (IntermediateNode*)pSourceNode;
	  const StreamSchema* pSourceSchema = 
		  pNewSourceNode->getNodeSchema();

	  AttributeID srcAttrID = pSourceSchema->
		  getID(attrName);

	  attrOffset = pSourceSchema->
		  getInternalAttributeOffset(srcAttrID);

	  AttrTypeID srcAttrTypeID = pSourceSchema->
		  getAttributeType(srcAttrID);

	  attrLen = DataTypeMgr::getInstance()->
		  getInternalLength(srcAttrTypeID);
}


char* Cayuga::AutomatonCompiler::copyEventAttrToNodeAttr(
	const StreamID streamID,
	const string& eventAttr,
	size_t& thisIp,
	size_t& szInstr, 
	size_t& attrLen) {
	ByteCode attrDataType;

	szInstr = sizeof(ByteCode)*2+g_szAttrInstruction;
	char* instruction = new char[szInstr];
	setValToProgram<ByteCode>(instruction, thisIp, AGG);
	size_t evAttrOffset;
	getEventAttrInfo(streamID, 
		eventAttr, 
		evAttrOffset, 
		attrLen,
		attrDataType);
	makeAttrInstruction(instruction, 
		thisIp,
		EVENT,
		attrDataType,
		evAttrOffset);

	return instruction;
}

char* Cayuga::AutomatonCompiler::aggEventNodeAttrToNodeAttr(
	const StreamID streamID,
	const string& eventAttr,
	size_t& thisIp,
	size_t& szInstr, 
	size_t& attrLen,
	const Node* pSourceNode,
	const string& nodeAttr,
	const ByteCode opType) {
	ByteCode attrDataType;

	szInstr = sizeof(ByteCode)*2
	  +g_szAttrInstruction
	  +g_szAttrInstruction
	  +g_szOpInstruction;
	char* instruction = new char[szInstr];
	setValToProgram<ByteCode>(instruction, thisIp, AGG);

	size_t srcAttrOffset;
	getNodeAttrInfo(pSourceNode, 
	  nodeAttr, 
	  srcAttrOffset, 
	  attrLen);

	size_t evAttrOffset, evAttrLen;
	getEventAttrInfo(streamID, 
	  eventAttr, 
	  evAttrOffset, 
	  evAttrLen,
	  attrDataType);

	makeAttrInstruction(instruction, 
	  thisIp,
	  NODE,
	  attrDataType,
	  srcAttrOffset);

	makeAttrInstruction(instruction, 
	  thisIp, 
	  EVENT, 
	  attrDataType,
	  evAttrOffset);

	makeOpInstruction(instruction, 
	  thisIp,
	  opType,
	  attrDataType);

	return instruction;
}

/**
* @brief Make a program for the UDF.
* @param funcName The name of the function.
* @param szInstr The size of the instruction. Output parameter.
* @param ip The pointer to the instruction. Input/Output parameter.
* @return Pointer to the construction instruction.
* @author Mingsheng Hong (mshong@cs.cornell.edu)
* @date September 20, 2006
* @version 1.0
*/
char* Cayuga::AutomatonCompiler::makeUDFProgram(const string& funcName, 
												size_t& szInstr, 
												size_t& ip) {
	const FuncEntry& fe = FuncMgr::getInstance()->getEntry(funcName);

	UDF_Ptr fp = fe.fp;
	int numArgs = FuncMgr::getInstance()->getNumArgs(funcName);

	//a UDF instruction looks as follows:
	//call fp retType #args arg1Type arg2Type
	//arg1Type appears only if #args > 0, and similarly arg2Type appears
	//only if #args > 1 (=2 in this case as we require)
	szInstr = sizeof(ByteCodeNS::ByteCode)*2 + sizeof(void*) + sizeof(int)
		+ numArgs*sizeof(ByteCodeNS::ByteCode); 

	char* pInstruction = new char[szInstr];
	
	setValToProgram<ByteCodeNS::ByteCode>
		(pInstruction, ip, ByteCodeNS::CALL);

	setValToProgram<UDF_Ptr>(pInstruction, ip, fp);

	setValToProgram<ByteCodeNS::ByteCode>
		(pInstruction, ip, fe.returnType);

	setValToProgram<int>(pInstruction, ip, numArgs);

	if (numArgs > 0) {
		setValToProgram<ByteCodeNS::ByteCode>
		(pInstruction, ip, fe.inputArg1Type);
	}

	if (numArgs > 1) {
		assert (numArgs == 2);
		setValToProgram<ByteCodeNS::ByteCode>
		(pInstruction, ip, fe.inputArg2Type);
	}

	return pInstruction;
}


Edge* Cayuga::AutomatonCompiler::makeEdge(EdgeType et, 
										  EdgeID nextForwardEdgeID) {
	switch (et) {
		case FILTER:
			return new FilterEdge(FILTER_EDGE_ID);
		case REBIND:
			return new RebindEdge(REBIND_EDGE_ID);
		case FORWARD:
			return new ForwardEdge(nextForwardEdgeID); 
		default:
			assert(false);
			return NULL;
	}
}

//explicit instantiation
template
void Cayuga::AutomatonCompiler::makeConstInstruction<int>(char* instruction, 
						 size_t& offset,
						 ByteCodeNS::ByteCode dataType,
						 const int& val);
