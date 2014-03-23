/**
* @file SchemaMapString.cpp
* @date April 25, 2006
* @brief Implementation file of SchemaMapString class.
* @author Mingsheng Hong (mshong@cs.cornell.edu)
*/


#include "AutomatonCompiler/Context.h"
#include "AutomatonCompiler/SchemaMapString.h"

#include "AutomatonCompiler/PredExprEval/L.hpp"
#include "AutomatonCompiler/PredExprEval/P.hpp"
#include "AutomatonCompiler/PredExprEval/BasicAST.h"
using namespace Cayuga::AutomatonCompiler;
using namespace Cayuga::AutomatonCompiler::PredExprEval;

#include "BasicDataStructure/Automaton/IntermediateNode.h"
using namespace Cayuga::BasicDataStructure::Automaton;

#include "BasicDataStructure/ByteCode.h"
using namespace Cayuga::BasicDataStructure::ByteCodeNS;

#include "SystemMgr/StreamSchema.h"

#include "Utils/RAssert.h"
using namespace Cayuga::Utils;

#include <sstream>

////////////////////////////////////////////////////////////////////////////////
// Foward declaration
////////////////////////////////////////////////////////////////////////////////

namespace Cayuga {
namespace AutomatonCompiler {

} //namespace AutomatonCompiler
} //namespace Cayuga

////////////////////////////////////////////////////////////////////////////////
// OldNewAttrPair member function definitions
////////////////////////////////////////////////////////////////////////////////

OldNewAttrPair::OldNewAttrPair() {
}

OldNewAttrPair::OldNewAttrPair (const string& src,
	const string& oldAttr,
	const string& newAttr,
	bool bIsAggregate) 
	: _src(src), _oldAttr(oldAttr), _newAttr(newAttr), 
	_bIsAggregate(bIsAggregate) {
}

void OldNewAttrPair::write(ostream& out) const {
	if (!_bIsAggregate) {
		out<<"<OldNewAttrPair ";
		out<<"OldAttrSource=\""<<_src<<"\" ";
		out<<"OldAttr=\""<<_oldAttr<<"\" ";
		out<<"NewAttr=\""<<_newAttr<<"\"";
		out<<"/>"<<endl;
	}
	else if (SchemaMapString::isSDF(_src)) {
		out<<"<Aggregate ";
		out<<"Name=\""<<_src<<"\" ";
		out<<"EventAttr=\""<<_oldAttr<<"\" ";
		out<<"NodeAttr=\""<<_newAttr<<"\"";
		out<<"/>"<<endl;
	}
	else {
		out<<"<UDF ";
		out<<"Expr=\""<<_src<<"\" ";
		out<<"Attr=\""<<_newAttr<<"\"";
		out<<"/>"<<endl;
	}
}

////////////////////////////////////////////////////////////////////////////////
// SchemaMapString member function definitions
////////////////////////////////////////////////////////////////////////////////

SchemaMapString::SchemaMapString() {
}

SchemaMapString::~SchemaMapString() {
}

void SchemaMapString::write(ostream& out) const {
	if (!_schemaMap.empty()) {
		out<<"<SchemaMap>"<<endl;
		//for (_SchemaMap::const_iterator it = _schemaMap.begin();
		//it != _schemaMap.end(); ++it) {
		for (vector<string>::const_iterator it = _destAttrs.begin();
			it !=  _destAttrs.end(); ++it) {
				//it->second.write(out);
			const OldNewAttrPair& onap = findDestAttr(*it);
			onap.write(out);
		}
		out<<"</SchemaMap>"<<endl;
	}
}

bool SchemaMapString::isSDF(const string& aggName) {
	if (aggName == "COUNT" || aggName == "SUM" 
		|| aggName == "MIN" || aggName == "MAX")
		return true;

	return false;
}

bool SchemaMapString::attributeCreatedByUDF(
	const set<string>& referredAttrs) const {
	for (set<string>::const_iterator it = referredAttrs.begin();
		it != referredAttrs.end(); ++it) {
			const string& referredAttr = *it;
			const OldNewAttrPair* pOnap = findDestAttrOrNULL(referredAttr);
			if (pOnap && pOnap->_bIsAggregate) 
				return true;
	}

	return false;

}

