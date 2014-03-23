/**
 * @file HashIndex.h
 * @date November 27, 2005
 * @brief Definition of hash predicate indices.

		  Each instance has one hash table, which can index multiple attributes.

		  The definitions of member functions for this template class has to be
		  inlined below, due to the ame reason as in template class ValueIDMap
		  for GCC compiler . See comment in inc/Utils/ValueIDMap.h.

 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _HASH_INDEX_H
#define _HASH_INDEX_H

#include <iostream>
#include <set>
#include <cstring>
#include <cassert>
using namespace std;

#include "Index.h"

namespace Cayuga {
namespace Index {
struct HashTableMonitor;

#ifdef _DEBUG
	#define HASH_TABLE_MONITOR(x) x
#else 
	#define HASH_TABLE_MONITOR(x) ;
#endif

/**
 * @brief A HashIndex object represents a hash predicate index
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 27, 2005
 * @version 1.0
 */
template <typename ID_T, typename ID_SET_ENTRY, typename ID_SET>
class HashIndex : public Index<char*, ID_T, ID_SET_ENTRY, ID_SET> {
public:
	//typedef hash_set<ID_T> ID_SET;
	
	/**
	* @brief A Bucket object represents a hash table bucket for hash predicate
	         index.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	class Bucket {
	public:
		/**
		* @brief Default constructor
		* @author Mingsheng Hong (mshong@cs.cornell.edu)
		* @date November 27, 2005
		* @version 1.0
		*/
		Bucket();

		/**
		* @brief Virtual destructor
		* @author Mingsheng Hong (mshong@cs.cornell.edu)
		* @date November 27, 2005
		* @version 1.0
		*/
		~Bucket();

		/** 
		* @brief Insert a predicate in this bucket chain. 

				 Find a bucket with the same key as this predicate, and insert
				 it there. If there does not exist one, create a new bucket in
				 this bucket chain.
		* @param strPred The predicate to insert
		* @param predLen Size of the predicate
		* @param predID ID of the predicate
		* @return The pointer to the bucket where the predicate is inserted.
		* @author Mingsheng Hong (mshong@cs.cornell.edu)
		* @date November 27, 2005
		* @version 1.0
		*/
		Bucket* insertPred(char* strPred, 
			const size_t predLen, 
			const ID_SET_ENTRY predID
#ifdef _DEBUG
			, HashTableMonitor* hashTableMonitor
#endif 
			);

		/** 
		* @brief Change the predicate ID. 
		* @param oldID Old ID of the predicate
		* @param newID New ID of the predicate
		* @author Mingsheng Hong (mshong@cs.cornell.edu)
		* @date November 27, 2005
		* @version 1.0
		*/
		//void changePredID(const ID_T oldID, const ID_SET_ENTRY newID);

		/** 
		* @brief Delete the predicate from this bucket. 
		* @invariant The predicate must belong to this bucket. 
		* @param predID ID of the predicate to delete
		* @author Mingsheng Hong (mshong@cs.cornell.edu)
		* @date November 27, 2005
		* @version 1.0
		*/
		void deletePred(const ID_T predID);

		/** 
		* @brief Get predicate ID set of the bucket matching the key under this 
				 bucket chain.
		* @param strPred Key of the predicate
		* @param predLen Length of the predicate key
		* @return The predicate ID set of the bucket with matching key. If no
		          such bucket is found, return NULL.
		* @author Mingsheng Hong (mshong@cs.cornell.edu)
		* @date November 27, 2005
		* @version 1.0
		*/
		ID_SET* getPredIDs(char* strPred, const size_t predLen
#ifdef _DEBUG
						   , HashTableMonitor* hashTableMonitor
#endif
						   );

		/**
		* This variable stores the key of this bucket.
		*/
		char* key;
		
		/**
		* This variable stores the pointer to the next bucket under this bucket
		* chain.
		*/
		Bucket* pNextBucket;
		
