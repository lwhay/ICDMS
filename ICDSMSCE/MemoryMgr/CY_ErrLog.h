#ifndef _CY_ErrLog_H_
#define _CY_ErrLog_H_

namespace CY_ErrLog {

void die0(const char * fname, int lineno);

};

#define DIE()	do { CY_ErrLog::die0(__FILE__, __LINE__); } while(0)

#if defined(CY_DEBUG)
#	define CY_ASSERT_DEBUG(b) do { if( !(b) ) DIE(); } while(0)
#else
#	define CY_ASSERT_DEBUG(b) do {} while(0)
#endif

inline void CY_ASSERT(bool b) { if( !b ) DIE(); }

#endif /* _CY_ErrLog_H_ */
