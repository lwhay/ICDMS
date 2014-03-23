#include "BasicDataStructure/Window/predWindow.h"
#include <set>
#include <cassert>
#include <iostream>
using namespace std;

predWindow::predWindow(){}
predWindow::predWindow(predWinID id, predWinCount wincount,predWinTemporal wintemp):
					_predwinid(id),_predwincount(wincount),_predwintemporal(wintemp){}

void predWindow::write(ostream& out) const {
    out << "<PREDWINDOW predWindowID=\"" << _predwinid << "\" ";
    out << "TEMPORAL=\"" << _predwintemporal << "\" ";
    out << "COUNT=\"" << _predwincount << "\" >" << endl;
    out << "</PREDWINDOW>" << endl;
}

