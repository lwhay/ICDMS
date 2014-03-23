/**
 * @file ByteCodeUtils.h
 * @date April 25, 2006
 * @brief Declares a set of functions that general common byte codes.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */


#ifndef _COMMON_BYTE_CODE_H
#define _COMMON_BYTE_CODE_H

#include "BasicDataStructure/ByteCode.h"
#include "BasicDataStructure/StreamID.h"
using namespace Cayuga::BasicDataStructure;

#include "BasicDataStructure/Automaton/EdgeID.h"
#include "BasicDataStructure/Automaton/EdgeType.h"
using namespace Cayuga::BasicDataStructure::Automaton;


#include <string>
using namespace std;

namespace Cayuga {
namespace BasicDataStructure {
namespace Automaton {
class Node;
class Edge;
}
}
using Cayuga::BasicDataStructure::Automaton::Node;

namespace AutomatonCompiler {

extern size_t g_szAttrInstruction;
extern size_t g_szOpInstruction;
extern size_t g_szIntConstInstruction;

//utilities for manipulating instructions
extern const size_t g_sizeCPInstruction;
extern const size_t g_sizePARAMInstruction;
extern const size_t g_sizeEVALInstruction;


/**
* @brief Overwrite the last three fields in the first instruction (CP) of 
		 program.
* @param program Pointer to the program
* @param ev Pointer to the current event content
* @param srcInstanceArray Pointer to the current instance content
* @param destInstanceArray Pointer to the destination instance content
* @author Mingsheng Hong (mshong@cs.cornell.edu)
* @date November 30, 2005
* @version 1.0
*/
void overwriteArrayFieldInCPInstruction(char* program, 
										const char* ev, 
										const char* srcInstanceArray, 
										char* destInstanceArray);

/**
* @brief Overwrite the first two fields in the first instruction (EVAL) of 
		 program.
* @param program Pointer to the program
* @param ev Pointer to the current event content
* @param srcInstanceArray Pointer to the current instance content
* @author Mingsheng Hong (mshong@cs.cornell.edu)
* @date November 30, 2005
* @version 1.0
*/
void overwriteArrayFieldInEVALInstruction(char* program, 
										const char* ev, 
										const char* srcInstanceArray);


/**
* @brief Get value of type T from the byte code program
* @param program Pointer to the program
* @param offset Will be modified to offset + sizeof(T) after thie function call
* @return The extracted value from the program
* @author Mingsheng Hong (mshong@cs.cornell.edu)
* @date November 30, 2005
* @version 1.0
*/
template <typename T>
T getValFromProgram(const char* program, size_t& offset) {
	T tmp =  *(T*)(program+offset);
	offset += sizeof(T);
	return tmp;
}

/**
* @brief Set value of type T to the byte code program
* @param program Pointer to the program
* @param offset Will be modified to offset + sizeof(T) after thie function call
* @param val Value of type T to set in the program
* @author Mingsheng Hong (mshong@cs.cornell.edu)
* @date November 30, 2005
* @version 1.0
*/
template <typename T>
void setValToProgram(char* program, size_t& offset, const T& val) {
	*(T*)(program+offset) = val;
	offset += sizeof(T);
}

/**
 * @brief Conver attribute type name to a ByteCode object.
 * @param typeName The input attribute type name. E.g. int, float, string
 * @return The ByteCode object to return. E.g. INT, FLOAT, STRING
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date April 15, 2006
 * @version 1.0
 */
ByteCodeNS::ByteCode convertDataTypeNameToByteCode(const string& typeName);

/**
* @brief Make an EVENT attr instruction for Cayuga engine interpreter from the 
		 input parameters
* @param streamID The input stream ID
* @param eventAttr The name of the attribute from the input stream
* @param indexInputLengthOut Input/output variable. Incremented it by the size 
		 of the attribute in each event.
* @return The interpreter instruction made.
* @author Mingsheng Hong (mshong@cs.cornell.edu)
* @date November 30, 2005
* @version 1.0
*/
char* eventAttr2Instruction(const StreamID& streamID,
							const string& eventAttr,
							size_t& indexInputLengthOut);

/**
* @brief Make an attribute instruction for Cayuga engine interpreter from the 
		 input parameters
* @param instruction The input instruction program to append to
* @param offset Input/output variable. Incremented it by the size 
		 of this instruction, which is 2*sizeof(ByteCode)+sizeof(size_t)
* @param attrSrc Source of the attribute.
* @param attrType Data type of the attribute.
* @param attrOffset Offset of the attribute in the data array.
* @author Mingsheng Hong (mshong@cs.cornell.edu)
* @date November 30, 2005
* @version 1.0
*/
void makeAttrInstruction(char* instruction, 
						 size_t& offset,
						 ByteCodeNS::ByteCode attrSrc,
						 ByteCodeNS::ByteCode attrType,
						 size_t attrOffset);

/**
* @brief Make an operator instruction for Cayuga engine interpreter from the 
		 input parameters
* @param instruction The input instruction program to append to
* @param offset Input/output variable. Incremented it by the size 
		 of this instruction, which is 2*sizeof(ByteCode)
* @param opType Type of the operator.
* @param dataType Data type of the operator.
* @author Mingsheng Hong (mshong@cs.cornell.edu)
* @date November 30, 2005
* @version 1.0
*/
void makeOpInstruction(char* instruction, 
						 size_t& offset,
						 ByteCodeNS::ByteCode opType,
						 ByteCodeNS::ByteCode dataType);

/**
* @brief Make a constant instruction for Cayuga engine interpreter from the 
		 input parameters
* @param instruction The input instruction program to append to
* @param offset Input/output variable. Incremented it by the size 
		 of this instruction, which is 2*sizeof(ByteCode)
* @param dataType Data type of the constant.
* @param val Value of the constant.
* @author Mingsheng Hong (mshong@cs.cornell.edu)
* @date November 30, 2005
* @version 1.0
*/
template<typename T>
void makeConstInstruction(char* instruction, 
						 size_t& offset,
						 ByteCodeNS::ByteCode dataType,
						 const T& val);

/**
* @brief Make a PARAM instruction for Cayuga engine interpreter from the 
		 input parameters
* @param attrSrc The source of this attribute
* @param offset The offset of this parameter in the input data array
* @param len The length of this parameter in the input data array
* @return The interpreter instruction made.
* @author Mingsheng Hong (mshong@cs.cornell.edu)
* @date November 30, 2005
* @version 1.0
*/
char* makePARAMInstruction(const ByteCodeNS::ByteCode& attrSrc, 
						   const size_t offset, 
						   const size_t len);

/**
* @brief Compared the two input programs
* @param lhs The first input program
* @param rhs The second input program
* @return True iff the two programs are the same
* @author Mingsheng Hong (mshong@cs.cornell.edu)
* @date November 30, 2005
* @version 1.0
*/
bool isSameProgram(char* lhs, char* rhs);

/**
* @brief Given the input stream ID and attribute name, find out its 
		 information in the compiled schema: offset, length and data type
* @param streamID The input stream ID
* @param eventAttr The input attribute name
* @param attrOffset Output variable. Store the offset of this attribute in the 
		      stream schema.
* @param attrLen Output variable. Store the length of this attribute in the 
		      stream schema.
* @param attrDataType Output variable. Store the data type of this attribute 
			  in the stream schema.
* @author Mingsheng Hong (mshong@cs.cornell.edu)
* @date November 30, 2005
* @version 1.0
*/
void getEventAttrInfo(const StreamID& streamID,
	const string& eventAttr,
	size_t& attrOffset,
	size_t& attrLen,
	ByteCodeNS::ByteCode& attrDataType);


/**
* @brief Given the input automaton node and attribute name, find out its 
		 information in the compiled node schema: offset and length
* @param pSourceNode The input automaton node
* @param attrName The input attribute name
* @param attrOffset Output variable. Store the offset of this attribute in the 
		      node schema.
* @param attrLen Output variable. Store the length of this attribute in the 
		      node schema.
* @author Mingsheng Hong (mshong@cs.cornell.edu)
* @date November 30, 2005
* @version 1.0
*/
void getNodeAttrInfo(const Node* pSourceNode,
					  const string& attrName,
					  size_t& attrOffset, 
					  size_t& attrLen);


/**
* @brief Generate an AGG instruction that copies event attribute to node
	     attribute.
* @param streamID The ID of the input event stream.
* @param eventAttr The name of the attribute in event schema to copy from.
* @param thisIp The pointer to the instruction. Will change the copy.
* @param szInstr Instruction length. Will be set by the function.
* @param attrLen Length of the attribute to copy. 
	     Will be set by the function.
* @return Pointer to the construction instruction.
* @author Mingsheng Hong (mshong@cs.cornell.edu)
* @date April 26, 2006
* @version 1.0
*/
char* copyEventAttrToNodeAttr(const StreamID streamID,
							  const string& eventAttr,
							  size_t& thisIp,
							  size_t& szInstr, 
							  size_t& attrLen);

/**
* @brief Generate an AGG instruction that aggregates event attribute and source
		 node attribute and stores the result in target node attribute.
* @param streamID The ID of the input event stream.
* @param eventAttr The name of the attribute in event schema.
* @param thisIp The pointer to the instruction. Will change the copy.
* @param szInstr Instruction length. Will be set by the function.
* @param attrLen Length of the attribute to copy. 
	     Will be set by the function.
* @param pSourceNode The pointer to the source node.
* @param nodeAttr The name of the attribute in source node schema.
* @param opType The aggregate operator.
* @return Pointer to the construction instruction.
* @author Mingsheng Hong (mshong@cs.cornell.edu)
* @date April 26, 2006
* @version 1.0
*/
char* aggEventNodeAttrToNodeAttr(const StreamID streamID,
							  const string& eventAttr,
							  size_t& thisIp,
							  size_t& szInstr, 
							  size_t& attrLen,
							  const Node* pSourceNode,
							  const string& nodeAttr,
							  const ByteCodeNS::ByteCode opType);

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
char* makeUDFProgram(const string& funcName, size_t& szInstr, size_t& ip);

/**
* @brief Given the input edge type, return a newly created edge.
* @param et The input edge type
* @param nextForwardEdgeID If the edge type is FORWARD, assign its ID to
		 nextForwardEdgeID.
* @return The newly created edge
* @author Mingsheng Hong (mshong@cs.cornell.edu)
* @date November 30, 2005
* @version 1.0
*/
Edge* makeEdge(EdgeType et, EdgeID nextForwardEdgeID);


/**
* @brief Given the two input data types, return a more general type. 
		 For example, given int and float, return float.
* @param lhs The first data type
* @param rhs The seconddata type
* @return As above.
* @author Mingsheng Hong (mshong@cs.cornell.edu)
* @date April 22, 2006
* @version 1.0
*/
ByteCodeNS::ByteCode combineDataType(ByteCodeNS::ByteCode lhs, 
									 ByteCodeNS::ByteCode rhs);

/**
* @brief Convert the input data type to a its string name.
* @param type The input data type
* @return As above.
* @author Mingsheng Hong (mshong@cs.cornell.edu)
* @date April 22, 2006
* @version 1.0
*/
string convertByteCodeTypeToString(ByteCodeNS::ByteCode type);

/**
* @brief Convert the input data type string name to its enum type.
* @param str The input data type name
* @return As above.
* @author Mingsheng Hong (mshong@cs.cornell.edu)
* @date April 22, 2006
* @version 1.0
*/
ByteCodeNS::ByteCode convertStringToByteCodeType(const string& str);

}  //namespace AutomatonCompiler
}  //namespace Cayuga

#endif //_COMMON_BYTE_CODE_H
