/**
 * @file InstanceFinder.h
 * @date December 9, 2005
 * @brief Finds handles in instances for GC in Cayuga engine.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _INSTANCE_FINDER_H
#define _INSTANCE_FINDER_H

#include "arch-dep.h"

#include "BasicDataStructure/Automaton/NodeID.h"
using Cayuga::BasicDataStructure::Automaton::NodeID;

#include "BasicDataStructure/Event/EventID.h"
using Cayuga::BasicDataStructure::EventNS::EventID;


#include "MemoryMgr/CY_GCFace.h"

#include <list>
using namespace std;

namespace Cayuga {
namespace BasicDataStructure {
namespace Automaton {
class Instance;
class IntermediateNode;


/**
 * @brief An InstanceFinder object enumerates all live instances in Cayuga 
          engine.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date December 9, 2005
 * @version 1.0
 */
class InstanceFinder : public CY_Heap::RootFinder {
public:
	/**
	* @brief Default constructor
	* @param nodesWithInstance The set of nodes with instances at this time 
	         instance
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date Decemeber 9, 2005
	* @version 1.0
	*/
	InstanceFinder(hash_set<NodeID>& nodesWithInstance);
	
	/**
	* @brief Virtual destructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date Decemeber 9, 2005
	* @version 1.0
	*/
	virtual ~InstanceFinder();

	/**
	* @brief Fill the buffer with Handle values, and return the
		number of values added.
	* @param bufsz The maximum number of handle values to add
	* @param buf The handle buffer to add handles to
	* @return Number of handle values added. <= 0 at end of enumeration.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date Decemeber 9, 2005
	* @version 1.0
	*/
	virtual int fill(unsigned bufsz, CY_Heap::GHandle * buf);

	/**
	* @brief Reset the status of this instance finder.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date Decemeber 9, 2005
	* @version 1.0
	*/
	virtual void reset();

private:
	/**
	* This variable stores the current iterator position on nodes with 
	* instances.
	*/
	hash_set<NodeID>::iterator _currNode_it;
	
	/**
	* This variable stores the current iterator position on instances under
	* the current node being processed.
	*/
	list<Instance*>::iterator _currInstance_it;
	
	/**
	* This variable stores a reference to the set of nodes with instances.
	*/
	hash_set<NodeID>& _nodesWithInstance;
}; //class InstanceFinder
}  //namespace Automaton
}  //namespace BasicDataStructure
}  //namespace Cayuga

#endif //_INSTANCE_FINDER_H
