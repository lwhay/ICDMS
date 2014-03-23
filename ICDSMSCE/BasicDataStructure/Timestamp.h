#ifndef _TIME_STAMP_H
#define _TIME_STAMP_H

//#include <boost/date_time/local_time/local_time.hpp>
//#include <boost/date_time/posix_time/posix_time.hpp>
//
////to be replaced with Boost time
//class Time {
//public:
//	bool operator == (const Time& rhs) {
//		return true;
//	}
//
//	bool operator > (const Time& rhs) {
//		return true;
//	}
//};
//
//class TimeStamp {
//public:
//	Time start;
//	Time end;
//};

namespace Cayuga {
namespace BasicDataStructure {
typedef long long TimeStamp;
} //namespace BasicDataStructure
} //namespace Cayuga

#endif //_TIME_STAMP_H