		/** 
		* This variable stores a set of predicate IDs associated with this 
		* bucket. 
		*/
		ID_SET _predIDs;
	};
		
	/**
	* @brief Default constructor
	* @param inputAssemblyProgram The input key assembly program
	* @param sizeBucket Size of index bucket
	* @param numBuckets Number of buckets
	* @param bForFREdge Indicate whether this index is for FR edges
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	HashIndex(char* inputAssemblyProgram, 
			  const size_t sizeBucket,
			  const size_t numBuckets,
			  bool bForFREdge = false);

	/**
	* @brief Virtual destructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual ~HashIndex();


	/**
	* @brief Evaluates the current event against the set of predicates indexed 
			 here, and returns the IDs of matched predicates
	* @param ev The content of the current event
	* @param predIDs The set of satisfied predicate IDs to return
	* @param pNode NULL if this index works globally. Not NULL if this index
	         only works on predicates on edges associated with this node.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual void eval(const char* ev, 
					  ID_SET*& predIDs, 
					  const Node* pNode = NULL);

	/**
	* @brief Insert the predicate into this index.
	* @param strPred The predicate to insert
	* @param predID The ID of this predicate
	* @param predIDEntry The ID set entry of this predicate
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/  
	virtual void insertPred(char* strPred, 
		const ID_T predID, 
		const ID_SET_ENTRY predIDEntry);

	/**
	* @brief Delete the predicate from this index.
	* @param predID The ID of the predicate to delete
	* @return True iff this predicate is located and deleted from this index.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/  
	virtual bool deletePred(ID_T predID);


	/**
	* @brief Get the key of the predicate
	* @param predID The ID of the predicate
	* @return The pointer to the pred key. 
			  The hash index still owns this string. 
			  If the predID is not in this index, return NULL.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/  
	virtual char* getPredKey(ID_T predID);

	/** 
	* @brief Change the predicate ID. 
	* @param oldID Old ID of the predicate
	* @param newID New ID of the predicate
	* @param newIDEntry New ID set entry of the predicate
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	//virtual void changePredID(const ID_T oldID, 
	//	const ID_SET_ENTRY newID, 
	//	const ID_SET_ENTRY newIDEntry);

	/**
	* @brief Return the size of the bucket.
	* @return As above
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/ 
	size_t getInputSize();

	/**
	* @brief Output member variable information in XML format.
	* @param out The output stream handle.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 22, 2005
	* @version 1.0
	* @todo Implement this function
	*/
	virtual void write(ostream& out = cout);

	/**
	* @brief Output the statistics collected in this index.
	* @param out The output stream handle.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date October 27, 2006
	* @version 1.0
	*/
	virtual void writeStats(ostream& out = cout);

	/**
	* @brief Return true iff this index has no entries.
	* @Return As above.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 6, 2006
	* @version 1.0
	*/
	virtual bool empty();

	/**
	* @brief Return a unique ID of this index.
	* @Return As above.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 28, 2007
	* @version 1.0
	*/
	size_t getID();

	//luoluo 3.16
	char  gotkey();

 private:
	/**
	* @brief Return the hashed bucket ID of this key
	* @param key The input predicate key
	* @return The hashed bucket ID
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 22, 2005
	* @version 1.0
	*/
	size_t hash(const char* key);

	/** 
	* This variable stores the size of hash table bucket.
	*/
	size_t _sizeBucket;
	
	/** 
	* This variable stores the number of buckets in hash table.
	*/
	size_t _numBuckets;
	
	/** 
	* This variable stores pointer to the buckets.
	*/
	Bucket* _buckets;

	/** 
	* This variable stores the map from predicate IDs to hash table buckets 
	*/
	hash_map<ID_T, Bucket*> _PredBucketMap;

	/** 
	* This variable stores the Interpreter program to assemble the hash index 
	* input from event 
	*/
	char* _inputAssemblyProgram;

	/** 
	* This variable stores the current hash index input.
	*/
	char* _input;

	/** 
	* This variable stores an empty set of predicate IDs.
	* It is returned to user by eval if there is no pred matching this event. 
	*/
	ID_SET _defaultPredIDs;

	/** 
	* This variable indicates whether this index is for FR edges.  
	* If so, the node identifier will be the last component of the index key 
	*/
	bool _bForFREdge;

	/** 
	* This variable stores the monitor for the hash index for collecting stats.
	*/
	HASH_TABLE_MONITOR(HashTableMonitor* _hashTableMonitor);

	/** 
	* This variable stores the unique ID of this index.
	*/
	size_t _id;

	static size_t _idCnt;

}; //class HashIndex
}  //namespace Index
}  //namespace Cayuga


#endif //_HASH_INDEX_H

