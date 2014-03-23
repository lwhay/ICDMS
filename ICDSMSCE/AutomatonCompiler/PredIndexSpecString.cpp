/**
 * @file PredIndexSpecString.cpp
 * @date November 20, 2005
 * @brief Implementation file of PredIndexSpecEntryString class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "AutomatonCompiler/PredIndexSpecString.h"
#include "AutomatonCompiler/Context.h"
#include "AutomatonCompiler/NormalAtomPredString.h"
#include "AutomatonCompiler/ByteCodeUtils.h"
using namespace Cayuga::AutomatonCompiler;


#include "BasicDataStructure/Automaton/IntermediateNode.h"
using namespace Cayuga::BasicDataStructure::Automaton;

#include "BasicDataStructure/ByteCode.h"
using namespace Cayuga::BasicDataStructure::ByteCodeNS;

#include "SystemMgr/StreamSchemaMgr.h"
using namespace Cayuga::SystemMgr;

////////////////////////////////////////////////////////////////////////////////
// Foward declaration
////////////////////////////////////////////////////////////////////////////////

namespace Cayuga {
namespace AutomatonCompiler {
char* eventAttr2Instruction(const StreamID& streamID,
							const string& eventAttr,
							size_t& indexInputLengthOut);
}
}

////////////////////////////////////////////////////////////////////////////////
// PredIndexSpecEntryString member function definitions
////////////////////////////////////////////////////////////////////////////////

PredIndexSpecEntryString::PredIndexSpecEntryString (const string&  indexType,
										const string&  evAttr,
										const string&  nodeAttr 
										) :
_indexType(indexType), _evAttr(evAttr), _nodeAttr(nodeAttr) {
}

char* PredIndexSpecString::compile(const Context& context,
						size_t& indexInputLengthOut) const {
	size_t ip = 0; //instruction pointer

	//CP num_chunks ev srcInstanceArray destInstanceArray
	char* cpInstruction = new char[g_sizeCPInstruction];
	
	setValToProgram<ByteCode>(cpInstruction, ip, CP);
	
	setValToProgram<size_t>(cpInstruction, ip, _predIndexEntries.size());

	//set the following three pointers to NULL for now
	overwriteArrayFieldInCPInstruction(cpInstruction, NULL, NULL, NULL);



	ip = 0;
	indexInputLengthOut = 0;
	vector<char*> instructions;
	//PARAM source offset len
	for (vector<PredIndexSpecEntryString>::const_iterator it 
		= _predIndexEntries.begin(); it != _predIndexEntries.end(); ++it) {
			//We comment out the assertion since we may get DYNAMIC STUFF HERE
			//assert (it->_indexType == "STATIC");
			instructions.push_back(eventAttr2Instruction(context._streamID, 
				it->_evAttr, 
				indexInputLengthOut));
	}

	//the size of the program of the size of the first instruction (CP) and the 
	//sum of that of the following PARAM instructions
	char* program = new char[g_sizeCPInstruction 
		+ g_sizePARAMInstruction * instructions.size()];
	memcpy(program, cpInstruction, g_sizeCPInstruction); 
	ip = g_sizeCPInstruction;
	for (size_t i = 0; i < instructions.size(); ++i) {
			memcpy(program+ip, instructions[i], g_sizePARAMInstruction);
			ip += g_sizePARAMInstruction;
			delete[] instructions[i];
	}
	return program;
}

string PredIndexSpecEntryString::getNodeAttr() const
{
	return _nodeAttr;
}

void PredIndexSpecEntryString::write(ostream& out) const {
	out<<"<IndexedAttr ";
	out<<"Type=\""<<_indexType<<"\" ";
	out<<"EvAttrName=\""<<_evAttr<<"\" ";
	if (!_nodeAttr.empty()) {
		out<<"IndexType=\""<<_nodeAttr;
	}
	out<<"\"/>"<<endl;
}

bool PredIndexSpecEntryString::indexIsOnEventAttr(
	const string& eventAttr) const {
	return _evAttr == eventAttr;
}

AttributeID PredIndexSpecEntryString::eventAttrName2AttrID(
	const StreamID& streamID) const {
	return StreamSchemaMgr::getInstance()->
		getStreamAttrID(streamID, _evAttr);
}

////////////////////////////////////////////////////////////////////////////////
// PredIndexSpecString member function definitions
////////////////////////////////////////////////////////////////////////////////

void PredIndexSpecString::setIndexType (const string& indexType) {
	//for now we only handle hash index
	assert (indexType == "HASH");
	_indexType = indexType;
}

void PredIndexSpecString::push_back(const PredIndexSpecEntryString& entry) {
	_predIndexEntries.push_back(entry);
}


void PredIndexSpecString::clear() {
	_predIndexEntries.clear();
}

void PredIndexSpecString::write(ostream& out) const {
	for (PredIndexSpecEntrySet::const_iterator entry_it 
		= _predIndexEntries.begin(); entry_it != _predIndexEntries.end();
		++entry_it) {
			entry_it->write(out);
	}
}

/** see whether this event attribute is indexed here */
bool PredIndexSpecString::find(const string& eventAttr) const {
	for (vector<PredIndexSpecEntryString>::const_iterator it 
		= _predIndexEntries.begin(); it != _predIndexEntries.end(); ++it) {
			if (it->indexIsOnEventAttr(eventAttr))
				return true;
	}
	return false;
}

