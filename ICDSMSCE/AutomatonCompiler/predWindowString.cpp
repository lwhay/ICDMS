#include "AutomatonCompiler/predWindowString.h"
using namespace Cayuga::AutomatonCompiler;

#include "Utils/AttrMap.h"
using namespace Cayuga::Utils;

long predWindowString::predWindIdCount = 0;

predWindowString ::predWindowString(){}

predWindowString::predWindowString(long tem,long num)
				:predTemporal(tem),predCount(num)
{
	predWindIdCount ++;
	predID = predWindIdCount;
}

predWindowString::predWindowString(const StringMap &attrmap)
{
	predCount = getParameterValue<predWinCount>(attrmap,"COUNT");
	predTemporal = getParameterValue<predWinTemporal>(attrmap,"TEMPORAL");
	predWindIdCount++;
	predID = predWindIdCount;
}

predWinID predWindowString ::getPredWinID()
{
	return predID;
}

predWinCount predWindowString::getPredWinCount ()
{
	return predCount;
}

predWinTemporal predWindowString::getPredWinTemporal()
{
	return predTemporal;
}

void predWindowString::setPredWinCount(predWinCount predCoun)
{
	predCount = predCoun;
}

void predWindowString::setPredWinTemporal(predWinTemporal predTemp)
{
	predTemporal= predTemp;
	
}

void predWindowString::write(ostream &out ){
	out<<"<PREDWINDOW predWindowID=\""<<getPredWinID()<<"\" ";
    out<<"TEMPORAL=\""<<getPredWinTemporal()<<"\" ";
	out<<"COUNT=\""<<getPredWinCount()<<"\" >"<<endl;
	out<<"</PREDWINDOW>"<<endl;
}

predWindow * predWindowString ::recompile(Context context)
{
	predWindow *pWindow = new predWindow(predID,predCount,predTemporal);
	return pWindow;
}