/*
** Cayuga.
**
** Heap and copying GC implementation.
*/

#include "MemoryMgr/CY_HeapFace.h"
#include "MemoryMgr/CY_GCFace.h"
#include "MemoryMgr/CY_HeapPvt.h"
#include "MemoryMgr/CY_ErrLog.h"

#include "corelib/os/threads/Mutex.h"

#include <cstddef>
#include <cstdlib>
#include <cstring>

using namespace cudb::corelib::os::threads;

namespace CY_Heap {

Mutex autoMutex;

#if (HISTORY_SIZE > 0)
HandleArrayHandle historyHandles;
unsigned historyIndex;
#endif

/*
** Ensure Header and ExtHeader are doubleword size ...
*/
int HandleSizeTest[sizeof(GHandle) == sizeof(void *)];


/*
** RootFinder implementations ...
*/

//
// "Buffered" RootFinder  ...
//
class BufferedRootFinder;

void BufferedRootFinder::fill() {
	CY_ASSERT(brf_numh == 0);
	if( (brf_numh = brf_rf->fill(brf_NUMH, brf_h)) <= 0 ) brf_numh = (-1);
}

inline Indir * BufferedRootFinder::get() {
	for(;;) {
		if( brf_numh > 0 ) {
			Indir * ip = Indir::fromPublic(brf_h[--brf_numh].getIndir());
			if( ip ) return ip;
		} else if( brf_numh < 0 ) {
			return 0;
		} else {
			fill();
		}
	}
}
//
void BufferedRootFinder::reset() {
	if( brf_rf ) {
		brf_numh = 0;
		brf_rf->reset();
	}
}

//
// A RootFinder that enumerates a specified set of Objects ...
//
class RootObjReader : public RootFinder {
	HandleArrayHandle ror_buf;
	unsigned ror_bufsz;
	int ror_cnt;
	int ror_pos;
public:
	virtual int fill(unsigned bufsz, GHandle * buf);
		// Caller holds lock.
	virtual void reset();
		// Caller holds lock.
	//
	void add(GHandle h);
		// Add a Handle to the set of objects.
		// Acquires lock.
	void remove(GHandle h);
		// Remove a Handle.
		// Acquires lock.
	//
	RootObjReader();
	virtual ~RootObjReader();
};


int RootObjReader::fill(unsigned bufsz, GHandle * buf) {
	if( ror_pos ) return 0;
	*buf = ror_buf; // GC will trace the buffer object
	ror_pos = 1;
	return 1;
}

void RootObjReader::reset() {
	ror_pos = 0;
}


// TODO: (ajd) make this a list of fixed-size HandleArrays
void RootObjReader::add(GHandle h) {
	unsigned nbufsz = 0;
	HandleArrayHandle nbuf;
	for(;;){
		{ /* begin critical section */
			Lock theLock = GC_MUTEX.scopedLock();
			if( nbufsz > ror_bufsz ) {
				if( nbuf.isNull() ) DIE();
				if( ror_bufsz ) {
					memcpy( &(nbuf[0]), &(ror_buf[0]), ror_bufsz*sizeof(GHandle) );
				}
				ror_buf = nbuf;  ror_bufsz = nbufsz;
			}
			if( ror_cnt < ror_bufsz ) {
				ror_buf[ror_cnt++] = h;
				nbufsz = 0;
			} else {
				nbufsz = (unsigned)(8 + ror_bufsz * 1.5);
			}
		} /* end critical section */
		if( nbufsz == 0 ) break;
		nbuf = HandleArrayHandle(/*nh=*/nbufsz, /*ext=*/1);
	}
}

void RootObjReader::remove(GHandle h) {
	{ /* begin critical section */
		Lock theLock = GC_MUTEX.scopedLock();
		for( unsigned i = 0; i < ror_cnt; i++ ) {
			if( ror_buf[i].getIndir() == h.getIndir() ) {
				ror_cnt -= 1;
				ror_buf[i] = ror_buf[ror_cnt];
			}
		}
	} /* end critical section */
}

RootObjReader::RootObjReader() : RootFinder() {
	ror_bufsz = ror_cnt = ror_pos = 0;
}

RootObjReader::~RootObjReader() {
	// nothing to do
}

	
//
// A RootFinder that enumerates a specified set of client Handle
// variables ...
//
class RootVarReader : public RootFinder {
	GHandle ** rvr_vbuf;
	unsigned rvr_vbufsz;
	int rvr_cnt;
	int rvr_pos;
public:
	virtual int fill(unsigned bufsz, GHandle * buf);
	virtual void reset();
	//
	void add(GHandle * hp);
		// Add the address of a Handle-valued variable
		// to the set of root variables to enumerate.
	void remove(GHandle * hp);
		// Remove an address.
	//
	RootVarReader();
	virtual ~RootVarReader();
};


int RootVarReader::fill(unsigned bufsz, GHandle * buf) {
	int i = 0;
	while( (i < bufsz) && (rvr_pos < rvr_cnt) ) {
		buf[i++] = *(rvr_vbuf[rvr_pos++]);
	}
	return i;
}

void RootVarReader::reset() {
	rvr_pos = 0;
}

void RootVarReader::add(GHandle * hp) {
	if( rvr_cnt >= rvr_vbufsz ) {
		rvr_vbufsz = (unsigned)(8 + rvr_vbufsz * 1.5);
		rvr_vbuf = (GHandle **)realloc(rvr_vbuf, rvr_vbufsz*sizeof(GHandle *));
		if( rvr_vbuf  == 0 ) DIE();
	}
	rvr_vbuf[rvr_cnt++] = hp;
}

void RootVarReader::remove(GHandle * hp) {
	for( unsigned i = 0; i < rvr_cnt; i++ ) {
		if( rvr_vbuf[i] == hp ) {
			rvr_cnt -= 1;
			rvr_vbuf[i] = rvr_vbuf[rvr_cnt];
		}
	}
}

RootVarReader::RootVarReader() : RootFinder() {
	rvr_vbufsz = rvr_cnt = rvr_pos = 0;
}

RootVarReader::~RootVarReader() {
	free( rvr_vbuf );
}


//
// The registered RootFinders ...
//
RootVarReader theRootVarReader;
BufferedRootFinder theRootVarFinder(&theRootVarReader);

RootObjReader theRootObjReader;
BufferedRootFinder theRootObjFinder(&theRootObjReader, &theRootVarFinder);

BufferedRootFinder * registeredRootFindersHd = &theRootObjFinder;
BufferedRootFinder * registeredRootFindersTl = &theRootVarFinder;
BufferedRootFinder * currentRootFinder = &theRootObjFinder;

void registerRootFinder(RootFinder * rf, bool conservative) {
	BufferedRootFinder * p = new BufferedRootFinder(rf);
	registeredRootFindersTl->brf_next = p;
	registeredRootFindersTl = p;
}

void unregisterRootFinder(RootFinder * rf) {
	BufferedRootFinder *prev = 0;
	BufferedRootFinder *p = registeredRootFindersHd;
	while( p ) {
		if( p->brf_rf == rf ) {
			CY_ASSERT( prev != 0 );
			if( p->brf_next == 0 ) registeredRootFindersTl = prev;
			prev->brf_next = p->brf_next;
			delete p;
			break;
		}
		prev = p; p = p->brf_next;
	}
}

void registerRootVar(GHandle * hp) {
	theRootVarReader.add( hp );
}

void unregisterRootVar(GHandle * hp) {
	theRootVarReader.remove( hp );
}

void registerRootObject(GHandle h) {
	theRootObjReader.add( h );
}

void unregisterRootObject(GHandle h) {
	theRootObjReader.remove( h );
}


BufferedRootFinder * getNextBufferedRootFinder(BufferedRootFinder * prev) {
	BufferedRootFinder * brf =
			(( prev == 0 ) ? registeredRootFindersHd : prev->brf_next);
	if( brf ) brf->reset();
	return brf;
}

/*
** ObjectSpace ...
*/

inline Object * ObjectSpace::alloc( size_t nb ) {
	size_t nba = (((unsigned)(nb)) + sizeof(double) - 1) & ~(sizeof(double) - 1);
	if( nba > getFree() ) return 0;
	// allocate an internal heap object by pushing objs_free pointer ...
	char * objp = objs_free;
	objs_free += nba;
	return (Object *)(objp);
}


ObjectSpace::ObjectSpace(size_t nb) {
	if( !(objs_start = (char *)malloc(nb)) ) {
		DIE();
	}
	objs_limit = objs_start + nb;
	reset();
}

ObjectSpace::~ObjectSpace() {
	free(objs_start);
}

/*
** Handle operation ...
*/
unsigned GHandle::getSize() {
	return Indir::fromPublic(getIndir())->ind_nb;
}

/*
** Indir (private representation) ...
*/
void Indir::checkInvariants() {
	if( ind_nb < ((ind_nh + ind_nwh)*sizeof(GHandle)) ) DIE();
#ifdef _LP64
    if( ((unsigned long long)(ind_objp)) & (sizeof(double)-1) ) DIE();
#else
    if( ((unsigned)(ind_objp)) & (sizeof(double)-1) ) DIE();
#endif
}


/*
** Indir lists ...
*/

inline void IndirList::addHd(Indir * indp) {
	indp->ind_next = indl_hd;  indp->ind_prev = 0;
	if( indl_hd ) {
		indl_hd->ind_prev = indp;
	} else {
		indl_tl = indp;
	}
	indl_hd = indp;
}

inline void IndirList::addTl(Indir * indp) {
	indp->ind_prev = indl_tl;  indp->ind_next = 0;
	if( indl_tl ) {
		indl_tl->ind_next = indp;
	} else {
		indl_hd = indp;
	}
	indl_tl = indp;
}

inline Indir * IndirList::unlink(Indir * indp) {
	if( indp ) {
		Indir * prev = indp->ind_prev;
		Indir * next = indp->ind_next;
		if( prev ) { prev->ind_next = next; } else { indl_hd = next; }
		if( next ) { next->ind_prev = prev; } else { indl_tl = prev; }
		indp->ind_prev = 0;  indp->ind_next = 0; // TODO: (ajd) necessary?
	}
	return indp;
}

inline void IndirList::spliceBefore(Indir * finger, Indir * first, Indir * last) {
	last->ind_next = finger;
	if( finger ) {
		Indir * p = finger->ind_prev;
		finger->ind_prev = last;
		first->ind_prev = p;
		if( p ) { p->ind_next = first; } else { indl_hd = first; }
	} else /* (finger == 0) ==> append */ {
		if( indl_tl ) { indl_tl->ind_next = first; } else { indl_hd = first; }
		first->ind_prev = indl_tl;
		indl_tl = last;
	}
}

inline void IndirList::spliceAfter(Indir * finger, Indir * first, Indir * last) {
	first->ind_prev = finger;
	if( finger ) {
		Indir * p = finger->ind_next;
		finger->ind_next = first;
		last->ind_next = p;
		if( p ) { p->ind_prev = last; } else { indl_tl = last; }
	} else /* (finger == 0) ==> prepend */ {
		if( indl_hd ) { indl_hd->ind_prev = last; } else { indl_hd = first; }
		last->ind_next = indl_hd;
		indl_hd = first;
	}
}

unsigned IndirList::checkInvariants() {
	if( (indl_hd == 0) && (indl_tl == 0) ) return 0;
	if( indl_hd && !indl_tl ) DIE();
	if( indl_tl && !indl_hd ) DIE();
	if( indl_hd->ind_prev ) DIE();
	if( indl_tl->ind_next ) DIE();
	//
	unsigned n = 1;
	for( Indir * p = indl_hd; p != indl_tl; p = p->ind_next) {
		if( !p ) DIE();
		if( p->ind_next && p->ind_next->ind_prev != p ) DIE();
		n++;
	}
	return n;
}


/*
** Indir Space ...
*/

char IndirSpace::inds_leaf0[inds_FANOUT] = { 0 };
	// "all invalid" leaf

//
// validate a pointer (for conservative GC)
//
inline Indir * IndirSpace::validate(Indir * p) {
#ifdef _LP64
	unsigned const l0Index =
        ((unsigned long long)(p)) >> (64 - inds_FANOUT_SHIFT);
	unsigned const l1Index =
        (((unsigned long long)(p)) >> (64 - 2*inds_FANOUT_SHIFT)) & inds_FANOUT_MASK;
	if( (inds_root[l0Index])[l1Index] ) {
		unsigned const boff =
            ( ((unsigned long long)(p)) & ((1 << (64 - 2*inds_FANOUT_SHIFT)) - 1) );
		if( boff % sizeof(Indir) == 0 ) {
			if( p->ind_mark >= inds_mark ) return p;
		}
	}
#else
    unsigned const l0Index =
        ((unsigned)(p)) >> (32 - inds_FANOUT_SHIFT);
    unsigned const l1Index =
        (((unsigned)(p)) >> (32 - 2*inds_FANOUT_SHIFT)) & inds_FANOUT_MASK;
    if( (inds_root[l0Index])[l1Index] ) {
        unsigned const boff =
            ( ((unsigned)(p)) & ((1 << (32 - 2*inds_FANOUT_SHIFT)) - 1) );
        if( boff % sizeof(Indir) == 0 ) {
            if( p->ind_mark >= inds_mark ) return p;
        }
    }
#endif
	return 0;
}


//
// add some Indirs 
//
void IndirSpace::grow(unsigned blks) {
	//
	// TODO: (ajd) blks == 0 should imply grow by a percentage
	// (we can use inds_count to figure this out)
	// but for now we just grow by a little ...
	//
	if( blks == 0 ) blks = 2;
	//
	// allocate a big enough aligned array of Indirs ...
	//
	size_t nb = (blks * inds_BLK_BYTES) + inds_BLK_BYTES - 1;
	Indir * ip = (Indir *)calloc( 1, nb );
	if( !ip ) DIE();
#ifdef _LP64
	ip = (Indir *)(
        (((unsigned long long)(ip)) + inds_BLK_BYTES - 1) & ~inds_BLK_ALIGN_MASK );
#else
    ip = (Indir *)(
        (((unsigned)(ip)) + inds_BLK_BYTES - 1) & ~inds_BLK_ALIGN_MASK );
#endif
	//
	// process each block of Indirs ...
	//
	for( unsigned i = 0; i < blks; i++ ) {
		// Insert block i into tree ...
		// find the corresponding leaf
		Indir * ipi = (Indir *)( ((char *)(ip)) + i*inds_BLK_BYTES );
#ifdef _LP64
		unsigned const l0index =
            ((unsigned long long)(ipi)) >> (64 - inds_FANOUT_SHIFT);
#else
        unsigned const l0index =
            ((unsigned)(ipi)) >> (32 - inds_FANOUT_SHIFT);
#endif
		char * leaf = inds_root[l0index];
		if( leaf == inds_leaf0 ) {
			// allocate new leaf ...
			leaf = (char *)calloc( 1, inds_FANOUT );
			if( !leaf ) DIE();
			inds_root[l0index] = leaf;
		}
		// set valid byte in leaf ...
#ifdef _LP64
        unsigned const l1index =
            (((unsigned long long)(ipi)) >> (64 - 2*inds_FANOUT_SHIFT)) & inds_FANOUT_MASK;
#else
		unsigned const l1index =
			(((unsigned)(ipi)) >> (32 - 2*inds_FANOUT_SHIFT)) & inds_FANOUT_MASK;
#endif
		CY_ASSERT( leaf[l1index] == 0 );
		leaf[l1index] = 1;
		// link Indirs in this block onto Indir list ...
		unsigned nnew = inds_BLK_BYTES/sizeof(Indir);
		for( unsigned j = 0; j < nnew; j++ ) {
			inds_all.addTl( ipi + j );
		}
		inds_count += nnew;
	}
}

//
// Allocate an Indir and object ...
//
inline Indir * IndirSpace::alloc( size_t nb, unsigned nh, unsigned nwh,
		ObjectSpace ** where ) {
	Object * objp;
	Indir * indp;
	size_t zlen = (nh+nwh) * sizeof(GHandle);
	CY_ASSERT_DEBUG( zlen <= nb );
	{ /* begin critical section */
		Lock theLock = ALLOCA_MUTEX.scopedLock();
		// allocate object body, internally if so requested and possible ...
		objp = ( where ? (*where)->alloc(nb) : 0 );
		if( !objp ) {
			if( !(objp = (Object *)malloc(nb)) ) DIE();
			where = 0;
		}
		// clear the handles and weak handles
		(void) memset((void *)objp, 0, zlen);
		// get a free Indiir
		// (grow the Indir space if necessary)
		indp = inds_free;
		if( (inds_free = indp->ind_next) == 0 ) {
			grow(0);  inds_free = indp->ind_next;
		}
		// initialize the Indir properly ...
		indp->ind_mark = inds_mark;
		indp->ind_nb = nb;
		indp->ind_nh = nh;
		indp->ind_nwh = nwh;
		indp->ind_objp = (char *)objp;
		if( where ) {
			indp->ind_gen = 0;
		} else {
			// external object must move to front ...
			indp->ind_gen = Indir::ind_gen_PROMOTED;
			inds_all.addHd(inds_all.unlink(indp));
#			if (HISTORY_LENGTH > 0)
				HISTORY_ADD( GHandle(indp->getPublic()) );
#			endif /* HISTORY_LENGTH */
		}
	} /* end critical section */
	return indp;
}

//
// The heart of the GC ...
//
void IndirSpace::collect(ObjectSpace * spTo) {

	//
	// Process the roots conservatively ...
	//
	IndirList worklist;
	BufferedRootFinder * brf = getNextBufferedRootFinder(0);
	while( brf ) {
		if( Indir * indp = brf->get() ) {
			if( indp = validate(indp) ) {
				if( indp->ind_mark <= inds_mark ) {
					CY_ASSERT_DEBUG( indp->ind_mark == inds_mark );
						/* because it passed validate! */
					indp->ind_mark = inds_mark+1;
					worklist.addTl( inds_all.unlink(indp) );
				}
			}
		} else {
			brf = getNextBufferedRootFinder(brf);
		}
	}
	//
	// Trace ...
	//
	for( Indir * indp = worklist.indl_hd; indp; indp = indp->ind_next ) {
		GHandle * ph = (GHandle *)(indp->ind_objp);
		for( unsigned j = indp->ind_nh; j > 0; j-- ) {
			if( !(ph->isNull()) ) {
				Indir * pj = Indir::fromPublic(ph->getIndir());
				CY_ASSERT_DEBUG( pj != 0 );
				if( pj->ind_mark <= inds_mark ) {
					pj->ind_mark = inds_mark+1;
					worklist.addTl( inds_all.unlink(pj) );
				}
			}
			ph++;
		}
	}
	//
	// Null out weak refs to dead objects ...
	// NOTE: this cannot be fused with previous loop!
	//
	for( Indir * indp = worklist.indl_hd; indp; indp = indp->ind_next ) {
		unsigned j;
		if( (j = indp->ind_nwh) == 0 ) continue;
		GHandle * ph = ((GHandle *)(indp->ind_objp)) + indp->ind_nh;
		for( ; j > 0; j-- ) {
			Indir * pj = Indir::fromPublic(ph->getIndir());
			if( (pj != 0) && (pj->ind_mark <= inds_mark) ) {
				*ph = GHandle();
			}
			ph++;
		}
	}
	//
	// Free any dead external objects ...
	//
	for(
		Indir * indp = inds_all.indl_hd
		; (indp != 0) && (indp->ind_gen >= Indir::ind_gen_PROMOTED)
		; indp = indp->ind_next
	) {
		void * p = (void *)(indp->ind_objp);
		CY_ASSERT_DEBUG( (p != 0)
			&& !(fromSpace->contains(p)) && !(toSpace->contains(p)) );
		free(p);
		indp->ind_objp = 0;
	}
	//
	// Copy the object bodies, distributing external objects to head
	// of new allocated list and internal objects to tail ...
	//
	IndirList alloclist;
	while( Indir * indp = worklist.unlink(worklist.indl_hd) ) {
		// TODO: (ajd) if we go to delayed copy, will have to do
		// something here -- currently it would just copy from
		// toSpace to toSpace, yuck!
		unsigned newgen = (indp->ind_gen) + 1;
		if( newgen < Indir::ind_gen_PROMOTED ) {
			if( !spTo->contains(indp->ind_objp) ) {
				char * newp = (char *)(spTo->alloc( indp->ind_nb ));
				if( !newp ) DIE(); // TODO: cant happen if tospace was empty
				memcpy( newp, indp->ind_objp, indp->ind_nb );
				indp->ind_gen = newgen;
				indp->ind_objp = newp;
			}
			alloclist.addTl(indp);
		} else if( newgen == Indir::ind_gen_PROMOTED ) {
			char * newp = (char *)(malloc( indp->ind_nb ));
			if( !newp ) DIE();
			memcpy( newp, indp->ind_objp, indp->ind_nb );
			indp->ind_gen = newgen;
			indp->ind_objp = newp;
			alloclist.addHd(indp);
		} else {
			alloclist.addHd(indp);
		}
	}
	//
	// Splice the lists ...
	//
	inds_free = inds_all.indl_hd;
	inds_all.spliceBefore( inds_all.indl_hd, alloclist.indl_hd, alloclist.indl_tl );
	//
	// Increment the mark ...
	//
	inds_mark += 1;
}

//
//
IndirSpace::IndirSpace(unsigned numObj)
		: inds_all(), inds_mark(1), inds_count(0) {
	for( unsigned i = 0; i < inds_FANOUT; i++ ) {
		inds_root[i] = inds_leaf0;
	}
	grow( (numObj * sizeof(Indir) + inds_BLK_BYTES - 1) / inds_BLK_BYTES );
	inds_free = inds_all.indl_hd;
}
//
//
IndirSpace::~IndirSpace() {
	// TODO: delete the indir blocks?
}
//
//
void IndirSpace::checkInvariants(ObjectSpace * spFrom, ObjectSpace *spTo) {
	// inds_all list structurally okay ...
	{ unsigned c = inds_all.checkInvariants();
		if( c != inds_count ) DIE();
	}
	//
	Indir * p;
  doExternal: ;
	for( p = inds_all.indl_hd; p; p = p->ind_next ) {
		if( p == inds_free ) goto doFree;
		if( !validate(p) ) DIE();
		p->checkInvariants();
		if( p->ind_gen < Indir::ind_gen_PROMOTED ) /* not external */ goto doInternal;
		if( p->ind_mark != inds_mark ) DIE();
		void * body = (void *)(p->ind_objp);
		if( !body || spFrom->contains(body) || spTo->contains(body) ) DIE();
		checkBodyInvariants( body, p->ind_nh, p->ind_nwh );
	}
	DIE();
  doInternal: ;
	for( ; p; p = p->ind_next ) {
		if( p == inds_free ) goto doFree;
		if( !validate(p) ) DIE();
		p->checkInvariants();
		if( p->ind_gen >= Indir::ind_gen_PROMOTED ) /* external */ DIE();
		if( p->ind_mark != inds_mark ) DIE();
		void * body = (void *)(p->ind_objp);
		if( !spTo->contains(body) ) DIE();
		checkBodyInvariants( body, p->ind_nh, p->ind_nwh );
	}
	DIE();
  doFree: ;
	for( ; p; p = p->ind_next ) {
		p->checkInvariants();
		if( p->ind_gen >= Indir::ind_gen_PROMOTED ) /* external */ {
			if( p->ind_objp ) DIE();
		} else /* internal */ {
			void * body = p->ind_objp;
			if( body
					&& !spFrom->contains(body)
					&& !spTo->contains(body) ) {
				DIE();
			}
		}
	}
  done: ;
}
//
void IndirSpace::checkBodyInvariants(void * body, unsigned nh, unsigned nwh) {
#ifdef _LP64
    if( ((unsigned long long)(body)) & (sizeof(double)-1) ) DIE();
#else
    if( ((unsigned)(body)) & (sizeof(double)-1) ) DIE();
#endif
	Indir ** ppind = (Indir **)body;
	for( int i = nh + nwh; i > 0; i-- ) {
		if( *ppind && !validate(*ppind) ) DIE();
		ppind++;
	}
}

/*
** Global Heap data structures ...
*/

IndirSpace * indirSpace = 0;
ObjectSpace * fromSpace = 0;
ObjectSpace * toSpace = 0;

size_t heapSpaceBytes = 0;
unsigned maxNumObj = 0;
size_t maxAutoBytes = 0;

void createHeap(size_t nbHeapSpace, size_t nbA, size_t nbM, unsigned nObj) {

	size_t oldHeapSpaceBytes = heapSpaceBytes;
	setHeapSpaceBytesWithDefault(nbHeapSpace);
	if( oldHeapSpaceBytes && (heapSpaceBytes > oldHeapSpaceBytes) ) {
		DIE();
	}

	unsigned oldMaxNumObj = maxNumObj;
	setMaxNumObjWithDefault(nObj);
	if( oldMaxNumObj && (maxNumObj > oldMaxNumObj) ) {
		DIE();
	}

	size_t oldMaxAutoBytes = maxAutoBytes;
	setMaxAutoBytesWithDefault(nbA);
	if( oldMaxAutoBytes && (maxAutoBytes > oldMaxAutoBytes) ) {
		DIE();
	}
	if( (2 * maxAutoBytes) > heapSpaceBytes ) {
		DIE();
	}

	size_t oldMaxManualBytes = maxManualBytes;
	setMaxManualBytesWithDefault(nbM);
	if( oldMaxManualBytes && (maxManualBytes > oldMaxManualBytes) ) {
		DIE();
	}

	if( indirSpace == 0 ) {
		indirSpace = new IndirSpace(maxNumObj);
		fromSpace = new ObjectSpace(heapSpaceBytes);
		toSpace = new ObjectSpace(heapSpaceBytes);
		if( (indirSpace == 0) || (fromSpace == 0) || (toSpace == 0) ) {
			DIE();
		}
		fromSpace->reset();
		toSpace->reset();
	}
#	if (HISTORY_SIZE > 0)
		historyHandles = HandleArrayHandle( /*nh=*/HISTORY_SIZE, /*ext=*/1 );
		registerRootVar(&historyHandles);
#	endif /* HISTORY_SIZE */
}

/*
** The actual automatic allocator ...
*/
IndirPublic * IndirPublic::alloc(size_t nb, unsigned nh, unsigned nwh, bool ext) {
	ObjectSpace ** where = ( ext ? 0 : &toSpace );
	return (indirSpace->alloc( nb, nh, nwh, where ))->getPublic();
}

/*
** Generic HandleArrayHandle implementation ...
*/

unsigned HandleArrayHandle::getSize() const {
	Indir * indp = Indir::fromPublic(getIndir());
	return indp->ind_nh;
}

GHandle & HandleArrayHandle::operator[] (unsigned i) const {
	Indir * indp = Indir::fromPublic(getIndir());
	if( i >= indp->ind_nh ) i = 0;	// TODO: ajd: exception here?
	GHandle *p = (GHandle *)(indp->ind_objp);
	return p[i];
}

HandleArrayHandle::HandleArrayHandle(unsigned nh, bool ext) : GHandle() {
	// TODO: should be external?
	if( nh <= Object::numHandlesMAX ) {
		ObjectSpace ** where = ( ext ? 0 : &toSpace );
		h_ind = (indirSpace->alloc(nh*sizeof(GHandle), nh, 0, where))->getPublic();
	}
}

//
// Client-invoked stop-the-world GC ...
//
bool runGC(bool force) {
	int ans = 0;
	{ Lock theLock = GC_MUTEX.scopedLock();
		// TODO: make this space computation more flexible and symbolic!
 		if( !force && (toSpace->getFree() > ((toSpace->getSize())>>3)) ) {
 			// do nothing
 		} else {
			// switch spaces ...
			{ ObjectSpace *tmp = fromSpace;  fromSpace = toSpace;  toSpace = tmp; }
			// do mark and sweep collection ...
			indirSpace->collect( toSpace );
			fromSpace->reset();
			ans = 1;
		}
	}
	return ans;
}

} /* namespace CY_Heap */

