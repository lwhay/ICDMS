/**
 * @file Interpreter.h
 * @date November 17, 2005
 * @brief The virtual machine that interprets byte code instructions.
 
		  At each invocation, Interpreter reads in a program, and based on its 
		  content, a) evaluates prediates encode there, b) or copies bytes from 
		  event and node array to destination array for schema map.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
*/

#ifndef _INTERPRETER_H
#define _INTERPRETER_H

#include "Utils/Singleton.h"

#include "MemoryMgr/CY_IntStringFace.h"
using namespace CY_IntString;

#include "BasicDataStructure/ByteCode.h"
#include "BasicDataStructure/UnionDataType.h"
using namespace Cayuga::BasicDataStructure;

#include <stack>

namespace Cayuga {
namespace QueryProcessor {
//typedef bool (*UDF_Ptr)(char* bytes);

/**
commands are listed as follows
* EVAL num_atom_preds ev srcInstanceArray
* OP lhs rhs
* CP num_chunks ev srcInstanceArray destInstanceArray
* PARAM source offset len

EVAL is the first instruction of a program, and is followed by a number of 
atomic predicate evaluations as a conjunction. EVAL has three parameters. 
num_atom_preds, of type size_t, indicates the number of OP instructions 
afterwards. ev and srcInstanceArray are both of type const char*, and represent 
the content of event and instance bindings respectively.

Each evaluation is an OP instruction. Note that OP is not the name of an 
instruction type. Rather, it is a category of instruction types. For example, 
one instance of OP is LE_INT_EV_CONST. OP gives information on the data type 
of operands, as well as the data source of them. For example, LE_INT_EV_CONST 
indicates this instruction is comparing two integer values drawn from event 
and constant repsectively. In this case, lhs is an offset to the event array, 
and is of size_t type, while rhs is the constant of integer type.


Similar to EVAL, CP is the first instruction of a program, followed by a number 
of parameter chunk copying instructions. CP has four parameters. num_chunks 
indicates the number of PARAM instructions afterwards. ev and srcInstanceArray 
are same to the second and third parameters in EVAL.destInstanceArray is of 
type char*, and is the destination array pointer for parameter chunks to be 
copied to.

Each copying is a PARAM instruction.
*/

class Interpreter;
/**
 * @brief An Interpreter object represents a virtual machine that interprets
		  byte code instructions.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 17, 2005
 * @version 1.0
 */
class Interpreter : public Cayuga::Utils::Singleton<Interpreter> {
public:
	/**
	* @brief Default constructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 17, 2005
	* @version 1.0
	*/
	Interpreter();
	
	/**
	* @brief Execute the input program.
	* @param program The input program to execute
	* @return True iff the input is a predicate evaluation program and the 
		      result is true, or the input is some other kind of program.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 17, 2005
	* @version 1.0
	*/
	bool exec(const char* program);

private:
	const char* _ev;
	const char* _srcInstArray;
	char* _destInstArray;

	/**
	* @brief A StackElem object is an element on the Interpreter stack when
	*		 evaluating the input program.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 17, 2005
	* @version 1.0
	*/
	struct StackElem {
		ByteCodeNS::ByteCode _op;
		UnionDataType _data;
	};

	
	/**
	* @brief Evaluate the predicate encoded in this program.
	* @param program The input program to execute
	* @param ip The instruction point. Will change during program execution.
	* @return True iff the input is a predicate evaluation program and the 
		      result is true, or the input is some other kind of program.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 15, 2006
	* @version 1.0
	*/
	bool eval(const char* program, size_t& ip);

	/**
	* @brief Perform the schema mapping encoded in this program.
	* @param program The input program to execute
	* @param ip The instruction point. Will change during program execution.
	* @return True always.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 25, 2006
	* @version 1.0
	*/
	bool schemaMap(const char* program, size_t& ip);

	/**
	* @brief Read a byte code commmand from the program.
	* @param program The input program to read
	* @param ip The instruction point. Will change after command reading.
	* @param elem Information read from the command will be stored in elem.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 15, 2006
	* @version 1.0
	*/
	void readCommand(const char* program, size_t& ip, StackElem& elem);
	