void SchemaMapString::composeSchemaMapToSchemaOfInputMap(
	const SchemaMapString* inputSchemaMap) {
	_SchemaMap newSchemaMap;
	vector<string> newDestAttrs;

	for (vector<string>::const_iterator it = inputSchemaMap->_destAttrs.begin();
		it != inputSchemaMap->_destAttrs.end(); ++it) {
		const string& destAttr = *it;
		const OldNewAttrPair& onap2 = inputSchemaMap->findDestAttr(destAttr);
		OldNewAttrPair newOnap;
		if (onap2._bIsAggregate) {
			newOnap = onap2;
		}
		else {
			//Since inputSchemaMap must be on a forward edge out of start node
			Assert (onap2._src == "EV");
			const string& middleAttr = onap2._oldAttr;
			const OldNewAttrPair& onap1 = findDestAttr(middleAttr);
			newOnap = onap1;
			newOnap._newAttr = destAttr;
		}

		newSchemaMap.insert(make_pair(newOnap._newAttr, newOnap));
		newDestAttrs.push_back(newOnap._newAttr);
	}

	//_schemaMap.clear();
	_schemaMap.swap(newSchemaMap);
	_destAttrs = newDestAttrs;

	//write();
}

void SchemaMapString::composeSchemaMapToSchemaOfCurrentMap(
	const SchemaMapString* inputSchemaMap) {
	for (vector<string>::const_iterator it = _destAttrs.begin();
		it != _destAttrs.end(); ++it) {
		const string& destAttr = *it;
		//OldNewAttrPair& onap = findDestAttr(destAttr);
		_SchemaMap::iterator find_it = _schemaMap.find(destAttr);
		Assert(find_it != _schemaMap.end());
		OldNewAttrPair& onap = find_it->second;

		if (onap._src == "EV") {
			//onap._oldAttr must be the newAttr of some entry in inputSchemaMap
			const OldNewAttrPair& onap2 = 
				inputSchemaMap->findDestAttr(onap._oldAttr);
			//onap._oldAttr = onap2._oldAttr;
			onap = onap2;
		}
	}
}

void compileSDF(const string& aggName,  
				const OldNewAttrPair& onap,
				const string& destAttr,
				const Node* pSourceNode,
				const StreamID& streamID,
				size_t &szInstr,
				char*& instruction,
				size_t& attrLen) {
	const string& eventAttr = onap._oldAttr;
	const string& nodeAttr = destAttr;

	bool bAggAttrNew;
	unsigned srcAttrID;
	if (typeid(*pSourceNode) == typeid(StartNode)) {
		bAggAttrNew = true;
	}
	else { 
		try {
			srcAttrID = ((IntermediateNode*)pSourceNode)->
				getNodeSchema()->getID(nodeAttr);
			bAggAttrNew = false;
		}
		catch (...) {
			//exception was generated by the above getID call
			//only when the source node schema does not have the
			//aggregate attribute.
			bAggAttrNew = true;
		}
	}

	size_t thisIp = 0; 
	if (aggName == "COUNT") {
		if (bAggAttrNew) {
			szInstr = sizeof(ByteCode)*2+g_szAttrInstruction;
			instruction = new char[szInstr];
			setValToProgram<ByteCode>(instruction, thisIp, AGG);
			makeConstInstruction<int>(instruction, thisIp, INT, 1);
			attrLen = sizeof(int);
		}
		else {
			szInstr = sizeof(ByteCode)*2
				+g_szAttrInstruction
				+g_szIntConstInstruction
				+g_szOpInstruction;
			instruction = new char[szInstr];
			setValToProgram<ByteCode>(instruction, thisIp, AGG);

			size_t srcAttrOffset;
			getNodeAttrInfo(pSourceNode, 
				nodeAttr,
				srcAttrOffset, 
				attrLen);

			makeAttrInstruction(instruction, 
				thisIp,
				NODE,
				INT,
				srcAttrOffset);

			makeConstInstruction<int>(instruction, thisIp, INT, 1);

			makeOpInstruction(instruction, 
				thisIp,
				PLUS,
				INT);
		}

	}
	else if (aggName == "SUM" 
		|| aggName == "MIN" 
		|| aggName == "MAX") {
			//if the NodeAttr is not present in source state, we should
			//initialize its value properly.
			if (bAggAttrNew) {
				instruction = copyEventAttrToNodeAttr(streamID,
					eventAttr,
					thisIp,
					szInstr, 
					attrLen);
			}
			else {
				ByteCode opType;
				if (aggName == "SUM") {
					opType = PLUS;
				}
				else if (aggName == "MIN") {
					opType = MIN;
				}
				else {
					assert (aggName == "MAX");
					opType = MAX;
				}

				instruction = aggEventNodeAttrToNodeAttr(streamID,
					eventAttr,
					thisIp,
					szInstr, 
					attrLen,
					pSourceNode,
					nodeAttr,
					opType);
			}

	}

	//put the EOP command at end of AGG instruction
	setValToProgram<ByteCode>(instruction, thisIp, EOP);

}

