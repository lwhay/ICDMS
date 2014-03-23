/**
* @file WindowGen.cpp
* @date March,24, 2011
* @brief This file defines WindowGen class.
* @author Rainbow (Rainbosworld@163.com)
*/
#include "P2TokenTypes.hpp"
#include "WindowGen.h"
#include "../ICDSMSCE/AutomatonCompiler/WindowString.h"

using namespace Cayuga::QLCompiler;


using namespace Cayuga::AutomatonCompiler;

#include "../ICDSMSCE/Utils/RAssert.h"
using namespace Cayuga::Utils;


void WindowGen::visit(RefCayugaQLAST& rootAst){
	Assert(rootAst->getType()==P2TokenTypes::QUERY);
	RefCayugaQLAST windAST=getWindowClauseChildFromQuery(rootAst);
	if (windAST)
	{
		WinType type;
		WinLength length;
		WinStep step;
		WinTime startime;

        RefCayugaQLAST childAST = static_cast<RefCayugaQLAST>(windAST->getFirstChild());
		string stype=childAST->getText();
        type = stype=="TIME"||stype=="time" ? /*WinType::*/TIMEWINDOW : /*WinType::*/COUNTWINDOW;

		childAST=childAST->getNextSibling();
		string slength=childAST->getText();
		length=atof(slength.c_str());

		childAST=childAST->getNextSibling();
		string sstep=childAST->getText();
		step=atof(sstep.c_str());

		childAST=childAST->getNextSibling();
		string sstartime=childAST->getText();
		startime=atof(sstartime.c_str());
		//rootAst->_thisWindow=new WindowString();

		rootAst->_thisWindow=new WindowString(type,length,step,startime);

	}


}