	/**
	* @brief Read a typed value from the data array starting at offset.
	* @param dataArray As above.
	* @param offset As above. Will change after reading the value.
	* @param elem Depending on elem._type, set the value of elem._i 
			 or ._f or ._s from bytes read from offset.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 15, 2006
	* @version 1.0
	*/
	void readTypedValue(const char* dataArray, 
		size_t& offset,						
		UnionDataType& elem);

	/**
	* @brief Write a typed value into the data array starting at offset.
	* @param dataArray As above.
	* @param offset As above. Will change after writing the value.
	* @param elem Depending on elem._type, set the value of elem._i 
			 or ._f or ._s from bytes read from offset.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date September 20, 2006
	* @version 1.0
	*/
	void writeTypedValue(char* dataArray, 
		size_t& offset, 
		const UnionDataType& elem);

	/**
	* @brief Write the last stack elem into the data array.
	* @param dataArray As above.
	* @param offset As above. Will change after writing the value.
	* @param stack The stack from which we draw the element
	* @invariant The stack must have only one element
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date September 20, 2006
	* @version 1.0
	*/
	void writeStackElemToArray(char* dataArray, 
								  size_t& offset,
								  stack<UnionDataType>& stack);

	/**
	* @brief Get the float value slotred in the input parameter.
	* @invariant The input parameter must be of type INT or FLOAT.
	* @param data As above.
	* @return As above.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 15, 2006
	* @version 1.0
	*/
	float getFloatValue(const UnionDataType& data);

	/**
	* @brief Get the string value slotred in the input parameter.
	* @invariant The input parameter must be of type STRING.
	* @param data As above.
	* @return As above.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date September 18, 2006
	* @version 1.0
	*/
	IntStringHandle getStringValue(const UnionDataType& data);

	/**
	* @brief Get the typed value stored in the last input parameter
	* @param arg This output parameter will be set according to type and data.
	* @param type The data type of the value.
	* @param data This parameter stores the value.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date September 18, 2006
	* @version 1.0
	*/
	void getTypedValue(void* arg, const ByteCodeNS::ByteCode& type, 
								  const UnionDataType& data);

	/**
	* @brief Set the typed value stored in the first input parameter
	* @param arg This input parameter will be used to set data, 
			 according to type.
	* @param type The data type of the value.
	* @param data This output parameter will be set.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date September 18, 2006
	* @version 1.0
	*/
	void setTypedValue(const void* arg, const ByteCodeNS::ByteCode& type, 
								  UnionDataType& data);
	/**
	* @brief Compute the result based on lhs, rhs and the arithmetic or 
			 aggregate operation.
	* @param op The arithmetic or aggregate operation
	* @param lhs The left hand side value.
	* @param rhs The right hand side value.
	* @param result The computed result.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 15, 2006
	* @version 1.0
	*/
	void calculate(const StackElem& op, 
		const UnionDataType& lhs, 
		const UnionDataType& rhs, 
		UnionDataType& result);

	/**
	* @brief Compare lhs and rhs based on the input comparison operator.
	* @param op The comparison operation
	* @param lhs The left hand side value.
	* @param rhs The right hand side value.
	* @return The boolean answer based on evaluating the comparison operator.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 15, 2006
	* @version 1.0
	*/
	bool compare(const StackElem& op, 
		const UnionDataType& lhs, 
		const UnionDataType& rhs);

	/**
	* @brief Compute the UDF from the program, and store the result in stack
	* @param dataArray As above.
	* @param ip As above. Will change after reading the program.
	* @param stack The stack that stores the result element
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date September 20, 2006
	* @version 1.0
	*/
	void computeUDF(const char* dataArray, 
		size_t& ip,
		stack<UnionDataType>& stack);

	/**
	* @brief Evaluate the intput element with stuff on the stack, 
	         and push the result onto stack
	* @param program The input program to read
	* @param ip The instruction point. Will change after command reading.
	* @param stack The stack that stores the result element
	* @param elem The element that we are trying to evaluate with the stack
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date September 22, 2006
	* @version 1.0
	*/
	void evalTermWithStack(const char* program, 
		size_t& ip,
		stack<UnionDataType>& stack, 
		StackElem& elem);

}; //class Interpreter
}  //namespace QueryProcessor
}  //namespace Cayuga

#endif //_INTERPRETER_H
