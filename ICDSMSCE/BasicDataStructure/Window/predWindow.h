#ifndef  _PREDWINDOW_H
#define _PREDWINDOW_H

#include "Utils/Singleton.h"
using Cayuga::Utils::Singleton;
#include "BasicDataStructure/Window/predWindowSchema.h"
using namespace Cayuga::BasicDataStructure::predWindowSchema;


#include <set>
#include <cassert>
#include <string>
using namespace std;
using namespace Cayuga;

// typedef WinLength WindowSize;
// typedef WinStep SlideStep;
// typedef WinTime Start;
// typedef WinID WindowID;
// typedef WinType WindowType;


class predWindow 
{

public:
	/**
	*@brief		Default constructor.
	*/
	predWindow();
    predWindow(predWinID id, predWinCount wincount,predWinTemporal wintemp);
	~predWindow(){}
	/**
	*@brief		Get and set the information of the predicate window.
	*/
   void setPredWinCount(predWinCount predcount)
   {
	   _predwincount = predcount;
   }
   void setPredWinTemporal(predWinTemporal predtemporal)
   {
	   _predwintemporal = predtemporal;
   }
   void setPredWinId(predWinID predid)
   {
	   _predwinid = predid;
   }
   predWinID getPredWinId()
   {
	   return _predwinid;
   }
   predWinCount getPredWinCount()
   {
	   return _predwincount;
   }
   predWinTemporal getPredWinTemporal()
   {
	   return _predwintemporal;
   }

	/**
	*@brief Output member variable information in XML format.
	*/
	virtual void write(ostream& out) const;
private:
	/**
	*@This following three variables are used to identify the 
	  information of the window.
	*/
    predWinID  _predwinid;
	predWinCount _predwincount;
	predWinTemporal _predwintemporal;
};
#endif