/**
 * @file FifoQueueDef.h
 * @date April 15, 2007
 * @brief Implementation file of FifoQueue class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _FIFO_QUEUE_DEF_H
#define _FIFO_QUEUE_DEF_H

#include "PriorityQueue/FiFoQueue.h"
using namespace Cayuga;

#include <cassert>
using namespace std;

template <typename ElemT>
FifoQueue<ElemT>::FifoQueue() {
}

template <typename ElemT>
const ElemT* FifoQueue<ElemT>::_peekElemT() {
	//This function is called by the public APIs peek and get above, 
	//so mutex is used in that level, not here.
	if (_elems.empty()) {
		return NULL;
	}

	const ElemT* pThisElem = _elems.front();
	return pThisElem;
}

template <typename ElemT>
const ElemT* FifoQueue<ElemT>::peek() {
	Lock lock = _mutex.scopedLock();
	return _peekElemT();
}

template <typename ElemT>
const ElemT* FifoQueue<ElemT>::get() {
	Lock lock = _mutex.scopedLock();
	const ElemT* pThisElem = _elems.front();
	assert (pThisElem);
	
	_elems.pop();	
	return pThisElem;
}

template <typename ElemT>
void FifoQueue<ElemT>::put(const ElemT* ev) {
	Lock lock = _mutex.scopedLock();
	_elems.push(ev);

}

template <typename ElemT>
bool FifoQueue<ElemT>::empty() {
	Lock lock = _mutex.scopedLock();
	return _elems.empty();
}

template <typename ElemT>
size_t FifoQueue<ElemT>::size() {
	return _elems.size();
}

#endif //_FIFO_QUEUE_DEF_H