char* SchemaMapString::compile(Node* pSourceNode, 
			  IntermediateNode* pDestNode,
			  size_t& schemaLen) const {
	schemaLen = 0;

	size_t ip = 0; //instruction pointer
	//CP num_chunks ev srcInstanceArray destInstanceArray
	char* cpInstruction = new char[g_sizeCPInstruction];

	setValToProgram<ByteCode>(cpInstruction, ip, CP);

	const vector<string>& attrNames = pDestNode->getNodeSchema()->_values;
	setValToProgram<size_t>(cpInstruction, ip, attrNames.size());

	//set the following three pointers to NULL for now
	overwriteArrayFieldInCPInstruction(cpInstruction, NULL, NULL, NULL);

	ip = 0;
	char* instruction;

	//a sequence of PARAM/AGG instructions
	vector<pair<char*, size_t> > instructions;

	//PARAM source offset len
	StreamID streamID = ((StartNode*)pSourceNode)->getInputStream();
	for (vector<string>::const_iterator it = attrNames.begin();
	  it !=  attrNames.end(); ++it) {
		  const string destAttr = *it;
		  size_t attrLen = 0;
		  OldNewAttrPair onap = findDestAttr(destAttr);
		  //bool bIsUDF = false;
		  if (!onap._bIsAggregate) {
			  //for PARAM instruction
			  if (onap._src == "EV") {
				  instruction = eventAttr2Instruction(streamID, 
					  onap._oldAttr, 
					  attrLen);
			  }
			  else if (onap._src == "NODE") {
				  assert (typeid(*pSourceNode) == typeid(IntermediateNode));

				  size_t srcAttrOffset;
				  getNodeAttrInfo(pSourceNode, 
					  onap._oldAttr, 
					  srcAttrOffset, 
					  attrLen);

				  instruction = makePARAMInstruction(NODE, 
					  srcAttrOffset, 
					  attrLen);
			  }
			  else {
				  assert(false);
			  }
			  instructions.push_back(make_pair(instruction, 
				  g_sizePARAMInstruction));
		  }
		  else {
			  //This is an aggregate. It can be a system-defined aggregate,
			  //SUM, COUNT, MIN, MAX, or user-defined one.

			  //hack: we overload the meaning of the member fields
			  //in OldNewAttrPair here. This may cause confusion. 
			  //So we first correctly name the fields for aggregate 
			  //specification.
			  string& aggName = onap._src;
			  size_t szInstr = 0;

			  if (isSDF(aggName)) {
				  compileSDF(aggName, onap, destAttr, pSourceNode, 
					  streamID, szInstr, instruction, attrLen);
			  }
			  //AVG will be simulated by SUM, COUNT and DIV in the query 
			  //front-end. So the engine does not handle AVG directly.
			  else {
				  //TODO: will refactor the following code segment into a 
				  //function later. 
				  const string& expr = aggName;
				  istringstream in(expr);
				  L lexer(in);
				  P parser(lexer);

				  ASTFactory my_factory("BasicAST", BasicAST::factory);
				  parser.initializeASTFactory(my_factory);
				  parser.setASTFactory(&my_factory);

				  // invoke the parser...
				  parser.arith_exp();
				  //cout<<"parsing successful!"<<endl;

				  //get AST
				  //RefBasicAST ast = RefBasicAST(parser.getAST());
				  RefBinaryCompArithOpAST ast = RefBinaryCompArithOpAST(parser.getAST());

				  //cout<<"print AST"<<endl;
				  //if (ast)
				  //	cout << ast->toStringList() << endl;
				  //else
				  //	cout << "null AST" << endl;

				  //cout<<"compiling stuff!"<<endl;
				  vector<pair<char*, size_t> > program;
				  Context context;
				  context._pSourceNode = pSourceNode;
				  context._streamID = streamID;
				  ast->compile(context, program);

				  //cout<<"compiling successful, output program!"<<endl;
				  //size_t szInstr = 0;
				  for (size_t i = 0; i < program.size(); i++) {
					  szInstr += program[i].second;
				  }

				  //one last instruction for EOP
				  szInstr += sizeof(ByteCode);

				  instruction = new char[szInstr];
				  size_t thisIp = 0;
				  for (size_t i = 0; i < program.size(); i++) {
					  memcpy(instruction+thisIp, 
						  program[i].first, 
						  program[i].second);
					  thisIp += program[i].second;
					  delete[] program[i].first;
				  }
				  setValToProgram<ByteCode>(instruction, thisIp, EOP);

				  //bug: for now, int, float and intStringHandle are all 4 bytes
				  attrLen = 4;
			  }
			  instructions.push_back(make_pair(instruction, szInstr));
		  }
		  schemaLen += attrLen;
	}

	//the size of the program of the size of the first instruction (CP) and the 
	//sum of that of the following PARAM instructions
	size_t szProgram = 0;
	szProgram += g_sizeCPInstruction;
	for (size_t i = 0; i < instructions.size(); ++i) {
		szProgram += instructions[i].second;
	}
	char* program = new char[szProgram];
	memcpy(program, cpInstruction, g_sizeCPInstruction); 
	delete[] cpInstruction;

	ip = g_sizeCPInstruction;
	for (size_t i = 0; i < instructions.size(); ++i) {
		memcpy(program+ip, instructions[i].first, instructions[i].second);
	  ip += instructions[i].second;
	  delete[] instructions[i].first;
	}
	return program;
}

