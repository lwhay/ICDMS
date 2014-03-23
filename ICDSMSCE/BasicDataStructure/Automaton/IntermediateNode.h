/**
 * @file IntermediateNode.h
 * @date November 27, 2005
 * @brief Stores intermediate nodes (including end nodes) in Cayuga engine.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _INTERMEDIATE_NODE_H
#define _INTERMEDIATE_NODE_H


#include "StartNode.h"

#include "BasicDataStructure/Event/EventID.h"
using Cayuga::BasicDataStructure::EventNS::EventID;

#include "BasicDataStructure/StreamID.h"
#include "BasicDataStructure/OutputStreamType.h"

#include "arch-dep.h"

#include "BasicDataStructure/Window/predWindow.h"

#include <list>
using namespace std;


namespace Cayuga {

namespace QueryProcessor {
class FilterIndex;
}

namespace SystemMgr {
class TraceMgr;
}

namespace BasicDataStructure {
namespace Automaton {
using Cayuga::BasicDataStructure::StreamID;
using Cayuga::SystemMgr::StreamSchema;

/**
 * @brief An IntermediateNode object represents an intermediate or end node in 
          Cayuga engine.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 27, 2005
 * @version 1.0
 */
class IntermediateNode : public StartNode {	
	typedef vector<pair<AttributeID, AttributeID> > _HashDynPredIndexSpec;
	typedef HashIndex<EventID, pair<EventID, Instance*>, 
		hash_map<EventID, Instance*> > DynHashIndexType;	
public:
	friend class Cayuga::QueryProcessor::FilterIndex;
	friend class Cayuga::QueryProcessor::FREvaluator;
	friend class Cayuga::QueryProcessor::QueryProcessorImpl;
	friend class InstanceFinder;
	friend class Query;
	friend class Cayuga::SystemMgr::TraceMgr;

	/**
	* @brief Default constructor
	* @param inputStream Input stream of this node
	* @param pNodeSchema The node schema
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	IntermediateNode(const StreamID inputStream, 
					 StreamSchema* pNodeSchema);

	/**
	* @brief Virtual destructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual ~IntermediateNode();

	/**
	* @brief Output member variable information in XML format.
	* @param out The output stream handle.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual void write(ostream& out = cout) const;

	/**
	* @brief Add an out-going edge to this node.
	* @param pEdge Pointer to the edge to add
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual void addEdge(Edge* pEdge);

	/**
	* @brief Set the length of the node schema.
	* @param schemaLen The schema length to set to.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual void setSchemaLen(const size_t schemaLen);

	/**
	* @brief Get the length of the node schema.
	* @return As above.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual size_t getSchemaLen() const;

	/**
	* @brief Get the the node schema.
	* @return As above.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual StreamSchema* getNodeSchema() const;

	/**
	* @brief Return true iff this node has the same content as that of the 
			 input node.
    * @param rhs The node to compare to
	* @return As above.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	bool operator ==(const IntermediateNode& rhs) const;

	/**
	* @brief Add output stream to this node.
	* @param outputStream The output stream to add to
	* @param isOut whether the stream has to be written to disc/network.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	void addOutputStream(const StreamID outputStream,
						const bool isOut);

	/**
	* @brief Return true this node has some output stream.
	* @return As above.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	bool hasOutputStream();

	/**
	* @brief Mark the particular output stream to be resub stream.
	* @param sid The output stream to mark.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 5, 2006
	* @version 1.0
	*/
	void markOutputStreamResub(const StreamID sid);

	/**
	* @brief Add a new instance under the pending list of the node.
	* @param pNewInstance Pointer to instance to add
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual void addNewInstance(Instance* pNewInstance);

	/**
	* @brief Return true iff this node current has instances.
	* @return As above
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual bool hasInstance() const;
	
	/**
	* @brief Get the number of handle bytes in the node schema.
	* @return As above.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual size_t getHandleBytes() const;

	/**
	* @brief Copy the handle bytes in the instance to the handle buffer provided
			 by the garbage collector (GC).
	* @param buf Pointer to the handle buffer provided by GC.
	* @param pCurInstance Pointer to the instance being processed.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual void copyHandles(CY_Heap::GHandle* buf, Instance* pCurInstance);

	/**
	* @brief Returns whether any of the output streams associated with the node
			 has to be output to the Disk/Network.
	* @author Varun Sharma (varun13687@yahoo.com)
	* @date June 16, 2006
	* @version 1.0
	*/

	//bool toBeOutput() const;

	/**
	* @brief Updates occupancy counter of the node
	* @author Varun Sharma (varun13687@yahoo.com)
	* @date June 22, 2006
	* @version 1.0
	*/
	//void incCounter();

	/**
	* @brief Increments occupancy counter of the node
	* @author Varun Sharma (varun13687@yahoo.com)
	* @date June 22, 2006
	* @version 1.0
	*/
	//void updateCounter();

	/**
	* @brief returns occupancy counter of the node
	* @return Value stored in occupancy counter
	* @author Varun Sharma (varun13687@yahoo.com)
	* @date June 22, 2006
	* @version 1.0
	*/
	//size_t getCounter() const;

