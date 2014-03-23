/**
 * @file Timer.cpp
 * @date November 22, 2005
 * @brief The is the implementation file of crTime and Timer classes.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "Utils/Timer.h"
using namespace Cayuga::Utils;

#include <iostream>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// crTime member function definitions
////////////////////////////////////////////////////////////////////////////////

crTime::crTime(const string& name) {
	//printf( "crTime::CrTime %s %X\n", 
	//	name.c_str(), this);
	this->name = name;
	total = 0;		
	QueryPerfFreq(&freq);
	if(freq == 0) {
		cerr<<"FREQ ERROR"<<endl;
		exit(-1);
	}
}
void crTime::printTimer(ostream& out) {
	out<<name.c_str();
#ifdef WIN32
	long long ms = (total*1000)/freq;
	out<<" total time: "<<total<<" ticks or "<<ms<<"ms"<<endl;
#else //unix
	long long ms = total/freq;
	out<<" total time: "<<ms<<" ms"<<endl;
#endif //WIN32
}

long long crTime::returnTimerInMicroSeconds() {
#ifdef WIN32
	long long mu_s = (total*1000*1000)/freq;
	return mu_s;
#else //unix
	long long mu_s = (total*1000)/freq;
	return mu_s;
#endif
}

long long crTime::returnTimerInMilliSeconds() {
#ifdef WIN32
	long long ms = (total*1000)/freq;
	return ms;
#else //unix
	long long ms = total/freq;
	return ms;
#endif
}

long double crTime::returnTimerInSeconds() {
#ifdef WIN32
	long double ms = (long double)((long double)total / (long double)freq);
	return ms;
#else //unix
	long double ms = (long double)((long double)total / (long double)freq) 
		/ (long double)1000;
	return ms;
#endif
}

long long crTime::returnCounter() {
	return total;	
}

////////////////////////////////////////////////////////////////////////////////
// Timer member function definitions
////////////////////////////////////////////////////////////////////////////////

Timer::Timer(void) {

}
Timer::~Timer(void) {
}

void Timer::addTimer(const string& name) {	
	ct.insert(crTimerMap::value_type(name, new crTime(name)));
}

void Timer::startTimer(const string& name) {	
	crTimerMap::iterator it = ct.find(name);
	if(it != ct.end()) (*it).second->start();
	else cerr<<"startTimer: TIMER NOT FOUND!"<<endl;
}

void Timer::stopTimer(const string& name) {	
	crTimerMap::iterator it = ct.find(name);
	if(it != ct.end()) (*it).second->stop();
	else cerr<<"stopTimer: TIMER NOT FOUND!"<<endl;
}

void Timer::resetAndStartTimer(const string& name) {	
	crTimerMap::iterator it = ct.find(name);
	if(it != ct.end()) (*it).second->resetAndStart();
	else cerr<<"stopTimer: TIMER NOT FOUND!"<<endl;
}

void Timer::addAndStartTimer(const string& name) {
	addTimer(name);
	startTimer(name);
}

void Timer::printTimer(const string& name, ostream& out) {
	crTimerMap::iterator it = ct.find(name);	
	if(it != ct.end()) (*it).second->printTimer(out);
	else cerr<<"printTimer: TIMER "<<name.c_str()<<" NOT FOUND!"<<endl;
}

void Timer::printAllTimers(ostream& out) {	
	for(crTimerMap::iterator it = ct.begin();
		it != ct.end(); it++)
		(*it).second->printTimer(out);
}

long long Timer::returnTimerInMilliSeconds(const string& name) {
	crTimerMap::iterator it = ct.find(name);		
	if(it != ct.end()) return (*it).second->returnTimerInMilliSeconds();
	else {
		cerr<<"returnTimer: TIMER "<<name.c_str()<<" NOT FOUND!"<<endl;
		return 0;
	}
}

long double Timer::returnTimerInSeconds(const string& name) {
	crTimerMap::iterator it = ct.find(name);	
	if(it != ct.end()) return (*it).second->returnTimerInSeconds();
	else {
		cerr<<"returnTimer: TIMER "<<name.c_str()<<" NOT FOUND!"<<endl;
		return 0;
	}
}
