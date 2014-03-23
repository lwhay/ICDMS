/**
 * @file Timer.h
 * @date November 22, 2005
 * @brief Defines the high resolution Timer class for measuring time cost of 
		  components in the system.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _TIMER_H
#define _TIMER_H

#include <math.h>
#include <map>
#include <string>
#include <set>
#include <vector>
#include <cstdlib>
#include <cfloat>
#include <iostream>
using namespace std;

#ifdef WIN32
#include <windows.h>
//#include <windef.h>
#else
#include <sys/time.h>
#endif 

namespace Cayuga {
namespace Utils {
#ifdef WIN32
/**
 * @brief Query the performance counter
 * @param p The performance counter to be set
 * @return For platform compatibility, assume it always returns true.
 * @date November 22, 2005
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @version 1.0
 */
inline bool QueryPerfCount(long long *p) {
	BOOL result = QueryPerformanceCounter((LARGE_INTEGER *)p); 
	return (result == TRUE)? true : false;
}

/**
 * @brief Query the performance frequency
 * @param p The performance frequency to be set
 * @return For platform compatibility, assume it always returns true.
 * @date November 22, 2005
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @version 1.0
 */
inline bool QueryPerfFreq(long long *p) { 
	BOOL result = QueryPerformanceFrequency((LARGE_INTEGER *) p); 
	return (result == TRUE)? true : false;
}

#else
//#elif defined(__GLIBCPP__) || defined(__GLIBCXX__)  // Using GNU C++ library?
//the trick is to use microseconds as the counting unit
//when returning the final result, freq will be divided. by setting it to 1000, 
//we ensure the returned value unit is millisecond.

inline bool QueryPerfCount(long long *p) {
	timeval start; gettimeofday(&start, NULL); 
	*p = start.tv_sec * 1000000 + start.tv_usec; 
	return true;
}

inline bool QueryPerfFreq(long long *p) {
	*p = 1000; 
	return true;
}
#endif //WIN32

/**
 * @brief A crTime object represents a timer. 
		  It calculates the difference of cpu ticks to measure the time elapse
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 22, 2005
 * @version 1.0
 */
class crTime {
public:
	/**
	* @brief Default constructor.
	* @param name Name of the timer
	* @date November 22, 2005
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @version 1.0
	*/
	crTime(const string& name);
	
	/**
	* @brief Start the timer.
	* @date November 22, 2005
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @version 1.0
	*/
	void inline start() {
		QueryPerfCount(&lastStart);
	}
	
	/**
	* @brief Stop the timer.
	* @date November 22, 2005
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @version 1.0
	*/
	void inline stop() {
		long long end; 
		QueryPerfCount(&end);
		total += (end - lastStart);
	}
	
	/**
	* @brief Reset and start the timer.
	* @date November 22, 2005
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @version 1.0
	*/
	void inline resetAndStart() {
		total = 0;
		QueryPerfCount(&lastStart);
	}

	/**
	* @brief Print the value of the timer to screen.
	* @return As above
	* @param out The output stream handle.
	* @date November 22, 2005
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @version 1.0
	*/
	void printTimer(ostream& out = cout); 

	/**
	* @brief Return the value of the timer in microsecond.
	* @return As above
	* @date March 13, 2007
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @version 1.0
	*/
	long long returnTimerInMicroSeconds();

	/**
	* @brief Return the value of the timer in millisecond.
	* @return As above
	* @date November 22, 2005
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @version 1.0
	*/
	long long returnTimerInMilliSeconds();
	
	/**
	* @brief Return the value of the timer in second.
	* @return As above
	* @date November 22, 2005
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @version 1.0
	*/
	long double returnTimerInSeconds();

	/**
	* @brief Return the total counter value of cpu.
	* @return As above
	* @date March 7, 2007
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @version 1.0
	*/
	long long returnCounter();
private:
	/**
	* This variable stores the string name of the timer.
	*/
	string name;
	
	// these are in cpu ticks
	/**
	* This variable stores CPU counter of the start time.
	*/
	long long lastStart;
	
	/**
	* This variable stores the difference of the CPU counter for end time and
	* that for start time.
	*/
	long long total;
	
	/**
	* This variable stores the frequency of the CPU counter.
	*/
	long long freq;
}; //class crTime


/**
 * @brief A Timer object wraps crTime objects to have multiple timers, 
 * used for different purposes. 
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 22, 2005
 * @version 1.0
 */
class Timer
{
	typedef map<string, crTime*> crTimerMap; 
public:		
	/**
	* @brief Default constructor.
	* @date November 22, 2005
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @version 1.0
	*/
	Timer(void);
	
	/**
	* @brief Destructor.
	* @date November 22, 2005
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @version 1.0
	*/
	~Timer(void);

	/**
	* @brief Add the timer of a given name.
	* @name The name of the timer to add.
	* @date November 22, 2005
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @version 1.0
	*/
	void addTimer( const string& name);
	
	/**
	* @brief Start the timer of a given name.
	* @name The name of the timer to start.
	* @date November 22, 2005
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @version 1.0
	*/
	void startTimer( const string& name);
	
	/**
	* @brief Stop the timer of a given name.
	* @name The name of the timer to stop.
	* @date November 22, 2005
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @version 1.0
	*/
	void stopTimer( const string& name);
	
	/**
	* @brief Reset and start the timer of a given name.
	* @name The name of the timer to reset and start.
	* @date November 22, 2005
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @version 1.0
	*/
	void resetAndStartTimer(const string& name);
	
	/**
	* @brief Add and start the timer of a given name.
	* @name The name of the timer to add and start.
	* @date November 22, 2005
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @version 1.0
	*/
	void addAndStartTimer( const string& name );

	/**
	* @brief Print the timer value of a given name to screen.
	* @param name The name of the timer to print.
	* @param out The output stream handle.
	* @date November 22, 2005
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @version 1.0
	*/
	void printTimer( const string& name, ostream& out = cout);
	
	/**
	* @brief Print all timer values.
	* @param out The output stream handle.
	* @date November 22, 2005
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @version 1.0
	*/
	void printAllTimers(ostream& out = cout);
	
	/**
	* @brief Return the value of the timer in millisecond.
	* @name The name of the timer to return.
	* @return As above
	* @date November 22, 2005
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @version 1.0
	*/
	long long returnTimerInMilliSeconds(const string& name);
	
	/**
	* @brief Return the value of the timer in second.
	* @name The name of the timer to return.
	* @return As above
	* @date November 22, 2005
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @version 1.0
	*/
	long double returnTimerInSeconds(const string& name);
private:
	/**
	* This variable stores the map from timer names to their values.
	*/
	crTimerMap ct;
}; //class Timer
}  //namespace Utils
}  //namespace Cayuga

#endif //_TIMER_H
