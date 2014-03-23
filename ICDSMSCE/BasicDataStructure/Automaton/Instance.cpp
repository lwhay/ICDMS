/**
 * @file Instance.cpp
 * @date November 30, 2005
 * @brief Implementation file of Instance class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "BasicDataStructure/Automaton/Instance.h"
using namespace Cayuga::BasicDataStructure::Automaton;

/*bool Instance::iterator_del =false;*/

Instance::Instance(size_t bodyBytes, EventID instanceID) : 
Event(bodyBytes, instanceID), _bTouchedInEpoch(false) ,iterator_del(false){
	_body = (char*)this + sizeof(Instance);
	num = 0;
}

Instance::~Instance() {
/*	cout<<"inst des called"<<endl;*/
}

Instance* Instance::allocInstance(size_t szSchema) {
	EventID id;
	Instance* pNewInstance = 
		(Instance*)CY_Heap::allocMGeneric(
		sizeof(Instance) + szSchema, &id);
	return new(pNewInstance) Instance(szSchema, id); 
}

void Instance::freeInstance(Instance* pThisInstance) {
	// Explicit destructor call
	// ONLY use with placement new
	pThisInstance->Instance::~Instance();
	size_t szInstance = sizeof(Instance) + pThisInstance->getBodySize();
	CY_Heap::freeMGeneric(
		pThisInstance, 
		pThisInstance, 
		szInstance
		);
}

bool Instance::isTouched() const {
	return _bTouchedInEpoch;
}

void Instance::setTouched() {
	_bTouchedInEpoch = true;
}

//char* Instance::getBody() {
//	return &(_body[0]);
//}
//
//const char* Instance::getBody() const {
//	return &(_body[0]);
//}

void* Instance::operator new(size_t, void* loc) {
	return loc;
}

void Instance :: SetCorrelatedEvent( /*const*/  EventID ptr){
	/*assert(ptr);*/
	ptr_event.insert(ptr);
	num++;
}


// void Instance ::DeleteCorrelation(/*const*/ Event*ptr){
// 	assert(ptr);
// 	assert(ptr_event.erase(ptr));
// }

