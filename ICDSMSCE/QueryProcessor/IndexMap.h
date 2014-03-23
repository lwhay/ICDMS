/**
 * @file IndexMap.h
 * @date November 26, 2005
 * @brief Maps index specs (sequence of attribute IDs) to indices. 
 
	      It is used by FilterEvaluator and FREvaluator.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _INDEX_MAP_H
#define _INDEX_MAP_H

#include "arch-dep.h"


#include <vector>
#include <cassert>
using namespace std;

namespace Cayuga {
namespace QueryProcessor {
using Cayuga::BasicDataStructure::Predicate::AttributeID;

typedef vector<AttributeID> _HashPredIndexSpec;
typedef long long IndexSpecKeyType;

#define NUM_BUCKETS 1024

/**
 * @brief An IndexMap object maps index specs (sequence of attribute IDs) to 
		  indices. 
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 26, 2005
 * @version 1.0
 */
template <typename ID_T>
class IndexMap {
public:
	/**
	* @brief Insert an index spec, index pair
	* @param hashIndexSpec Index specification
	* @param pHashIndex Pointer to the index
	* @return True if this entry is successfully inserted. False if this entry
			  always exists.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 26, 2005
	* @version 1.0
	*/
	bool insert(const _HashPredIndexSpec& hashIndexSpec, ID_T* pHashIndex) {
		IndexSpecKeyType hashKey = 0;
		for (vector<AttributeID>::const_iterator it = hashIndexSpec.begin();
			it != hashIndexSpec.end(); ++it) {
				hashKey = hashKey*MAX_NUM_EVENT_ATTRS + *it;
		}
		pair<typename hash_map<IndexSpecKeyType, ID_T*>::iterator, bool> ret 
			=  _indexSpecs.insert(make_pair(hashKey, pHashIndex));
		return ret.second;
	}

	/**
	* @brief Locate the index by the specification.
	* @param hashIndexSpec The index specification to find the index
	* @return Pointer to the index corresponding to the input index spec if
			  the index exists, and NULL otherwise.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 26, 2005
	* @version 1.0
	*/
	ID_T* find(const _HashPredIndexSpec& hashIndexSpec) {
		IndexSpecKeyType hashKey = 0;
		for (vector<AttributeID>::const_iterator it = hashIndexSpec.begin();
			it != hashIndexSpec.end(); ++it) {
				hashKey = hashKey*MAX_NUM_EVENT_ATTRS + *it;
		}
		typename hash_map<IndexSpecKeyType, ID_T*>::iterator find_it 
			=  _indexSpecs.find(hashKey);
		if (find_it != _indexSpecs.end())
			return find_it->second;

		return NULL;
	}

private:
	/**
	* This variable defines the maximum number of attributes in an event stream.
	*/
	static const int MAX_NUM_EVENT_ATTRS = 10; 
	
	/**
	* This variable stores the set of index spec, index pairs.
	*/
	hash_map<IndexSpecKeyType, ID_T*> _indexSpecs;
}; //class IndexMap
}  //namespace QueryProcessor
}  //namespace Cayuga

#endif //_INDEX_MAP_H
