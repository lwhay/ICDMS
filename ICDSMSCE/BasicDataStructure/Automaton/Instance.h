/**
 * @file Instance.h
 * @date November 30, 2005
 * @brief Stores the information of automaton instances in Cayuga engine.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _INSTANCE_H
#define _INSTANCE_H

#include "BasicDataStructure/Event/Event.h"

#include <list>
#include <hash_set>
using namespace std;

namespace Cayuga {
namespace BasicDataStructure {
namespace Automaton {
using Cayuga::BasicDataStructure::EventNS::Event;

/**
 * @brief An Instance object represents an automaton instance in Cayuga engine.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 30, 2005
 * @version 1.0
 */
class Instance : public Event {
	//friend class Cayuga::QueryProcessor::FilterEvaluator;
	//friend class FREvaluator;
	//friend class QueryProcessorImpl;

	//luoluo 4-7
	typedef hash_set</*const*/ Event * > point_event;
public:
	/**
	* @brief Default constructor
	* @param bodyBytes Number of bytes in the instance body
	* @param instanceID eid of this instance
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 30, 2005
	* @version 1.0
	*/
	Instance(size_t bodyBytes, EventID instanceID);
	
	/**
	* @brief Destructor Not virtual, because memory is released here. If 
			 destructor of base class (Event) is called as well, memory will be
			 released again.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 30, 2005
	* @version 1.0
	*/
	virtual ~Instance();

	/**
	* @brief Allocate a new instance with allocMGeneric
	* @param szSchema Size of the instance body
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 28, 2007
	* @version 1.0
	*/
	static Instance* allocInstance(size_t szSchema);

	/**
	* @brief Free the input instance with freeMGeneric.
	* @param pThisInstance The input instance to free.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 28, 2007
	* @version 1.0
	*/
	static void freeInstance(Instance* pThisInstance);

	/**
	* @brief Return the content of this instance.
	* @return As above.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 30, 2005
	* @version 1.0
	*/
	//virtual char* getBody();
	
	/**
	* @brief Return the content of this instance.
	* @return As above.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 30, 2005
	* @version 1.0
	*/
	//virtual const char* getBody() const;

	/**
	* @brief Return true iff this node is touched during current event 
	         processing epoch.
	* @return As above.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 30, 2005
	* @version 1.0
	*/
	virtual bool isTouched() const;
	
	/**
	* @brief Set this node to be touched during current event 
	         processing epoch.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 30, 2005
	* @version 1.0
	*/
	virtual void setTouched();

	///**
	//* @brief Overload the new operator to control memory management for Instance
	//	     class.

	//		 Specifically, we use our own memory pool to manage automaton 
	//		 instances.
	//* @author Mingsheng Hong (mshong@cs.cornell.edu)
	//* @date December 9, 2005
	//* @version 1.0
	//*/
	void* operator new(size_t, void* loc);
	/**
	*@brief		In order to set the relationship between instance and event.
	*@author		leixiaoluos
	*@date		2011-4-7
	*/
    void SetCorrelatedEvent( /*const*/ EventID ptr );
	/**
	*@brief		Return the iterator to its caller.
	*@author		leixiaoluo
	*@date		2011-4-7
	*/
/*	point_event GetCorrelatedEvent();*/
// 	point_event::iterator Instance ::GetCorrelatedEvent(){
// 		assert(!ptr_event.empty());
// 		hash_set</*const*/ Event *> ::iterator ite = ptr_event.begin();
// 		return ite;
// 	}
// 	/**
// 	*@brief		Delete the correlation between the event and instance.
// 	*@author		leixiaoluo
// 	*@date		2011-4-7
// 	*/
// 	void DeleteCorrelation(/*const*/ Event* ptr);
    /*point_event ReturnCorrelation();*/
// 	point_event Instance::ReturnCorrelation(){
// 		return this->ptr_event;
// 	}

		/**
	*@brief		To store the correlation between the event and the instance.
	*@authot		leixiaoluo
	*@date		2011-4-7
	*/
	  hash_set< /*const*/  EventID>ptr_event;

	  //luoluo 8-6
	  //to record the numbers of events which is belonged to the current node of the instance.
	  int num;

	  /**
	  *@brief	In order to show whether the itetator of the multimap should be delete.
	  *@luoluo 4-11
	  */
	   bool iterator_del;
	
private:
	/**
	* This variable indicates whether this node has been touched during current
	* event processing epoch.
	*/
	bool _bTouchedInEpoch;

	/**
	* This variable stores the first byte of the instance body (content).
	*/
	//char _body[1];



}; //class Instance
}  //namespace Automaton
}  //namespace BasicDataStructure
}  //namespace Cayuga


#endif //_INSTANCE_H