	/**
	* @brief Sets the Dynamic Filter Pred Index and Internalized Dynamic 
	*        Index Spec
	* @param dynFilterPredIndex Pointer to the HashIndex allocated
	* @param dynIndex The Internalized Dynamic Index Spec corresponding to the
	         Index
	* @author Varun Sharma (varun13687@yahoo.com)
	* @date June 26, 2006
	* @version 1.0
	*/
	void setDynIndex(DynHashIndexType* dynFilterPredIndex,
					_HashDynPredIndexSpec dynIndexSpec);

	
	/**
	* @brief returns pointer to the dynamic filter predicate index
	* @return pointer to dynamic filter predicate index
	* @author Varun Sharma (varun13687@yahoo.com)
	* @date June 26, 2006
	* @version 1.0
	*/
	DynHashIndexType* getDynFilterPredIndex() const;

	/**
	* @brief returns the dynamic filter predicate index spec
	* @return dynamic filter predicate index spec
	* @author Varun Sharma (varun13687@yahoo.com)
	* @date June 26, 2006
	* @version 1.0
	*/
	_HashDynPredIndexSpec getDynIndexSpec() const;

	/**
	* @brief returns whether this node has dynamic indexing enabled
	* @return true if _dynFilterPredIndex is not null
	* @author Varun Sharma (varun13687@yahoo.com)
	* @date June 26, 2006
	* @version 1.0
	*/
	bool hasDynFilterIndex() const;

	/**
	* @brief set the clause number with which the single Dynamic Filter
		     Predicate Index is associated.
	* @param dynIndexedClause The Clause Number
	* @author Varun Sharma (varun13687@yahoo.com)
	* @date June 26, 2006
	* @version 1.0
	*/
	void setDynIndexedClause(int dynIndexedClause);
	
	/**
	* @brief since one Dynamic Filter Index is associated with a particular
	         clause, returns whether the Dyn Filter Index is already associated
			 or not (if an index covers 2 clauses, we take the 1st to be covered
			 and leave the other)
	* @return true if clause number is already set
	* @author Varun Sharma (varun13687@yahoo.com)
	* @date June 26, 2006
	* @version 1.0
	*/
	bool noDynIndexedClause() const;

	/**
	* @brief check whether this filter predicate clause is dynamically indexed
	         or not
	* @param clauseno the clause number to check against
	* @return true if the clause numbers match false otherwise
	* @author Varun Sharma (varun13687@yahoo.com)
	* @date June 26, 2006
	* @version 1.0
	*/
	bool isClauseDynIndexed(size_t clauseno) const;

	//luoluo 8-2
	//let the prediecate window belong to 
	void setPredWindow(predWindow * ptr);
	predWindow * returnPredWindow();

protected:		
	/**
	* @brief Output the output stream information in XML format.
	* @param out The output stream handle.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	void _writeOutputStreams(ostream& out = cout) const;

	/** 
	* This variable stores the optional filter edge.
	* NULL if there is not filter edge on this node.
	*/
	FilterEdge* _pFilter;

	/** 
	* This variable stores the optional rebind edge.
	* NULL if there is not rebind edge on this node.
	*/
	RebindEdge* _pRebind;

	/** 
	* This variable stores the schema of this node.
	*/
	StreamSchema* _pNodeSchema;

	/** 
	* This variable stores the length of the node schema 
	*/
	size_t _schemaLen;

	/** 
	* This variable stores the set of output streams, which could be empty. 	
	*/
	//vector<pair<StreamID, bool> > _outputStreams;
	vector<OutputStreamType> _outputStreams;

	/**
	* This variable stores the output flags (to be passed to IO or not)
	* corresponding to the outputstreams vector
	*/
	//vector<bool> _streamsToBeOutput;

	/**
	* This variable stores whether this node has an associated output
	* stream which needs to be output to the Disc/Network
	* @invariant The value of this variable is always ORed from s.bOutput
				for all s in _outputStreams
	*/
	bool _toBeOutput;
	
	/** 
	* This variable stores the list of current instances of this node.
	*/
	list<Instance*> _instances;

	/** 
	* This variable stores the list of pending instances of this node.
	*/
	list<Instance*> _newInstances;

	/** 
	* This variable stores the set of affected current instances of this node.
	*/
	hash_map<EventID, Instance*> _affectedInstances;

	/**
	* This variable updates the counter representing average occupancy of the
	* node.
	*/
	//size_t counter;

	/**
	* This varibale stores the internalized Dynamic AI index specs.
	*/
	_HashDynPredIndexSpec _dynIndexSpec;

	/**
	* This variable stores the Filter Predicate clause which is dynamically 
	* indexed.
	*/
	int _dynIndexedClause;

	/**
	* This variable stores the Dynamic Filter Hash Index
	*/
	DynHashIndexType* _dynFilterPredIndex;

	//luoluo  8-2
	predWindow * _predWindow;
    
}; //class IntermediateNode
}  //namespace Automaton
}  //namespace BasicDataStructure
}  //namespace Cayuga

#endif //_INTERMEDIATE_NODE_H
