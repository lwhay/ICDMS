/**
* @file WindowString.h
* @date March 23, 2011
* @brief Base class of window read from AIR.
* @author Rainbow (rainbowsworld@163.com)
*/
#include "AutomatonCompiler/WindowString.h"
//#include "BasicDataStructure/Window/Window.h"
using namespace Cayuga::AutomatonCompiler;
using namespace Cayuga::BasicDataStructure::WindowSchema;

#include "Utils/AttrMap.h"
using namespace Cayuga::Utils;

long WindowString::winIDCount=0;
WindowString::WindowString()
{

}
WindowString::WindowString(WinType _winType, WinLength _length , WinStep _step , WinTime _startTime ):
winType(_winType),winLength(_length),
winStep(_step),startTime(_startTime){
	winIDCount++;
	winID=winIDCount;
}	
WindowString::WindowString(const StringMap &attrmap)
{
//    winType=getParameterValue<enum WinType>(attrmap,"WindowType");
//	winLength=getParameterValue<WinLength>(attrmap,"WindowSize");
//	winStep=getParameterValue<WinStep>(attrmap,"SlideStep");
//	startTime=getParameterValue<WinTime>(attrmap,"Start");
    setWinType(getParameterValue<string>(attrmap,"WindowType"));
    setWinLength(getParameterValue<string>(attrmap,"WindowSize"));
    setWinStep(getParameterValue<string>(attrmap,"SlideStep"));
    setWinStartTime(getParameterValue<string>(attrmap,"Start"));

    winIDCount++;
	winID=winIDCount;

}
WinType  WindowString::getWinType()
{
	return winType;
}
WinLength WindowString::getWinLength(){
	return winLength;
}
WinStep WindowString::getWinStep(){
	return winStep;
}
WinID WindowString::getWindowID(){
	return winID;
}
WinTime WindowString::getWinStartTime(){
	return startTime;

}
void WindowString::setWinType(WinType _winType){
	winType=_winType;

}
void WindowString::setWinLength(WinLength _length){
	winLength=_length;
}
void WindowString::setWinStep(WinStep _step){
	winStep=_step;
}
void WindowString::setWindowID(WinID id)
{
	winID=id;
}
void WindowString::setWinStartTime(WinTime _startTime){
	startTime=_startTime;
}


void WindowString::write(ostream &out ){
	out<<"<Window WindowID=\""<<getWindowID()<<"\" ";
	string type="";
	switch (getWinType())
	{
    case /*WinType::*/COUNTWINDOW:
		type="COUNTWINDOW";
		break;
    case /*WinType::*/TIMEWINDOW:
		type="TIMEWINDOW";
		break;

	}
	out<<"WindowType=\""<<type.c_str()<<"\" ";
	out<<"WindowSize=\""<<getWinLength()<<"\" ";
	out<<"SlideStep=\""<<getWinStep()<<"\" ";
	out<<"Start=\""<<getWinStartTime()<<"\" >"<<endl;
	out<<"</Window>"<<endl;

}

inputWindow * WindowString ::recompile(Context context){
	inputWindow *pinwindow = new inputWindow(winID,winLength,winType,winStep,startTime);
	return pinwindow;
} 

void WindowString::setWinType(const string& setwintype){
    if(setwintype == "TIMEWINDOW")winType = TIMEWINDOW;
    else
        winType = COUNTWINDOW;
}

void WindowString::setWinLength(const string& setwinlength){
    winLength = atof(setwinlength.c_str());
}

void WindowString::setWinStep(const string& setwinstep){
    winStep = atof(setwinstep.c_str());
}

void WindowString::setWinStartTime(const string& setwinstarttime){
    startTime = atof(setwinstarttime.c_str());
}
