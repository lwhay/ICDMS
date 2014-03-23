#ifndef	_CY_HeapPvt_H_
#define	_CY_HeapPvt_H_

#include "CY_HeapFace.h"
#include "CY_GCFace.h"
#include "CY_ErrLog.h"

#include "corelib/os/threads/Mutex.h"

#include <cstddef>
#include <cstdlib>
#include <cstring>

namespace CY_Heap {

class BufferedRootFinder;
class ObjectSpace;
class Indir;
class IndirList;
class IndirSpace;

////////////
// History HACK ... TODO: (ajd) fix this later!
/*
TODO: eventually there will be a params object passed
in place of `ext' and an unpin call on it ...
*/

#define HISTORY_SIZE	16
#if (HISTORY_SIZE > 0)

extern HandleArrayHandle historyHandles;
extern unsigned historyIndex;

#define HISTORY_ADD(h) do {\
	historyHandles[historyIndex] = (h);\
	if( (++historyIndex) >= HISTORY_SIZE ) historyIndex = 0;\
} while(0)

#endif /* HISTORY_SIZE */
//
////////////

/*
** Locks
*/

extern cudb::corelib::os::threads::Mutex autoMutex;
#define ALLOCA_MUTEX	autoMutex
#define GC_MUTEX		autoMutex

extern cudb::corelib::os::threads::Mutex manualMutex;
#define ALLOCM_MUTEX	manualMutex

/*
** Tunable parameters.
*/

// default heap space bytes ...
extern size_t heapSpaceBytes;
inline void setHeapSpaceBytesWithDefault(size_t nb) {
	heapSpaceBytes = ( nb
		? nb
		: 8*1024*1024	/* default */
	);
}

// default number of objects ...
extern unsigned maxNumObj;
inline void setMaxNumObjWithDefault(unsigned nObj) {
	maxNumObj = ( nObj
		? nObj
		: 16*1024		/* default */
	);
}

// default for max expected internal auto object size
// (larger auto objects will be allocated externally)
extern size_t maxAutoBytes;
inline void setMaxAutoBytesWithDefault(size_t nb) {
	maxAutoBytes = ( nb
		? nb
		: 1024 /* default */
	);
}

// default for max expected manual size
// (larger manual objects will not use freelists)
static const size_t absoluteMaxManualBytes = 4096;
extern size_t maxManualBytes;
inline void setMaxManualBytesWithDefault(size_t nb) {
	maxManualBytes = ( nb
		? (nb <= absoluteMaxManualBytes ? nb : absoluteMaxManualBytes)
		: 256 /* default */
	);
}


/*
** GC Root Enumeration ...
*/

//
// "Buffered" RootFinder has a fast inlined get method for use in the
//   GC inner loop ...
// Note the get() function returns a null Indir pointer only at the end!
// (Conservative: it may return a non-null but invalid Indir pointer any time.)
//
class BufferedRootFinder {
public:
	BufferedRootFinder * brf_next;
	static unsigned const brf_NUMH = 32;
	RootFinder * brf_rf;
	GHandle brf_h[brf_NUMH];
	int brf_numh;
	//
	void fill();
	//
	RootFinder * getRootFinder() { return brf_rf; }
	//
	Indir * get();
	//
	void reset();
	//
	BufferedRootFinder(RootFinder * arf, BufferedRootFinder * nxt = 0)
			: brf_next(nxt), brf_rf(arf), brf_numh(-1) {}
	BufferedRootFinder()
			: brf_next(0), brf_rf(0), brf_numh(-1) {}
	~BufferedRootFinder() { /* DO NOT delete brf_rf; */ }
};

//
// How to enumerate the installed BufferedRootFinders.
// Returns 0 at end of enumeration.
// Passing prev = 0 restarts the enumeration.
//
BufferedRootFinder * getNextBufferedRootFinder(BufferedRootFinder * prev);

/*
** Object Space.
** Contains object bodies
*/
class ObjectSpace {
private:
	char * objs_start;
	char * objs_limit;
	char * objs_free;
		// free pointer for allocation
public:
	//
	// Queries about internal Objects in the space ...
	//
	size_t getSize() { return objs_limit - objs_start; }
	size_t getUsed() { return objs_free - objs_start; }
	size_t getFree() { return objs_limit -  objs_free; }
	bool contains(void * p) { return
			(((char *)(p)) >= objs_start) && (((char *)(p)) < objs_limit); }
	//
	// Allocate an Object
	//   The Object will be allocated internally if ext == 0
	//     *and* the object fits in this space, else it will
	//     be external (using malloc())
	//
	Object * alloc(size_t nb);
	//
	void reset() { objs_free = objs_start; }
	//
	ObjectSpace(size_t nb);
	//
	~ObjectSpace();
};


