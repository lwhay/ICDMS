#ifndef	_CY_HeapFace_H_
#define	_CY_HeapFace_H_

/*
** Cayuga.
**
** Client interface to allocator.
** Two kinds of allocation:
**   Automatic (Garbage-Collected)
**   Manual (explicit free)
*/

#include <cstddef>
#include <cstring>

//added by Mingsheng 07.03.11
#include <cassert>

namespace CY_Heap {

class Object;
class GIndir;
class GHandle;

//
// Object.
// This is a base class for client-defined object bodies that can be
//   allocated for "Automatic" reclaiming (GC).
// Client object classes mmust be derived from this (empty) base class
//   and must override {numBytes, numHandles, numWeak, info} with
//   public constants.
//
class Object {

public:
	//
	// Supported (maximum) object sizes ...
	//
	static unsigned const numBytesBITS = 30;
	static size_t const numBytesMAX = ((1<<numBytesBITS)-1);
		// maximum object size
	static unsigned const numHandlesBITS = 24;
	static unsigned const numHandlesMAX = ((1<<numHandlesBITS)-1);
		// maximum number of Handles in an Object
	static unsigned const numWeakBITS = 4;
	static unsigned const numWeadMAX = ((1<<numWeakBITS)-1);
		// maximum number of weak Handles in an Object
	//
	// Sizes of a particular object.
	// These should be overridden for each UDT ...
	//
	static size_t const numBytes = 0;
		// number of bytes in Object representation
		// (must be overridden)
		// (for a variable size object, this is the minimum)
	static unsigned const numHandles = 0;
		// number of Handles in (beginning of) Object representation
		// (default is none)
	static unsigned const numWeak = 0;
		// number of "weak" Handles following the Handles in the Object
		//   representation.  A weak Handle doesn't prevent its referent
		//   from being reclaimed, but if the referent is reclaimed the
		//   weak Handle is set to null by the GC.
};

/*
**
** Automatic Allocation
**
*/

//
// Indirect pointer.
// A handle points to one of these,
//   and the Indir points to the Object body in the heap.
// Live objects in the heap are in 1-to-1 correspondence with live Indirs.
// Clients never create Indirs -- they are managed by allocA.
// TODO: allocA???
//
class GIndir {
private:
	// object pointer ...
	Object * ind_objp;
	//
	GIndir() : ind_objp(0) {}
	~GIndir() {}
	GIndir operator= (const GIndir &);
		// private to prevent use by clients
public:
	Object * getObject() { return ind_objp; }
	static GIndir * alloc( size_t nb, unsigned nh, unsigned nwh, bool ext );
		// This is the generic allocator for automatic objects.
		// Create an object of size nb, with nh handles and nwh weak refs,
		//   returning a pointer to an Indir
		// If ext is true, the object body is allocated outside the heap
		//   using malloc().  In this case the cost of allocating and reclaiming
		//   the object is increased, but it is not copied by the GC.  Thus,
		//   ext should be specified for objects that are expected to live for
		//   a long time.
		//   
};

typedef GIndir IndirPublic;

template <typename T>
class TIndir : public GIndir {
private:
	// TIndir : GIndir() {}
	// ~TIndir() {}
	TIndir operator= (const TIndir &);
		// private to prevent use by clients
public:
	T * getObject() { return (T *)ind_objp; }
	static TIndir * alloc() {
		return (TIndir *)
			GIndir::alloc(T::numBytes, T::numHandles, T::numWeak, 0);
	}
	static TIndir * alloc(size_t extraBytes, bool ext = 0) {
		return (TIndir *)
			GIndir::alloc(T::numBytes+extraBytes, T::numHandles, T::numWeak, ext);
	}
};


//
// Generic versions of an object handle.
// Clients can use these safely,
//   but they ought to use the typed versions declared below.
//
class GHandle {
protected:
	GIndir * h_ind;
public:
	bool isNull() const { return h_ind == 0; }
	GIndir * getIndir() const { return h_ind; }
	Object * getObject() { return h_ind->getObject(); }
	unsigned getSize();
	GHandle() : h_ind(0) {}
	GHandle(const GHandle& h) : h_ind(h.getIndir()) {}
	GHandle(GIndir * ind) : h_ind(ind) {}
	~GHandle() {}
};


//
// Typed Handle template.
// Instantiate with a class derived from Object.
// TODO: (ajd) fancy template tricks to enfore that requirement! ???
//
// To allocate an object: use a typed handle constructor.
//
template <typename T>
class THandle : public GHandle {
public:
	//
	typedef T ObjectType;
	//
	T * operator->() {
		//added by Mingsheng 07.03.11
		assert(h_ind);

		return ((T *)(h_ind->getObject()));
	}
	T& operator* () {
		return ((T &)(*(h_ind->getObject())));
	}
	//
	bool operator== (THandle th) const { return h_ind == th.h_ind; }
	//
	// THandle operator= (THandle th) { th_ind = th.h_ind; return (*this); }
	//
	THandle() : GHandle() {}
	THandle(const THandle & th) : GHandle(th.h_ind) {}
	THandle(TIndir<T> * ti) : GHandle(ti) {}
	THandle(size_t extraBytes, bool ext = 0)
		: GHandle( TIndir<T>::alloc(extraBytes, ext) ) {}
	~THandle() {}
};

template <typename T>
THandle<T> Talloc() { return THandle<T>(0, 0); }

template <typename T>
THandle<T> Talloc(size_t extraBytes, bool ext) { return THandle<T>(extraBytes, ext); }

	
//
// Dynamically sized array of Handles, initially all null.
// There are generic and typed versions -- clients are encouraged
//   to use the typed version ...
//
// Example:
//
//   THandleArrayHandle<Foo>(17) myFooArray(17); defines a handle to an array
//     of 17 typed-handles-to-Foo
//   So myFooArray[0] thru myFooArray[16] have type THandle<Foo>
//
//

class HandleArrayHandle: public GHandle {
public:
	unsigned getSize() const;
	GHandle & operator[] (unsigned i) const;
	HandleArrayHandle() : GHandle() {} // TODO: (ajd) correct? was GHandle(0)
	HandleArrayHandle(const HandleArrayHandle & h) : GHandle(h.h_ind) {}
	HandleArrayHandle(unsigned nh, bool ext = 0);
	~HandleArrayHandle() {}
};


template<typename T>
class THandleArrayHandle: public HandleArrayHandle {
public:
	THandle<T> & operator[] (unsigned i) const {
		return (THandle<T> &)( HandleArrayHandle::operator[](i) );
	}
	THandleArrayHandle() : HandleArrayHandle() {} // TODO: (ajd) correct? was HandleArrayHandle(0)
	THandleArrayHandle(const THandleArrayHandle & th)
		: HandleArrayHandle( th ) {}
	THandleArrayHandle(unsigned nh, bool ext = 0)
		: HandleArrayHandle( nh, ext ) {}
	~THandleArrayHandle() {}
};


/*
**
** Manual Allocation
**
*/

//
// generic allocator ...
//
//second parameter pObjectID added by Mingsheng 07.04.28
//If it is not NULL, will set it to a new object ID
void * allocMGeneric( size_t nb, long long* pObjectID = NULL );
	// allocate object of size nb
	
void freeMGeneric( void * hd, void * tl, size_t nb );
	// free the list between hd and tl,
	// assuming hd and tl delimit a homogeneous list linked by 0th word
	//   (this can always be achieved by setting tl == hd, which
	//   frees a single object).

//
// typed allocator templates ...
//
template <typename T>
T * allocM() {
	return (T *)allocMGeneric( sizeof(T) );
}

template <typename T>
T * allocMZ() {
	void * p = allocMGeneric( sizeof(T) );
	if( unsigned n  = (T::numHandles+T::numWeak) ) {
		memset( p, 0, n*sizeof(GHandle) );
	}
	return (T *)(p);
}

template <typename T>
void freeM(T * obj) {
	freeMGeneric( ((void *)(obj)), ((void *)(obj)), sizeof(T) );
}

template <typename T>
void freeM(T * objhd, T * objtl) {
	freeMGeneric( ((void *)(objhd)), ((void *)(objtl)), sizeof(T) );
}

//added by Mingsheng Hong 07.04.23
extern size_t numLiveManualBytes;	
extern size_t numLiveManualObjects;	
size_t getNumLiveManualBytes();
size_t getNumLiveManualObjects();

/*
** Create the heap.
*/
void createHeap(
	size_t nbHeapSpace = 0,		// size of fromSpace/toSpace of GC
	size_t nbA = 0,				// estimated max size of a heap object
	size_t nbM = 0,				// estimated max size of a manual object
	unsigned nObj = 0			// estimated max number of objects in heap
);
	// create heap with specified sizes
	// in all cases a value of 0 means system should use best guess
} /* CY_Heap */

#endif /* _CY_HeapFace_H_ */
