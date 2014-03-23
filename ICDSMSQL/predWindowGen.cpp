#include "P2TokenTypes.hpp"
#include "predWindowGen.h"
#include <math.h>
#include <stdlib.h>
#include "../ICDSMSCE/AutomatonCompiler/predWindowString.h"


#include "CayugaQLASTWalker.h"
using namespace Cayuga::QLCompiler;
using namespace Cayuga::AutomatonCompiler;

#include "../ICDSMSCE/Utils/RAssert.h"
using namespace Cayuga::Utils;


void predWindowGen::visit(RefCayugaQLAST& rootAst){
// 	Assert(rootAst->getType()==P2TokenTypes::QUERY);
// 	RefCayugaQLAST streamExpr = getStreamExprChildFromQuery(rootAst);
//     RefCayugaQLAST binOpAST = getBinOpChildOfStreamExpr(streamExpr);
// 	RefCayugaQLAST predWinAST = getPredWindowClauseChildFromQuery(binOpAST);
// 	if (predWinAST)
// 	{	
// 		predWinCount predcount;
// 		predWinTemporal predtemporal;
// 		RefCayugaQLAST childAST =(RefCayugaQLAST)predWinAST->getFirstChild();
// 		if (childAST->getType()==P2TokenTypes::INT)
// 		{
// 			string exp = childAST->getText();
// 			predtemporal = atol(exp.c_str());
// 			childAST = (RefCayugaQLAST)childAST->getNextSibling();
// 		}		
// // 		Assert (childAST->getType() == P2TokenTypes::COMMA);
// // 		childAST = childAST->getNextSibling();
// 		if (childAST->getType()==P2TokenTypes::INT)
// 		{
// 			string exp1 = childAST->getText();
// 			predcount = atol(exp1.c_str());
// 		}
// 
// 		rootAst->_thisPredWindow= new predWindowString(predtemporal,predcount);
// 	}
	if (rootAst)
	{	
		predWinCount predcount;
		predWinTemporal predtemporal;
		RefCayugaQLAST childAST =(RefCayugaQLAST)rootAst->getFirstChild();

		//luoluo 8-19
		//add the following code to indicate the situation of no predicate window.
		if (childAST->getType()!=P2TokenTypes::PRED_WINDOW)
		{
			rootAst->_thisPredWindow= NULL;
		}
		else
		{
			Assert(childAST->getType()==P2TokenTypes::PRED_WINDOW);
			 childAST = (RefCayugaQLAST)childAST->getFirstChild();
			if (childAST->getType()==P2TokenTypes::INT)
			{
				string exp = childAST->getText();
				predtemporal = atol(exp.c_str());
				childAST = (RefCayugaQLAST)childAST->getNextSibling();
			}		
			// 		Assert (childAST->getType() == P2TokenTypes::COMMA);
			// 		childAST = childAST->getNextSibling();
			if (childAST->getType()==P2TokenTypes::INT)
			{
				string exp1 = childAST->getText();
				predcount = atol(exp1.c_str());
			}

			rootAst->_thisPredWindow= new predWindowString(predtemporal,predcount);
		}
	}

}