extern ObjectSpace * fromSpace;
extern ObjectSpace * toSpace;


/*
** Indirect maintenance.
*/

//
// Internal version of an Indir ...
//
class Indir {
public:
	char * ind_objp;
		// -> object body in heap or external space
		// must be doubleword aligned
	Indir * ind_next;
	Indir * ind_prev;
		// links in doubly-linked list
	unsigned ind_nb;
		// size of object in bytes
		// must be multiple of doubleword
	unsigned ind_nh : 24;
		// number of handles
	unsigned ind_nwh : 4;
		// number of weak handles
	unsigned ind_gen : 4;
	static unsigned const ind_gen_PROMOTED = 3;
		// generation number
	unsigned long ind_mark;
		// time when last marked reachable
	// unsigned short ind_info;
		// UDT info
		// TODO: (ajd) NYI -- to implement this, must steal
		// half word from ind_mark and reset the marks occasionally

	IndirPublic * getPublic() { return (IndirPublic *)(this); }
	static Indir * fromPublic(IndirPublic * ip) { return (Indir *)(ip); }
private:
	Indir() {}
	~Indir() {}
public:
	//
	void checkInvariants();
	//
};

//
// Doubly-linked list of Indirs
//
class IndirList {
public:
	Indir * indl_hd;
	Indir * indl_tl;
public:
	void addHd(Indir * indp);
	void addTl(Indir * indp);
	Indir * unlink(Indir * indp);
	void spliceBefore(Indir * finger, Indir * first, Indir * last);
	void spliceAfter(Indir * finger, Indir * first, Indir * last);
	IndirList() : indl_hd(0), indl_tl(0) {}
	~IndirList() {}
	//
	unsigned checkInvariants();	// returns list size
	//
};


//
// Space containing the Indirs
//
class IndirSpace {

	//
	// Indir (handle) validation tree ...
	//
#ifdef _LP64
    static const unsigned inds_FANOUT_SHIFT =	10;
    static const unsigned inds_FANOUT_MASK =	((1<<inds_FANOUT_SHIFT)-1);
    static const unsigned inds_FANOUT =			(1<<inds_FANOUT_SHIFT);

    static const unsigned long long inds_BLK_BYTES = 		(1 << (64-2*inds_FANOUT_SHIFT));
    static const unsigned long long inds_BLK_ALIGN =		inds_BLK_BYTES;
    static const unsigned long long inds_BLK_ALIGN_MASK =	(inds_BLK_ALIGN - 1);

    static const unsigned long long inds_BLK_GROW_DFLT =	8;
#else
    static const unsigned inds_FANOUT_SHIFT =	10;
    static const unsigned inds_FANOUT_MASK =	((1<<inds_FANOUT_SHIFT)-1);
    static const unsigned inds_FANOUT =			(1<<inds_FANOUT_SHIFT);

    static const unsigned inds_BLK_BYTES = 		(1 << (32-2*inds_FANOUT_SHIFT));
    static const unsigned inds_BLK_ALIGN =		inds_BLK_BYTES;
    static const unsigned inds_BLK_ALIGN_MASK =	(inds_BLK_ALIGN - 1);

    static const unsigned inds_BLK_GROW_DFLT =	8;
#endif

	char * (inds_root[inds_FANOUT]);
		// root of Indir validation tree
	static char inds_leaf0[inds_FANOUT];
		// "all invalid" leaf

	//
	// List of Indirs ...
	//
	IndirList inds_all;
		// list of all Indirs
		// allocated Indirs precede free ones
	unsigned inds_count;
		// total number of Indirs on inds_all list
// TODO: maintain this and check it in the invariants checking code!
	Indir * inds_free;
		// -> first free Indir in inds_all list
		// assert( there is always at least one free Indir )
	unsigned inds_mark;
		// current mark value

public:
	//
	// validate an Indir pointer (conservative collector)
	//
	Indir * validate(Indir * p);

private:
	//
	// add some Indirs 
	//
	void grow(unsigned blks = inds_BLK_GROW_DFLT);

public:
	//
	// Allocate an Indir and object ...
	//
	Indir * alloc( size_t nb, unsigned nh, unsigned nwh, ObjectSpace ** where );
	//
	void collect(ObjectSpace * spTo);
	//
	// IndirSpace();
	IndirSpace(unsigned numObj);
	//
	~IndirSpace() ;
	//
	void checkInvariants(ObjectSpace * spFrom, ObjectSpace * spTo);
	void checkBodyInvariants(void *body, unsigned nh, unsigned nwh);
	//
};


extern IndirSpace * indirSpace;

} /* namespace CY_Heap */

#endif	/* _CY_HeapPvt_H_ */
