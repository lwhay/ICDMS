/**
 * @file Interpreter.cpp
 * @date November 26, 2005
 * @brief Implementation file of Interpreter class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "QueryProcessor/Interpreter.h"
using namespace Cayuga::QueryProcessor;

#include "AutomatonCompiler/ByteCodeUtils.h"
using Cayuga::AutomatonCompiler::getValFromProgram;
using Cayuga::AutomatonCompiler::setValToProgram;

#include "BasicDataStructure/ByteCode.h"
using namespace Cayuga::BasicDataStructure::ByteCodeNS;

#include "MemoryMgr/CY_IntStringFace.h"
using namespace CY_IntString;

#include "SystemMgr/FuncMgr.h"
using namespace Cayuga::SystemMgr;

#include <cassert>
#include <iostream>
#include <stack>
#include <cmath>
using namespace std;

Interpreter::Interpreter() {
}

bool Interpreter::exec(const char* program) {
	_ev = NULL;
	_srcInstArray = NULL;
	_destInstArray = NULL;

	size_t ip = 0;  //instruction pointer

	ByteCode bcc = getValFromProgram<ByteCode>(program, ip);
	switch (bcc) {
		case EVAL:
			return eval(program, ip);	
		case CALL:
			assert(false);
		case CP:
			return schemaMap(program, ip);
		//case GC:
		default:
			assert(false);

	}
	return true;
}

inline void Interpreter::readTypedValue(const char* dataArray, 
										size_t& offset,						
										UnionDataType& elem) {
	switch (elem._type) {
		case INT:
			elem._i = getValFromProgram<int>(dataArray, offset);
			break;
		case FLOAT:
			elem._f = getValFromProgram<float>(dataArray, offset);
			break;
		case STRING:
			elem._s = getValFromProgram<IntStringHandle>(dataArray, offset);
			break;
		default:
			assert(false);
	}
}

inline void Interpreter::readCommand(const char* program, 
							  size_t& ip, 
							  StackElem& elem) {
	elem._op = getValFromProgram<ByteCode>(program, ip);

	if (elem._op == EOP) {
		return;
	}

	if (elem._op == CALL) {
		return;
	}

	//other cases
	elem._data._type = getValFromProgram<ByteCode>(program, ip);

	if ((elem._op >= EQ && elem._op <= EXP) ||
		(elem._op >= MIN && elem._op <= MAX)) { 
		//arith, comp or agg stuff
		return;
	}
	
	if (elem._op == CONSTANT) {
		readTypedValue(program, ip, elem._data);
		return;
	}

	if (elem._op == EVENT || elem._op == NODE) {
		size_t offset = getValFromProgram<unsigned>(program, ip);
		if (elem._op == EVENT) {
			readTypedValue(_ev, offset, elem._data);
		}
		else {
			readTypedValue(_srcInstArray, offset, elem._data);
		}
		return;
	}

	cerr<<"feature not implemented yet."<<endl;
	assert(false);

}


inline void Interpreter::getTypedValue(void* arg, const ByteCode& type, 
								  const UnionDataType& data) {
  assert(data._type == type);
  switch (type) {
	  case INT:
		  *(int*)arg = data._i;
		  break;
	  case FLOAT:
		  *(float*)arg = data._f;
		  break;
	  case STRING:
		  *(IntStringHandle*)arg = data._s;
		  break;
	  default:
		  cerr<<"error: type value not understood "<<type<<endl;
		  assert(false);
  }
}

inline void Interpreter::setTypedValue(const void* arg, const ByteCode& type, 
								  UnionDataType& data) {
  data ._type = type;
  switch (type) {
	  case INT:
		  data._i = *(int*)arg;
		  break;
	  case FLOAT:
		 data._f =  *(float*)arg;
		  break;
	  case STRING:
		  data._s = *(IntStringHandle*)arg;
		  break;
	  default:
		  cerr<<"error: type value not understood "<<type<<endl;
		  assert(false);
  }
}
					

inline float Interpreter::getFloatValue(const UnionDataType& data) {
	if (data._type == INT)
		return (float)data._i;

	assert (data._type == FLOAT);
	return data._f;
}

inline IntStringHandle Interpreter::getStringValue(const UnionDataType& data) {
	assert (data._type == STRING);
	return data._s;
}

inline void Interpreter::calculate(const StackElem& op, 
								   const UnionDataType& lhs, 
								   const UnionDataType& rhs, 
								   UnionDataType& result) {
   result._type = op._data._type;
   float fResult;
   float fLhs = getFloatValue(lhs);
   float fRhs = getFloatValue(rhs);

   switch (op._op) {
	   //arithmetic operators
	   case PLUS:
		   fResult = fLhs + fRhs;
		   break;
	   case MINUS:
		   fResult = fLhs - fRhs;
		   break;
	   case MUL:
		   fResult = fLhs * fRhs;
		   break;
	   case DIV:
		   fResult = fLhs / fRhs;
		   break;
	   case MOD:
		   assert (lhs._type == INT);
		   assert (rhs._type == INT);
		   fResult = float((int)fLhs % (int)fRhs);
		   break;
	   case EXP:
		   fResult = powf(fLhs,fRhs);
		   break;

		   //aggregate operators
	   case MIN:
		   fResult = min(fLhs, fRhs);
		   break;
	   case MAX:
		   fResult = max(fLhs, fRhs);
		   break;

	   default:
		   assert (false);
   }

   if (result._type == INT) {
	   result._i = (int)fResult;
   }
   else {
	   assert (result._type == FLOAT);
	   result._f = fResult;
   }
}

inline bool Interpreter::compare(const StackElem& op, 
								 const UnionDataType& lhs, 
								 const UnionDataType& rhs) {
	 if (op._data._type != STRING) {
		 float fLhs = getFloatValue(lhs);
		 float fRhs = getFloatValue(rhs);

		 switch (op._op) {
	   case EQ:
		   return fLhs == fRhs;
		   break;
	   case NE:
		   return fLhs != fRhs;
		   break;
	   case LE:
		   return fLhs <= fRhs;
		   break;
	   case LT:
		   return fLhs < fRhs;
		   break;
	   case GE:
		   return fLhs >= fRhs;
		   break;
	   case GT:
		   return fLhs > fRhs;
		   break;
	   default:
		   assert (false);
		 }
	 }
	 else {
		 assert (lhs._type == STRING);
		 assert (rhs._type == STRING);
		 switch (op._op) {
	   case EQ:
		   return lhs._s == rhs._s;
		   break;
	   case NE:
		   return !(lhs._s == rhs._s);
		   break;
	   default:
		   assert (false);
		 }
	 }

	 //this will never be reached
	 return false;
}

void Interpreter::computeUDF(const char* program, 
							 size_t& ip,
							 stack<UnionDataType>& stack) {
	UDF_Ptr fp = getValFromProgram<UDF_Ptr>(program, ip);

	ByteCode retType = getValFromProgram<ByteCode>
		(program, ip);

	int numArgs = getValFromProgram<int>(program, ip);

	UnionDataType lhs, rhs;
	ByteCode arg1Type, arg2Type;
	char arg1Array[8], arg2Array[8];
	char* arg1 = arg1Array;
	void* arg2 = arg2Array;
	if (numArgs == 2) {
		//for now assume it is a binary operator
		rhs = stack.top();
		stack.pop();
		lhs = stack.top();
		stack.pop();

		arg1Type = getValFromProgram<ByteCode>
			(program, ip);
		arg2Type = getValFromProgram<ByteCode>
			(program, ip);

		getTypedValue(arg1, arg1Type, lhs);
		getTypedValue(arg2, arg2Type, rhs);

	}
	else if (numArgs == 1) {
		lhs = stack.top();
		stack.pop();

		arg1Type = getValFromProgram<ByteCode>
			(program, ip);

		getTypedValue(arg1, arg1Type, lhs);

	}
	else {
		assert (numArgs == 0);
	}

	//IntStringHandle lhsHandle = getStringValue(lhs);
	//IntStringHandle rhsHandle = getStringValue(rhs);

	UnionDataType result;
	char retArray[8];
	void* ret = retArray;
	(*fp)(arg1, arg2, ret);
	setTypedValue(ret, retType, result);

	stack.push(result);
}

void Interpreter::evalTermWithStack(const char* program, 
									size_t& ip,
									stack<UnionDataType>& stack, 
									StackElem& elem) {
	if (elem._op == CONSTANT || elem._op == EVENT || elem._op == NODE) {
		stack.push(elem._data);
	}
	else if (elem._op >= PLUS && elem._op <= CALL) { 
		if (elem._op != CALL) {
			UnionDataType rhs = stack.top();
			stack.pop();
			UnionDataType lhs = stack.top();
			stack.pop();


			UnionDataType result;
			calculate(elem, lhs, rhs, result);
			stack.push(result);
		}
		else {
			//call function pointer
			computeUDF(program, ip, stack);
		}
	}
	else {
		assert (false);
	}
}

bool Interpreter::eval(const char* program, size_t& ip) {
	//variables used in some CASE statement below
	//int rhs;
	//float fRhs;
	//size_t rhs_offset;
	//size_t dest_offset;

	size_t numAtomPreds = getValFromProgram<size_t>(program, ip);

	_ev = getValFromProgram<const char*>(program, ip);
	_srcInstArray = getValFromProgram<const char*>(program, ip);


	for (size_t atomPredIndex = 0; atomPredIndex < numAtomPreds; 
		++atomPredIndex) {
		stack<UnionDataType> stack;
		StackElem bottom;
		readCommand(program, ip, bottom);

		assert (bottom._op == CONSTANT 
			|| bottom._op == EVENT || bottom._op == NODE);
		stack.push(bottom._data);
		
		while (true) {
			assert (!stack.empty());

			StackElem elem;
			readCommand(program, ip, elem);
			
			//evaluate the new elem together with stuff in stack
			if (!(elem._op >= EQ && elem._op <= GT)) {
				evalTermWithStack(program, ip, stack, elem);
			}
			else {
				//last step
				assert (elem._op >= EQ && elem._op <= GT);
				UnionDataType rhs = stack.top();
				stack.pop();
				UnionDataType lhs = stack.top();
				stack.pop();


				assert (stack.empty());
				bool ret = compare(elem, lhs, rhs);
				if (ret == false) {
					return false;
				}
				else {
					break;
				}
			}
		}
	}
	
	return true;
}

void Interpreter::writeTypedValue(char* dataArray, 
								  size_t& offset, 
								  const UnionDataType& result) {
	switch (result._type) {
	case INT:
		setValToProgram<int>(dataArray, 
			offset, 
			result._i);
		break;
	case FLOAT:
		setValToProgram<float>(dataArray, 
			offset, 
			result._f);
		break;
	case STRING:
		setValToProgram<IntStringHandle>(dataArray, 
			offset, 
			result._s);
		break;
	default:
		assert (false);
	}
}

void Interpreter::writeStackElemToArray(char* dataArray, 
								  size_t& offset,
								  stack<UnionDataType>& stack) {
	assert (stack.size() == 1);
	UnionDataType result = stack.top();
	stack.pop();
	writeTypedValue(dataArray, offset, result);
}


bool Interpreter::schemaMap(const char* program, size_t& ip) {
		
	size_t numParamChunks = getValFromProgram<size_t>(program, ip);
	
	_ev = getValFromProgram<const char*>(program, ip);
	_srcInstArray = getValFromProgram<const char*>(program, ip);
	_destInstArray = getValFromProgram<char*>(program, ip);

	size_t dest_offset = 0;
	size_t src_offset, len;
	ByteCode attrSrc;
	StackElem bottom;
	stack<UnionDataType> stack;
	for (size_t chunkIndex = 0; chunkIndex < numParamChunks; ++chunkIndex) {
		//PARAM source offset len
		ByteCode bcc = getValFromProgram<ByteCode>(program, ip);
		switch (bcc) {
			case PARAM:
				attrSrc = getValFromProgram<ByteCode>(program, ip);

				src_offset = getValFromProgram<size_t>(program, ip);
				len = getValFromProgram<size_t>(program, ip);

				switch (attrSrc) {
					case EVENT:
						memcpy(_destInstArray+dest_offset, 
							_ev+src_offset, len);
						break;
					case NODE:
						memcpy(_destInstArray+dest_offset, 
							_srcInstArray+src_offset, len);
						break;
					default:
						assert(false);
				}
				dest_offset += len;
				break;
			case AGG:
				readCommand(program, ip, bottom);

				assert (bottom._op == CONSTANT 
					|| bottom._op == EVENT || bottom._op == NODE);
				stack.push(bottom._data);

				while (true) {
					assert (!stack.empty());

					StackElem elem;
					readCommand(program, ip, elem);

					if (elem._op == EOP) {
						//end of agg program.
						//the stack should have one element, which is the result
						writeStackElemToArray(_destInstArray, 
							dest_offset, 
							stack);
						break; //done with agg instruction
					}

					//evaluate the new elem together with stuff in stack
					if (elem._op == CONSTANT || elem._op == EVENT 
						|| elem._op == NODE) {
						stack.push(elem._data);
					}
					else if ((elem._op >= PLUS && elem._op <= EXP)
						|| (elem._op >= MIN && elem._op <= MAX) ) { 
						UnionDataType rhs = stack.top();
						stack.pop();
						UnionDataType lhs = stack.top();
						stack.pop();


						UnionDataType result;
						calculate(elem, lhs, rhs, result);
						stack.push(result);
					}
					else {
						assert (false);
					}
				}
				break;
			//case CALL:
			//	//Now in schema map this is not necessarily a CALL byte code
			//	//any more. A 0-ary function is so, like CayugaMsg1(), but
			//	//functions like SIMILARITY(body1, body2) will put the two
			//	//input arguments in the byte code sequence, and then evaluate.
			//	//This therefore requires that here we can invoke the eval() 
			//	//function a bit to do general stack-based computation?
			//	computeUDF(program, ip, stack);
			//	writeStackElemToArray(_destInstArray, 
			//				dest_offset, 
			//				stack);
			//	break; //done with CALL instruction


			default:
				//retract the byte code command just read
				ip -= sizeof(ByteCode);

				//general evaluation strategy
				while (true) {					
					//assert (!stack.empty());
					StackElem elem;
					readCommand(program, ip, elem);

					//evaluate the new elem together with stuff in stack
					if (elem._op != EOP) {
						evalTermWithStack(program, ip, stack, elem);
					}
					else {
						writeStackElemToArray(_destInstArray, 
							dest_offset, 
							stack);
						break;
					}
				}
				//	assert(false);
				break;
		}
	}
	return true;
}
