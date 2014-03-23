#ifndef _CY_IntStringFace_H_
#define _CY_IntStringFace_H_

/*
** Cayuga Internalized strings ...
**
** Nov 2005 ajd:
**	Created
** May 2006 ajd:
**	Thread-safe
*/

#include "CY_HeapFace.h"

#include <string>

namespace CY_IntString {

//
// String object returned to clients ...
//
class IntString : public CY_Heap::Object {
public:
	static size_t const numBytes = 1;
	static unsigned const numHandles = 0;
	static unsigned const numWeak = 0;
	//
private:
	char is_body[1];
		// text body as a (null-terminated) C string
	friend class IntStringTbl;
	IntString() {}
	~IntString() {}
public:
	const char * getBody() const { return (const char *)(is_body); }
		// (readonly) body of string
	char * getBodyForWrite() { return (char *)(is_body); }
};

typedef CY_Heap::THandle<IntString> IntStringHandle;

unsigned inline getLen(IntStringHandle h) { return h.getSize()-1; }
	// (cheaper than strlen()

//
// Convert C string to internalized string ...
// if the string is not already internalized,
//   if install is true, internalize a copy of s
//   if install is false, just return a null Handle
//
	
IntStringHandle lookup(const char * s, bool install);

//
// Convert a heap string object to internalized string ...
// if the string is already internalized
//   return the already internalized IntStringHandle
// if the string is not already internalized,
//   if install is true, internalize (shared) sh and return it
//   if install is false, just return a null Handle
//
// Example:
//
// IntStringHandle sh = IntStringHandle(len, true);
//   // len is strlen( the string you want to insert )
// memcpy( &(*sh), somebytes, length+1 );
//   // +1 for trailing null!
// sh = lookup(sh, true);
//

IntStringHandle lookup(IntStringHandle ish, bool install);

//added by Mingsheng 07.04.14
//if externalSpace is true, insert the input string into external space.
//Otherwise insert it into from space.
IntStringHandle lookup(const std::string& str, bool install, bool externalSpace);

//
// Statistics:
//
//   *pCnt = approximate number of internalized strings
//   *pBytes = approximate total bytes of internalized strings
//   *pLookup = # lookups since last reset
//	 *pFail = # lookups that failed (returned NULL) since last reset
//   *pInstall = # new strings installed since last reset
//
//   any NULL argument pointers are just ignored
//
void getStatistics(unsigned * pCnt, unsigned * pBytes,
	unsigned * pLookup, unsigned * pFail, unsigned * pInstall,
	bool reset = false );

} /* CY_IntString */

#endif /* _CY_IntStringFace_H_ */
