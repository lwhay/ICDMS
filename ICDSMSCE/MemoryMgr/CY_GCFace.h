#ifndef	_CY_GCFace_H_
#define	_CY_GCFace_H_

/*
** Cayuga.
**
** Interface to garbage collector.
*/

#include <cstddef>
#include <cstdlib>

#include "CY_HeapFace.h"
#include "CY_ErrLog.h"

namespace CY_Heap {


/*
** Base class for buffered root finding.
** Client should derive a subclass with ...
**		a fill() method,
**		a reset() method,
**   with appropriate state for use by fill().
*/
class RootFinder {
public:
	virtual int fill(unsigned bufsz, GHandle * buf) = 0;
		// Fill buf with up to bufsz Handle values, return the
		// number of values added.  Return <= 0 at end of enumeration.
	virtual void reset() = 0;
		// Reset the fill() enumeration to the beginning again.
	RootFinder() {}
	virtual ~RootFinder() {}
};


/*
** Register a RootFinder or root variable with the GC.
** There must be at least one registration, else everything will
**	be reclaimed by every garbage collection.
** There may be multiple RootFinders.
** The roots they enumerate need not be disjoint sets.
**
** Unregistering is provided for completeness, it is not
**  intended to be heavily used or efficient.
*/
void registerRootFinder(RootFinder * rf, bool conservative = true);
void unregisterRootFinder(RootFinder * rf);

void registerRootVar(GHandle *hp);
void unregisterRootVar(GHandle *hp);

void registerRootObject(GHandle hp);	// THREADS TODO:
void unregisterRootObject(GHandle hp);	// THREADS TODO:

/*
** Run an atomic GC if appropriate ...
**   force => do it even if it seems "inappropriate"
**   return true iff a GC was done
** To run a GC, the registered RootFinders are
**   reset and enumerated.
*/
bool runGC(bool force = 0);


}; /* CY_Heap */

#endif /* _CY_GCFace_H_ */
