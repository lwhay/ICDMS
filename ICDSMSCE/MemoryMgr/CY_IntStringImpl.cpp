/*
** Cayuga Internalized Strings
**
** October 2005 ajd:
**	Created
** May 2006 ajd:
**	Thread-safe
*/


// TODO: tune the sweep process!


#include "MemoryMgr/CY_IntStringFace.h"
#include "MemoryMgr/CY_HeapFace.h"
#include "MemoryMgr/CY_GCFace.h"
#include "MemoryMgr/CY_ErrLog.h"

#include "corelib/os/threads/Mutex.h"

#include <cstring>

#include <string>
using std::string;


using namespace CY_Heap;
using namespace cudb::corelib::os::threads;

namespace CY_IntString {



//
// Each hash header cell contains a list of nodes ...
//
struct Node : public Object {
	static size_t const numBytes = 12;
	static unsigned const numHandles = 1;
	static unsigned const numWeak = 1;
	//
	THandle<Node> node_next;
	IntStringHandle node_value;
	unsigned node_hash;
};

typedef THandle<Node> NodeHandle;


//
// The hash table is a struct pointing to an array of header handles ...
//
struct IntStringTbl : public Object {
	//
	// tuning parameters ...
	//
	static unsigned const ist_numHdrsINIT = 1024;
	static unsigned const ist_numHdrsMAX = CY_Heap::Object::numHandlesMAX;
	static double const ist_coeffGROW;
//	static unsigned const ist_sweepINTERVAL = 16;
	static unsigned const ist_sweepINTERVAL = 16;
//	static unsigned const ist_sweepCHUNK = 2;
	static unsigned const ist_sweepCHUNK = 2;
	//
	// memory representation ...
	//
	//invariant: ist_nodes must be the first member field in this class
	THandleArrayHandle<Node> ist_nodes;
	unsigned ist_numEntries;
	unsigned ist_sweep;
	unsigned ist_sincesweep;
	//
	// statistics ...
	//
	unsigned ist_numBytes;
	unsigned ist_numBytesWork;
	unsigned ist_numLookup;
	unsigned ist_numFail;
	unsigned ist_numInstall;
	//
	//invariant: numBytes must be equal to the sum of bytes taken by the 
	//member fields of IntStringTbl
	//static size_t const numBytes = 16;
	//we added 5 more variables for stats
	static size_t const numBytes = 16+20; 
	static unsigned const numHandles = 1;
	//
	// methods ...
	//
	static void hashString(const char *s, unsigned *hashp, unsigned *lenp);
		// hash function
	IntStringHandle lookup(const char * s, IntStringHandle ais, bool install);
		// lookup / insert C string s or IntStringHandle ais
		// exactly one of {s, ais} must be null
		// if ais is supplied and installed, it will be shared rather than copied
	void grow();
		// grow the hash header array if possible
	void sweep();
		// incrementally clean up dangling weak refs
	unsigned checkInvariants();
	int gec(unsigned bucket);
};

double const IntStringTbl::ist_coeffGROW = 1.5;

////////////////

unsigned IntStringTbl::checkInvariants() {
	THandleArrayHandle<Node> hdrs = ist_nodes;
	unsigned numHdrs = hdrs.getSize();
	unsigned nle = 0;
	unsigned nde = 0;
	for( unsigned i = 0; i < numHdrs; i++ ) {
		for( NodeHandle p = hdrs[i]; !p.isNull(); p = p->node_next ) {
			Node * np = &(*p);
			if( (np->node_hash % numHdrs) != i ) DIE();
			if( !np->node_value.isNull() ) {
				IntString * is = &(*np->node_value);
				if( getLen(np->node_value) != strlen(is->is_body) ) DIE();
				nle += 1;
			} else {
				nde += 1;
			}
			if( (nle + nde) > ist_numEntries ) DIE();
		}
	}
	if( (nle + nde) != ist_numEntries ) DIE();
	return nle;
}

#undef	CHECK_INVARIANTS
#define	CHECK_INVARIANTS()	do { checkInvariants(); } while(0)

#undef	CHECK_INVARIANTS
#define	CHECK_INVARIANTS()	do {} while(0)

////////////////

//
// ELF hash function
//
inline void IntStringTbl::hashString(const char *s, unsigned *hashp, unsigned *lenp) {
	unsigned hash = 0;
	const unsigned char *p = (const unsigned char *)(s);
	unsigned char c;
	unsigned x;
	
	while( (c = *p++) != 0 ) {
		hash = (hash << 4) + c;
		if( (x = hash & 0xF0000000L) != 0 ) {
			hash ^= (x >> 24);
			hash &= ~x;
		}
	}
	*hashp = (hash & 0x7FFFFFFF);
	*lenp = (p - 1) - (const unsigned char *)(s);
}

//
// lookup / insert
//

inline IntStringHandle IntStringTbl::lookup(
		const char * s, IntStringHandle ais, bool install) {
	CY_ASSERT_DEBUG( (s == 0) ^ ais.isNull() );
	ist_numLookup += 1;
	if( s == 0 ) s = ais->getBody();
	if( ++ist_sincesweep >= ist_sweepINTERVAL ) sweep();
	THandleArrayHandle<Node> hdrs = ist_nodes;
	if( hdrs.isNull() ) {
		hdrs = ist_nodes = THandleArrayHandle<Node>(ist_numHdrsINIT, 1);
	}
	unsigned numHdrs = hdrs.getSize();
	unsigned hashval, len;
	hashString(s, &hashval, &len);
	unsigned i = hashval % numHdrs;
	NodeHandle p;
	// look up requested string ...
	CHECK_INVARIANTS();
	for( p = hdrs[i]; !p.isNull(); p = p->node_next ) {
		Node * np = &(*p);
		if( np->node_hash != hashval ) continue;
		if( np->node_value.isNull() ) continue;
		// if( getLen(np->node_value) != len ) continue; (probably not worthwhile)
		if( strcmp(np->node_value->is_body, s) != 0 ) continue;
		// found it ...
		return np->node_value;
	}
	// not found 
	if( !install ) {
		ist_numFail += 1;
		return IntStringHandle(); // null
	}
	ist_numInstall += 1;
	// create string value ...
	IntStringHandle is = ais;
	if( is.isNull() ) {
		is = IntStringHandle(len);
		strcpy( is->getBodyForWrite(), s);
	}
	// create node and insert it ...
	NodeHandle pnew = NodeHandle(/*extraBytes=*/0, /*ext=*/1);
	pnew->node_hash = hashval;
	pnew->node_value = is;
	pnew->node_next = hdrs[i];
	hdrs[i] = pnew;
	ist_numEntries += 1;
	CHECK_INVARIANTS();
	// return string value ...
	return is;
}

//
// Grow the headers array ...
//
void IntStringTbl::grow() {
	THandleArrayHandle<Node> oldHdrs = ist_nodes;
	unsigned oldNumHdrs = oldHdrs.getSize();
	unsigned newNumHdrs = ((unsigned)(ist_coeffGROW * oldNumHdrs));
	if( newNumHdrs > ist_numHdrsMAX ) newNumHdrs = ist_numHdrsMAX;
	THandleArrayHandle<Node> newHdrs = THandleArrayHandle<Node>(newNumHdrs, 1);
	for( unsigned i = 0; i < oldNumHdrs; i++ ) {
		NodeHandle p;
		NodeHandle nextp;
		for( p = oldHdrs[i]; !p.isNull(); p = nextp ) {
			Node * np = &(*p);
			nextp = np->node_next;
			if( np->node_value.isNull() ) {
				ist_numEntries -= 1;
				continue;
			}
			unsigned inew = np->node_hash % newNumHdrs;
			np->node_next = newHdrs[inew];
			newHdrs[inew] = p;
		}
	}
	ist_nodes = newHdrs;
}

//
// Sweep one cell of the headers array looking for dead weak refs ...
//
void IntStringTbl::sweep() {
	THandleArrayHandle<Node> hdrs = ist_nodes;
	unsigned numHdrs = hdrs.getSize();
	for( int i = 0; i < ist_sweepCHUNK; i++ ) {
		if( ++ist_sweep >= numHdrs ) {
			ist_sweep = 0;
			ist_numBytes = ist_numBytesWork;
			ist_numBytesWork = 0;
		}
		NodeHandle prev;
		NodeHandle p(hdrs[ist_sweep]);
		while( !p.isNull() ) {
			if( p->node_value.isNull() /* found deleted weak ref */ ) {
				// remove *np from hash table ...
				if( prev.isNull() ) {
					hdrs[ist_sweep] =  p->node_next;
				} else {
					prev->node_next =  p->node_next;
				}
				ist_numEntries -= 1;
			} else {
				prev = p;
				ist_numBytesWork += 1+getLen(p->node_value);
			}
			p = p->node_next;
		}
	}
	if( (ist_numEntries > numHdrs) && (numHdrs < ist_numHdrsMAX) ) {
		grow();
	}
	ist_sincesweep = 0;
}


Mutex theIntStringsMutex;

THandle<IntStringTbl> theIntStrings;

void initTheIntStrings() {
	if( theIntStrings.isNull() ) {
		theIntStrings = THandle<IntStringTbl>(/*extraBytes=*/0, /*ext=*/1);
		//Mingsheng 06.05.15
		//Since the constructor for IntStringTbl is never called, initialize
		//the following fields correctly.

		//modified 07.04.25
		THandleArrayHandle<Node>& ist_nodes = theIntStrings->ist_nodes;
		CY_ASSERT(ist_nodes.isNull());
		theIntStrings->ist_numBytes = 0;
		theIntStrings->ist_numBytesWork = 0;
		theIntStrings->ist_numLookup = 0;
		theIntStrings->ist_numFail = 0;
		theIntStrings->ist_numInstall = 0;
		theIntStrings->ist_nodes = THandleArrayHandle<Node>();
		// this should have happened automatically (ajd)
		theIntStrings->ist_numEntries = 0;
		theIntStrings->ist_sweep = 0;
		theIntStrings->ist_sincesweep = 0;

		registerRootVar(&theIntStrings);
	}
}

IntStringHandle lookup(const char * s, bool install) {
	IntStringHandle h;
	{ Lock theLock = theIntStringsMutex.scopedLock();
		if( theIntStrings.isNull() ) initTheIntStrings();
		h = theIntStrings->lookup(s, IntStringHandle(), install);
	}
	return h;
}

IntStringHandle lookup(IntStringHandle ish, bool install) {
	IntStringHandle h;
	{ Lock theLock = theIntStringsMutex.scopedLock();
		if( theIntStrings.isNull() ) initTheIntStrings();
		h = theIntStrings->lookup(0, ish, install);
	}
	return h;
}

IntStringHandle lookup(const string& str, bool install, bool externalSpace) {
	if (!externalSpace) {
		return lookup(str.c_str(), install);
	}
	else {
		size_t strLen = str.size();
		IntStringHandle h(strLen, true);
		char* hBody = h->getBodyForWrite();
		memcpy(hBody, str.c_str(), strLen);
		hBody[strLen] = '\0';

		//this overloaded lookup function will just link to the 
		//string already copied to external space
		return lookup(h, install);
	}
}

//
//
// Statistics ...
//
//

void getStatistics(
	unsigned * pCnt, unsigned * pBytes,
	unsigned * pLookup, unsigned * pFail, unsigned * pInstall,
	bool reset
){
	{ Lock theLock = theIntStringsMutex.scopedLock();
		if( theIntStrings.isNull() ) initTheIntStrings();

		if( pCnt ) *pCnt = theIntStrings->ist_numEntries;
		if( pBytes ) *pBytes = theIntStrings->ist_numBytes;
		if( pLookup ) *pLookup = theIntStrings->ist_numLookup;
		if( pFail ) *pFail = theIntStrings->ist_numFail;
		if( pInstall ) *pInstall = theIntStrings->ist_numInstall;
		if( reset ) {
			theIntStrings->ist_numLookup = 0;
			theIntStrings->ist_numFail = 0;
			theIntStrings->ist_numInstall = 0;
		}
	}
}

} /* namespace CY_IntString */
