/*
** Cayuga
**
** Manual-release memory allocator
**
** This implementation is just a simple pool allocator with doubleword-
**   aligned sizes ...
**
** Nov 05 ajd:
**	Created
** May 06 ajd:
**	Thread-safe
*/

#include "MemoryMgr/CY_ErrLog.h"
#include "MemoryMgr/CY_HeapFace.h"
#include "MemoryMgr/CY_HeapPvt.h"

#include "corelib/os/threads/Mutex.h"

#include <cstddef>
#include <cstdlib>

using namespace cudb::corelib::os::threads;


namespace CY_Heap {

Mutex manualMutex;

size_t maxManualBytes = 0;

static unsigned const poolPageSizeMIN = 8 * 1024;	// min size of freepool page

//added by Mingsheng Hong 07.04.23
//these variables cannot be accessed directly in order to prevent race 
//conditions; they have to be accessed by the following methods 
size_t numLiveManualBytes = 0;	
//number of manually allocated objects that are still live
size_t numLiveManualObjects = 0;	
//number of manually allocated objects
long long numAllocatedManualObjects = 0;	

size_t getNumLiveManualBytes() {
	/* begin critical section */
	Lock theLock = ALLOCM_MUTEX.scopedLock();
	return numLiveManualBytes;
	/* end critical section */
}

size_t getNumLiveManualObjects() {
	/* begin critical section */
	Lock theLock = ALLOCM_MUTEX.scopedLock();
	return numLiveManualObjects;
	/* end critical section */
}

struct PoolElt {
	PoolElt * pe_next;
};

PoolElt *(freePool[absoluteMaxManualBytes/sizeof(double)]) = { 0 };
Mutex freePoolMutex;

void fillFreePool(unsigned i) {
	PoolElt * p;
	unsigned i1 = i + 1;
	size_t poolPageSize = i1 * sizeof(double);
	if( poolPageSize < poolPageSizeMIN ) poolPageSize = poolPageSizeMIN;
	if( !(p = (PoolElt *)malloc(poolPageSize)) ) DIE();
	for( int j = poolPageSize / (i1 * sizeof(double)); j > 0; j-- ) {
		p->pe_next = freePool[i];  freePool[i] = p;
		p = (PoolElt *)( ((double *)(p)) + i1 );
	}
}

void * allocMGeneric( size_t nb, long long* pObjectID ) {
	{ /* begin critical section */
		Lock theLock = ALLOCM_MUTEX.scopedLock();

		//added by Mingsheng Hong 07.04.28
		if (pObjectID) {
			*pObjectID = numAllocatedManualObjects;
		}

		//added by Mingsheng Hong 07.04.23
		numLiveManualBytes += nb;
		++numLiveManualObjects;
		++numAllocatedManualObjects;
		
		if( nb == 0 ) {
			nb = sizeof(double);
		}
		if( nb <= maxManualBytes ) {
			unsigned i = ((nb  - 1) / sizeof(double));
			PoolElt * pe;
			{ Lock theLock = freePoolMutex.scopedLock();
				pe = freePool[i];
				if( !pe ) {
					fillFreePool(i);  pe  = freePool[i];
				}
				freePool[i] = pe->pe_next;
			}
			return ((void *)(pe));
		}
		{
			void * m = malloc(nb);
			if( !m ) DIE();
			return m;
		}
	} /* end critical section */
}


void freeMGeneric( void * hd, void * tl, size_t nb ) {
	{ /* begin critical section */
		Lock theLock = ALLOCM_MUTEX.scopedLock();
		if( !hd ) return;
		if( !tl ) tl = hd;

		//added by Mingsheng Hong 07.04.23
		//in theory a free call deallocate a list of objects. 
		//However here we make the simplified assumption that
		//a free call deallocates one object
		numLiveManualBytes -= nb;
		--numLiveManualObjects;

		if( nb == 0 ) nb = sizeof(double);
		if( nb <= maxManualBytes ) {
			unsigned i = (nb - 1) / sizeof(double);
			{ Lock theLock = freePoolMutex.scopedLock();
				((PoolElt *)(tl))->pe_next = freePool[i];
				freePool[i] = ((PoolElt *)(hd));
			}
		} else {
			do {
				PoolElt * pe = (PoolElt *)(hd);
				hd = (void *)(pe->pe_next);
				free( (void *)(pe) );
			} while (hd != tl);
		}
	} /* end critical section */
}

} /* namespace CY_Heap */
