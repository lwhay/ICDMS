/**
 * @file SchemaMapBuilder.cpp
 * @date April 8, 2007
 * @brief This file defines SchemaMapBuilder class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include <fstream>
#include <set>
using namespace std;

#include "P2TokenTypes.hpp"
#include "SchemaMapBuilder.h"

using namespace Cayuga::QLCompiler;

#include "../ICDSMSCE/AutomatonCompiler/SchemaMapString.h"
using namespace Cayuga::AutomatonCompiler;

#include "../ICDSMSCE/SAXHandler/ConfigMgr.h"
using namespace Cayuga::SAXHandler;

#include "../ICDSMSCE/SystemMgr/DataTypeMgr.h"
#include "../ICDSMSCE/SystemMgr/Options.h"
#include "../ICDSMSCE/SystemMgr/StreamSchemaMgr.h"
using namespace Cayuga::SystemMgr;

#include "../ICDSMSCE/Utils/AttrMap.h"
#include "../ICDSMSCE/Utils/RAssert.h"
using namespace Cayuga::Utils;

#include "../ICDSMSCE/SystemMsg.h"
using namespace Cayuga::SystemMsg;

//SchemaMapBuilder::SchemaMapBuilder() {
//}
//
//SchemaMapBuilder::~SchemaMapBuilder(void) {
//}

void SchemaMapBuilder::visit(RefCayugaQLAST& thisAST) {
	Assert (thisAST->getType() == P2TokenTypes::QUERY);

	_processQuery(thisAST);
}

void SchemaMapBuilder::_processQuery(RefCayugaQLAST& queryAST) {
	//get select clause
	RefCayugaQLAST selectClauseAST = getSelectClauseChildFromQuery(queryAST);

	//process streamExpr clause
	RefCayugaQLAST streamExprAST = getStreamExprChildFromQuery(queryAST);

	RefCayugaQLAST outputClauseAST 
		= (RefCayugaQLAST)streamExprAST->getNextSibling();

	_constructSchemaMapForStreamExpr(streamExprAST);

	//process optional output/publish/result clause
	_constructSchemaMapForSelectClause(selectClauseAST, streamExprAST);	
}

void SchemaMapBuilder::_constructSchemaMapForStreamExpr(
	RefCayugaQLAST& streamExprAST) {	
	RefCayugaQLAST unaryOrStreamExprAST 
		= (RefCayugaQLAST)streamExprAST->getFirstChild();
	if (unaryOrStreamExprAST->getType() == P2TokenTypes::UNARY_EXPR) {
		_processUnaryExpr(unaryOrStreamExprAST);
	}
	else {
		_constructSchemaMapForStreamExpr(unaryOrStreamExprAST);
	}

	RefCayugaQLAST binOpAST 
		= (RefCayugaQLAST)unaryOrStreamExprAST->getNextSibling();
	if (!binOpAST) {
		//sexp is unary
		//do nothing in this case, since we have invoked _processUnaryExpr on
		//the unaryExpr child of sexp above
	}
	else {
		RefCayugaQLAST rhsUnaryExprAST 
			= (RefCayugaQLAST)binOpAST->getNextSibling();
		Assert (rhsUnaryExprAST 
			&& rhsUnaryExprAST->getType() == P2TokenTypes::UNARY_EXPR);
		
		_processUnaryExpr(rhsUnaryExprAST);

		//rhsUnaryExprAST->_thisForwardEdgeSchemaMap->write();

		//construct the schema maps of the two forward edges in the top level
		//2-step automaton. Store the first one in binOp, and store the second
		//one in sexp.
		if (binOpAST->getType() == P2TokenTypes::MU_OP) {
			//first forward edge
			_constructSchemaMapForForwardEdgeIntoMU(
				unaryOrStreamExprAST->_thisSchema,	//event schema
				binOpAST->_thisSchema,				//destState schema
				*(binOpAST->_thisForwardEdgeSchemaMap)
				);

			//second forward edge
			_constructSchemaMapForForwardEdgeOutOfMUIntoEND(
				unaryOrStreamExprAST->_thisSchema,	//prevEvent schema
				binOpAST->_thisSchema,				//srcState schema
				rhsUnaryExprAST->_thisSchema,		//event schema
				streamExprAST->_thisSchema,			//destState schema
				binOpAST,							//AST node possibly
													//containing UDF specs
				*(streamExprAST->_thisForwardEdgeSchemaMap));

			
			//TO EDIT: constructing rebind edge schema map
			_constructSchemaMapForRebindEdge(
				rhsUnaryExprAST->_thisSchema,			//event schema
				binOpAST->_thisSchema,					//destState schema
				binOpAST,
				*(binOpAST->_thisRebindEdgeSchemaMap));		
		}
		else {
			Assert (binOpAST->noUDF());
			//first forward edge
			_constructSchemaMapForForwardEdgeInToSEQ(
				unaryOrStreamExprAST->_thisSchema,	//event schema
				binOpAST->_thisSchema,				//destState schema
				rhsUnaryExprAST->_thisSchema,		//nextEvent schema
				*(binOpAST->_thisForwardEdgeSchemaMap)
				);

			//second forward edge
			_constructSchemaMapForForwardEdgeOutOfSEQIntoEND(
				binOpAST->_thisSchema,				//srcState schema
				rhsUnaryExprAST->_thisSchema,		//event schema
				streamExprAST->_thisSchema,			//destState schema
				*(streamExprAST->_thisForwardEdgeSchemaMap)
				);
		}
	}
}

void SchemaMapBuilder::_processUnaryExpr(
	RefCayugaQLAST& unaryExprAST) {
	RefCayugaQLAST childAST 
		= (RefCayugaQLAST)unaryExprAST->getFirstChild();
	RefCayugaQLAST primaryExprAST;
	if (childAST->getType() ==  P2TokenTypes::UNARY_OPS)
		primaryExprAST = childAST->getNextSibling();
	else
		primaryExprAST = childAST;

	_processPrimaryExpr(primaryExprAST);

	if (childAST->getType() ==  P2TokenTypes::UNARY_OPS) {
		//construct a schema map for the forward edge on the top level 
		//1-step autumaton here, which treates the schema of 
		//primaryExprAST as both the event schema (source) and the end state
		//schema.
		//Therefore, this schema map is an identity map
		_constructIdentitySchemaMapForUnaryExpr(
			primaryExprAST->_thisSchema,
			*(childAST->_thisForwardEdgeSchemaMap)
			);		
	}
}


void SchemaMapBuilder::_processPrimaryExpr(
	RefCayugaQLAST& primaryExprAST) {
	if (primaryExprAST->getType() == P2TokenTypes::STREAM_NAME) {
		//the schema map in this case maps the schema of event stream 
		//named STREAM_NAME to the schema of primaryExprAST
	}
	else {
		Assert (primaryExprAST->getType() == P2TokenTypes::QUERY);
		//process sub-query
		_processQuery(primaryExprAST);
	}
}

void SchemaMapBuilder::_constructSchemaMapForSelectClause(
	RefCayugaQLAST& selectClauseAST,
	const RefCayugaQLAST& streamExprAST) {
	_constructSchemaMapForForwardEdgeInOneStepAutomaton(
		streamExprAST->_thisSchema,		//event schema
		selectClauseAST->_thisSchema,	//end state schema
		selectClauseAST,
		*(selectClauseAST->_thisForwardEdgeSchemaMap));
}

void SchemaMapBuilder::_constructSchemaMapForForwardEdgeOutOfSEQIntoEND(
	const ASTSchema& srcNodeSchema, 
	const ASTSchema& eventSchema, 
	const ASTSchema& destSchema, 
	SchemaMapString& outSchemaMap) {
	Assert (!destSchema.empty());
	Assert (!srcNodeSchema.intersection(eventSchema));

	for (ConstASTSchemaIterator it = destSchema.begin(); 
		it != destSchema.end(); ++it) {
		string attrName = it.getName();
		string outAttrName = it.getAliasName(); 

		//case 1: an attribute may store a UDF expression
		//case 2: for all attribute names foo, if these names can be found
		//in the event schema, they come from EV
		//case 3: otherwise they come from NODE
		string attrSrc = "EV";
		bool bInEventSchema = eventSchema.findAliasName(attrName);
		if (!bInEventSchema) {
			//not in the event stream schema, so must be in the 
			//source node schema!
			Assert (srcNodeSchema.findAliasName(attrName));
			attrSrc = "NODE";
		}
		else {
			Assert (!srcNodeSchema.findAliasName(attrName));
		}
		outSchemaMap.push_back(
			OldNewAttrPair(attrSrc, 
			attrName, 
			outAttrName));
	}
}

void SchemaMapBuilder::_constructSchemaMapForForwardEdgeOutOfMUIntoEND(
	const ASTSchema& prevEventSchema, 
	const ASTSchema& srcNodeSchema, 
	const ASTSchema& eventSchema, 
	const ASTSchema& destSchema, 
	const RefCayugaQLAST& muOpAST,
	SchemaMapString& outSchemaMap) {
	Assert (!destSchema.empty());
	Assert (prevEventSchema.contain(eventSchema));
	Assert (muOpAST != NULL);

	//case 1: an attribute may store a UDF expression.
	//case 2: dest attribute a: a in both ev and prevEv. src has a_1, a_n
	//				ev.a -> a
	//case 3: dest attribute a_1: a in both ev and prevEv, src has a_1, a_n
	//				node.a_1 -> a_1
	//case 4: dest attribute a: a in ev - prevEv, src has a_n
	//				node.a_n -> a
	for (ConstASTSchemaIterator it = destSchema.begin(); 
		it != destSchema.end(); ++it) {
		string attrName = it.getName();
		string outAttrName = it.getAliasName(); 

		if (!muOpAST->findUDF(attrName+"_n")) {
			if (prevEventSchema.findAliasName(attrName) 
				&& eventSchema.findAliasName(attrName)) {
				//case 2:
				Assert (srcNodeSchema.findAliasName(attrName + "_1"));
				Assert (srcNodeSchema.findAliasName(attrName + "_n"));
				outSchemaMap.push_back(
					OldNewAttrPair("EV", 
					attrName, 
					outAttrName)
				);
			}
			else if (prevEventSchema.findAliasName(attrName) 
				&& !eventSchema.findAliasName(attrName)) {
				//case 4:
				Assert (srcNodeSchema.findAliasName(attrName + "_n"));
				outSchemaMap.push_back(
					OldNewAttrPair("NODE", 
					attrName+"_n", 
					outAttrName)
				);
			}
			else {
				//assert it is in Case 3
				Assert (attrName.size() > 2);
				Assert (attrName[attrName.size()-2] == '_');
				Assert (attrName[attrName.size()-1] == '1');
				string attrNameWithoutRename 
					= attrName.substr(0, attrName.size()-2);
				Assert (srcNodeSchema.findAliasName(attrName));
				Assert (
					srcNodeSchema.findAliasName(attrNameWithoutRename+"_n"));
				outSchemaMap.push_back(
					OldNewAttrPair("NODE", 
					attrName, 
					outAttrName));
			}
		}
		else {
			//case 1
			outSchemaMap.push_back(
				OldNewAttrPair(muOpAST->getUDFExpr(attrName+"_n"), 
				"", 
				attrName,
				true));
		}
	}
}


void SchemaMapBuilder::_constructIdentitySchemaMapForUnaryExpr(
	const ASTSchema& eventAndDestSchema, 
	SchemaMapString& outSchemaMap) {
		_constructSchemaMapForForwardEdgeInOneStepAutomaton(
			eventAndDestSchema, 
			eventAndDestSchema, 
			RefCayugaQLAST(),
			outSchemaMap);
}


void SchemaMapBuilder::_constructSchemaMapForForwardEdgeInOneStepAutomaton(
	const ASTSchema& eventSchema, 
	const ASTSchema& destStateSchema, 
	const RefCayugaQLAST& selectAST,
	SchemaMapString& outSchemaMap) {
	Assert (!destStateSchema.empty());

	for (ConstASTSchemaIterator it = destStateSchema.begin(); 
		it != destStateSchema.end(); ++it) {
		string attrName = it.getName();
		string outAttrName = it.getAliasName(); 

		if (!selectAST || !selectAST->findUDF(outAttrName)) {
			Assert (eventSchema.findAliasName(attrName));
			outSchemaMap.push_back(
				OldNewAttrPair("EV", 
				attrName, 
				outAttrName));
		}
		else {
			outSchemaMap.push_back(
				OldNewAttrPair(selectAST->getUDFExpr(outAttrName), 
				"", 
				outAttrName,
				true));
		}
	}
}

void SchemaMapBuilder::_constructSchemaMapForForwardEdgeInToSEQ(
	const ASTSchema& eventSchema, 
	const ASTSchema& destStateSchema, 
	const ASTSchema& nextEventSchema, 
	SchemaMapString& outSchemaMap) {
	Assert (!destStateSchema.empty());

	//bool bNeedRename = (eventSchema.intersection(nextEventSchema));

	for (ConstASTSchemaIterator it = destStateSchema.begin(); 
		it != destStateSchema.end(); ++it) {
		string attrName = it.getName();
		string outAttrName = it.getAliasName(); 
		//outAttrName could be attrName_1
		//Assert (attrName == outAttrName);

		//attrName can always be found in eventSchema
		Assert (eventSchema.findAliasName(attrName));
		outSchemaMap.push_back(
			OldNewAttrPair("EV", 
			attrName, 
			outAttrName));
	}
}

//for forward edge pointing to mu only
void SchemaMapBuilder::_constructSchemaMapForForwardEdgeIntoMU(
	const ASTSchema& eventSchema,
	const ASTSchema& destSchema,
	SchemaMapString& outSchemaMap) {
	
	//some sanity checks here	
	Assert (!destSchema.empty());
	//for each attribute in destSchema, no matter whether it is suffixed with 
	//_1 or _n, check whether it comes from srcNodeSchema or nextEventSchema
	for (ConstASTSchemaIterator it = destSchema.begin(); 
		it != destSchema.end(); ++it) {
		string attrName = it.getName();
		string outAttrName = it.getAliasName(); 
		//outAttrName could be attrName_1 or attrName_n
			
		//each attrName has suffix _1 or _n
		Assert (outAttrName.size() > 2 
			&& outAttrName[outAttrName.size()-2] == '_'); 
		Assert (outAttrName[outAttrName.size()-1] == '1' 
			|| outAttrName[outAttrName.size()-1] == 'n');

		string inAttrAlias = outAttrName.substr(0, outAttrName.size()-2);
		Assert (attrName == inAttrAlias);
		
		//since we know eventSchema and srcNodeSchema are disjoint, each
		//attribute comes from either of the two places
		Assert (eventSchema.findAliasName(inAttrAlias));

		outSchemaMap.push_back(
			OldNewAttrPair("EV", 
			inAttrAlias, 
			outAttrName));
		
	}
}

//for rebind edge of the mu only
void SchemaMapBuilder::_constructSchemaMapForRebindEdge(
	const ASTSchema& pNextEventSchema,
	const ASTSchema& destSchema,
	const RefCayugaQLAST& muOpAST,
	SchemaMapString& outSchemaMap) {
	//for each attribute in destSchema, if it is suffixed with _1, it comes
	//from NODE. If it is suffixed with _n, need to check whether
	//it comes from pNextEventSchema. If so, attrSrc = EV. Otherwise,
	//it could be from NODE (preserved in this first iteration and never 
	//changed since, or computed by UDF
	Assert (muOpAST != NULL);
	for (ConstASTSchemaIterator it = destSchema.begin(); it != destSchema.end(); 
		++it) {
		string attrName = it.getName();
		string outAttrName = it.getAliasName(); 
		if (!muOpAST->findUDF(outAttrName)) {
			
			//each attrName has suffix _1 or _n
			Assert (outAttrName.size() > 2 
				&& outAttrName[outAttrName.size()-2] == '_'); 
			if (outAttrName[outAttrName.size()-1] == '1') {
				outSchemaMap.push_back(
					OldNewAttrPair("NODE", 
					outAttrName, 
					outAttrName));
			}
			else {
				Assert (outAttrName[outAttrName.size()-1] == 'n');
				string inAttrName = outAttrName.substr(0, outAttrName.size()-2);
				Assert (attrName == inAttrName);
				if (pNextEventSchema.findName(inAttrName)) {
					outSchemaMap.push_back(
						OldNewAttrPair("EV", 
						inAttrName, 
						outAttrName));
				}
				else {
					outSchemaMap.push_back(
						OldNewAttrPair("NODE", 
						outAttrName, 
						outAttrName));
				}
			}
		}
		else {
			outSchemaMap.push_back(
				OldNewAttrPair(muOpAST->getUDFExpr(outAttrName), 
				"", 
				outAttrName,
				true));
		}
	}
}
