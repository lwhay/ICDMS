/**
 * @file ByteCode.h
 * @date November 17, 2005
 * @brief Definition of a stream ID.
		  
		  Put in a separate file to make it easy for other files to include it.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _BYTE_CODE_H
#define _BYTE_CODE_H

#include <string>
using namespace std;

namespace Cayuga {
namespace BasicDataStructure {
namespace ByteCodeNS {
/**
 * @brief A ByteCode object represents a byte code instruction type.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 30, 2005
 * @version 1.0
 */
enum ByteCode {
	EVAL,

	//relational comparison operators
	EQ,
	NE,
	LE,
	LT,
	GE,
	GT,

	//arithmetic operators, function invocation
	PLUS,
	MINUS,
	MUL,
	DIV,
	MOD,
	DUR,
	EXP,
	CALL, //function call 

	//Attribute source
	EVENT,
	NODE,
	CONSTANT,

	//data type
	INT,
	FLOAT,
	STRING,
	SAX_EVENT_TYPE,
	SHORT_STRING,
	BOOL,
	INVALID, //this type is used for an argument that does not exist

	//schema map
	CP,
	PARAM,
	AGG,

	//aggregate functions: SUM, COUNT, AVG, MIN, MAX
	//SUM and COUNT are implemented by PLUS
	//AVG is implemented by SUM, COUNT and DIV altogether
	MIN,
	MAX,

	//end of program.
	//used for marking the end of AGG instruction
	EOP,

	//GC. (Reserved)
	GC
}; //enum ByteCode

} //namespace ByteCodeNS
} //namespace BasicDataStructure
} //namespace Cayuga

#endif //_BYTE_CODE_H