bool SchemaMapString::empty() const {
	return _schemaMap.empty();
}

const OldNewAttrPair& SchemaMapString::findDestAttr(const string& destAttr) 
const {
	_SchemaMap::const_iterator find_it = _schemaMap.find(destAttr);
	assert(find_it != _schemaMap.end());
	return find_it->second;
}

const OldNewAttrPair* SchemaMapString::findDestAttrOrNULL(
	const string& destAttr) const {
	_SchemaMap::const_iterator find_it = _schemaMap.find(destAttr);
	if (find_it != _schemaMap.end()) {
		return &find_it->second;
	}

	return NULL;
}

void SchemaMapString::push_back(const OldNewAttrPair& attrPair) {
	//_schemaMap.push_back(attrPair);
	_schemaMap.insert(make_pair(attrPair._newAttr, attrPair));

	_destAttrs.push_back(attrPair._newAttr);
}

//bool SchemaMapString::IsIdentityMap() const {
//	for (vector<string>::const_iterator it = _destAttrs.begin();
//		it != _destAttrs.end(); ++it) {
//			const string& destAttr = *it;
//			const OldNewAttrPair& onap = findDestAttr(destAttr);
//
//			if (onap._oldAttr == onap._newAttr) {
//				Assert (!onap._bIsAggregate);
//				Assert (onap._src == "EV" || onap._src == "NODE");
//			}
//			else
//				return false;
//	}
//
//	return true;
//}

bool SchemaMapString::renameAttrRefAndSource(const string& inAttr, 
							string& outAttr,
							bool& outAttrFromEvent) const {
	const OldNewAttrPair* evAttrPair 
		= findDestAttrOrNULL(inAttr);
	//the attribute cannot be created from UDF
	if (evAttrPair) {
		Assert (!evAttrPair->_bIsAggregate);
		outAttr = evAttrPair->_oldAttr;
		outAttrFromEvent = (evAttrPair->_src == "EV");
		return true;
	}

	return false;
}