/** see whether this (event, node) attribute pair is index here */
bool PredIndexSpecString::find(const string& eventAttr,
							   const string& nodeAttr) const
{
	for(vector<PredIndexSpecEntryString>::const_iterator it
		= _predIndexEntries.begin(); it != _predIndexEntries.end(); ++it) {
			if(it->indexIsOnEventAttr(eventAttr) &&
				it->getNodeAttr() == nodeAttr)
				return true;
	}
	return false;
}

bool PredIndexSpecString::specMatchesPred(
	const vector<NormalAtomPredString*>& indexedPreds) const {
	if (_predIndexEntries.size() != indexedPreds.size()) {
		return false;
	}

	PredIndexSpecEntrySet::const_iterator entry_it = _predIndexEntries.begin();
	for (vector<NormalAtomPredString*>::const_iterator it 
		= indexedPreds.begin();	it != indexedPreds.end(); ++it) {
			assert (entry_it != _predIndexEntries.end());
			if (!entry_it->indexIsOnEventAttr((*it)->getEventAttr())) {
				return false;
			}
			++entry_it;
	}
	return true;
}




/** @return a sequence of event attribute IDs */
vector<AttributeID> PredIndexSpecString::spec2InternalIndexSpec(
	const Context& context) const {
	vector<AttributeID> attrIDs;
	for (PredIndexSpecEntrySet::const_iterator entry_it 
		= _predIndexEntries.begin(); entry_it != _predIndexEntries.end();
		++entry_it) {
			attrIDs.push_back(entry_it->eventAttrName2AttrID(
				context._streamID));
	}
	return attrIDs;
}

/** @returns a sequence of pairs of (event, node) attribute IDs */
vector<pair<AttributeID, AttributeID> > PredIndexSpecString::
		spec2InternalDynIndexSpec(const Context& context) const
{
	vector<pair<AttributeID, AttributeID> > EvNodeAttrPairs;
	Node* currNode = context._pSourceNode;		
	for (PredIndexSpecEntrySet::const_iterator entry_it
		= _predIndexEntries.begin(); entry_it != _predIndexEntries.end();
		++entry_it) {		
			EvNodeAttrPairs.push_back(pair<AttributeID, AttributeID> (
				entry_it->eventAttrName2AttrID(currNode->getInputStream()),
				((IntermediateNode*) currNode)->getNodeSchema()->getID
				(entry_it->getNodeAttr())));			
	}	
	return EvNodeAttrPairs;
}
