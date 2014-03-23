/**
* @file HashIndexDef.h
* @date October 26, 2006
* @brief The file contains the definition of template class HashIndex.

* @author Mingsheng Hong (mshong@cs.cornell.edu)
*/

#pragma once

#include <iostream>
#include <set>
#include <cstring>
#include <cassert>
using namespace std;

#include "HashIndex.h"
using namespace Cayuga::Index;

#include "BasicDataStructure/Automaton/node.h"
using Cayuga::BasicDataStructure::Automaton::Node;

#include "QueryProcessor/Interpreter.h"
using Cayuga::QueryProcessor::Interpreter;

#include "AutomatonCompiler/ByteCodeUtils.h"
using Cayuga::AutomatonCompiler::overwriteArrayFieldInCPInstruction;

////////////////////////////////////////////////////////////////////////////////
// HashTableMonitor function definitions
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief A HashTableMonitor object collects stats about a Cayuga hash table.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 27, 2005
 * @version 1.0
 */
struct Cayuga::Index::HashTableMonitor {
	HashTableMonitor();

	void write(ostream& out = cout);

	int _numBucketCollisions;
	int _numRedundantBucketVisits;
};

HashTableMonitor::HashTableMonitor() : 
_numBucketCollisions(0), _numRedundantBucketVisits(0) {
}

void HashTableMonitor::write(ostream& out) {
	out<<"number of bucket collisions: "<<_numBucketCollisions<<endl;
	out<<"number of wasted bucket visits during event processing: "
		<<_numRedundantBucketVisits<<endl;

}

////////////////////////////////////////////////////////////////////////////////
// HashIndex::Bucket function definitions
////////////////////////////////////////////////////////////////////////////////

template <typename ID_T, typename ID_SET_ENTRY, typename ID_SET> 
HashIndex<ID_T, ID_SET_ENTRY, ID_SET>::Bucket::Bucket()  
: key(NULL), pNextBucket(NULL) {
}

template <typename ID_T, typename ID_SET_ENTRY, typename ID_SET> 
HashIndex<ID_T, ID_SET_ENTRY, ID_SET>::Bucket::~Bucket()  {
	delete[] key;
	delete pNextBucket;
}


template <typename ID_T, typename ID_SET_ENTRY, typename ID_SET> 
typename HashIndex<ID_T, ID_SET_ENTRY, ID_SET>::Bucket* 
HashIndex<ID_T, ID_SET_ENTRY, ID_SET>::Bucket::insertPred(char* strPred, 
				   const size_t predLen, 
				   const ID_SET_ENTRY predIDEntry
#ifdef _DEBUG
				   , HashTableMonitor* hashTableMonitor
#endif 
				   ) {
   if (predLen == 0 
	   || key == NULL || !memcmp(strPred, key, predLen)) {
		   if (predLen != 0) {
			   if (key == NULL) {
				   key = strPred;
			   }
			   else {
				   delete []strPred;
			   }
		   }
		   pair<typename ID_SET::iterator, bool> ret 
			   = _predIDs.insert(predIDEntry);
		   assert (ret.second);
		   return this;
   }
   else {
	   HASH_TABLE_MONITOR(++hashTableMonitor->_numBucketCollisions);

	   //insert the predicate in pNextBucket
	   if (pNextBucket == NULL) {
		   pNextBucket = new Bucket();
	   }	
	   return pNextBucket->insertPred(strPred, predLen, predIDEntry
#ifdef _DEBUG
		   , hashTableMonitor
#endif
		   );
   }
}

//template <typename ID_T, typename ID_SET_ENTRY, typename ID_SET> 
//void HashIndex<ID_T, ID_SET_ENTRY, ID_SET>::Bucket::changePredID(
//	const ID_T oldID, const ID_SET_ENTRY newIDEntry) {
//	assert (key != NULL);
//	_predIDs.erase(oldID);
//	_predIDs.insert(newIDEntry);
//}

template <typename ID_T, typename ID_SET_ENTRY, typename ID_SET> 
void HashIndex<ID_T, ID_SET_ENTRY, ID_SET>::Bucket::deletePred(const ID_T predID) {
	size_t ret = _predIDs.erase(predID);
	assert (ret == 1);
}

template <typename ID_T, typename ID_SET_ENTRY, typename ID_SET> 
ID_SET* HashIndex<ID_T, ID_SET_ENTRY, ID_SET>::Bucket::getPredIDs(
	char* strPred, const size_t predLen
#ifdef _DEBUG
				   , HashTableMonitor* hashTableMonitor
#endif
				   ) {
   if (key == NULL) {
	   assert (_predIDs.empty());
	   return NULL;
   }
   else if (!memcmp(strPred, key, predLen)) {
	   return &_predIDs;
   }
   else {
	   HASH_TABLE_MONITOR(
		   ++hashTableMonitor->_numRedundantBucketVisits);
	   if (pNextBucket == NULL) {
		   return NULL;
	   }	
	   return pNextBucket->getPredIDs(strPred, predLen
#ifdef _DEBUG
		   , hashTableMonitor
#endif
		   );
   }
}

////////////////////////////////////////////////////////////////////////////////
// HashIndex function definitions
////////////////////////////////////////////////////////////////////////////////

template <typename ID_T, typename ID_SET_ENTRY, typename ID_SET> 
size_t HashIndex<ID_T, ID_SET_ENTRY, ID_SET>::_idCnt = 0;

 
template <typename ID_T, typename ID_SET_ENTRY, typename ID_SET> 
HashIndex<ID_T, ID_SET_ENTRY, ID_SET>::HashIndex(char* inputAssemblyProgram, 
		  const size_t sizeBucket,
		  const size_t numBuckets,
		  bool bForFREdge) 
	: _sizeBucket(sizeBucket), _numBuckets(numBuckets), 
	_inputAssemblyProgram(inputAssemblyProgram),
	_bForFREdge(bForFREdge), _id(_idCnt++) {
	  _sizeBucket = (_bForFREdge? _sizeBucket+4 : _sizeBucket);

	  //this index degenerates to a sequential list if size = 0
	  if (_sizeBucket != 0) {
		  _input  = new char[_sizeBucket];
		  _buckets = new Bucket[_numBuckets];
	  }	

	  HASH_TABLE_MONITOR(_hashTableMonitor = new HashTableMonitor());
}

template <typename ID_T, typename ID_SET_ENTRY, typename ID_SET> 
HashIndex<ID_T, ID_SET_ENTRY, ID_SET>::~HashIndex() {
	if (_sizeBucket != 0) {
		delete[] _buckets;
		delete[] _input;
	}
	
	HASH_TABLE_MONITOR(delete _hashTableMonitor);

}


template <typename ID_T, typename ID_SET_ENTRY, typename ID_SET> 
void HashIndex<ID_T, ID_SET_ENTRY, ID_SET>::eval(const char* ev, 
				  ID_SET*& predIDs, 
				  const Node* pNode) {
	if (_sizeBucket == 0) {
	  //return the sequential list
	  predIDs = &_defaultPredIDs;
	  return;
	}

	//overwrite the ev and destInstanceArray field in the first
	//instruction (CP) of _inputAssemblyProgram
	overwriteArrayFieldInCPInstruction(
	  _inputAssemblyProgram,
	  ev, 
	  NULL, 
	  _input);		

	Interpreter::getInstance()->exec(_inputAssemblyProgram);		
	//assemble the nodeID as last part of the index key
	if (pNode) {
	 // *(const Node**)(_input+_sizeBucket-4) = pNode;
	  *(NodeID*)(_input+_sizeBucket-4) = pNode->getID();
	}

	size_t bucketID = hash(_input);
	assert (bucketID >= 0 && bucketID < _numBuckets);
	predIDs = _buckets[bucketID].getPredIDs(_input, _sizeBucket
	#ifdef _DEBUG
	  , _hashTableMonitor
	#endif
	  );
	if (predIDs == NULL) {
	  predIDs = &_defaultPredIDs;
	}
}

template <typename ID_T, typename ID_SET_ENTRY, typename ID_SET>  
void HashIndex<ID_T, ID_SET_ENTRY, ID_SET>::insertPred(
	char* strPred, 
	const ID_T predID, 
	const ID_SET_ENTRY predIDEntry) {
	if (_sizeBucket == 0) {
		_defaultPredIDs.insert(predIDEntry);
		return;
	}
	size_t bucketID = hash(strPred);
	assert (bucketID >= 0 && bucketID < _numBuckets);
	Bucket* pBucket = _buckets[bucketID].insertPred(strPred, 
		_sizeBucket, 
		predIDEntry
#ifdef _DEBUG
		, _hashTableMonitor
#endif
		);

	_PredBucketMap.insert(make_pair(predID, pBucket));
}

template <typename ID_T, typename ID_SET_ENTRY, typename ID_SET> 
bool HashIndex<ID_T, ID_SET_ENTRY, ID_SET>::deletePred(ID_T predID) {
	if (_sizeBucket == 0 
		&& _defaultPredIDs.find(predID) != _defaultPredIDs.end()) {
			/*size_t numDeletedEntries = */ _defaultPredIDs.erase(predID);
			//cout<<"number of deleted entries from the dummy predicate is "
			//	<<numDeletedEntries<<endl;
			return true;
	}

	typename hash_map<ID_T, Bucket*>::iterator find_it =
		_PredBucketMap.find(predID);
	//assert (find_it != _PredBucketMap.end());
	if (find_it != _PredBucketMap.end()) {
		find_it->second->deletePred(predID);
		_PredBucketMap.erase(find_it);
		return true;
	}
	else {
		return false;
	}
}


template <typename ID_T, typename ID_SET_ENTRY, typename ID_SET> 
char* HashIndex<ID_T, ID_SET_ENTRY, ID_SET>::getPredKey(ID_T predID)  {
	typename hash_map<ID_T, Bucket*>::iterator find_it = 
		_PredBucketMap.find(predID);
	//assert (find_it != _PredBucketMap.end());
	if (find_it != _PredBucketMap.end()) {
		return find_it->second->key;
	}
	else {
		return NULL;
	}
}

//template <typename ID_T, typename ID_SET_ENTRY, typename ID_SET> 
//bool HashIndex<ID_T, ID_SET_ENTRY, ID_SET>::changePredID(
//	const ID_T oldID, 
//	const ID_SET_ENTRY newID, 
//	const ID_SET_ENTRY newIDEntry) {
//	typename hash_map<ID_T, Bucket*>::iterator find_it = 
//		_PredBucketMap.find(oldID);
//	//assert (find_it != _PredBucketMap.end());
//
//	if (find_it != _PredBucketMap.end()) {
//		Bucket* pBucket = find_it->second;
//		_PredBucketMap.erase(find_it);
//		pBucket->changePredID(oldID, newIDEntry);
//		_PredBucketMap.insert(make_pair(newID, pBucket));
//		return true;
//	}
//	else {
//		return false;
//	}
//}

template <typename ID_T, typename ID_SET_ENTRY, typename ID_SET> 
size_t HashIndex<ID_T, ID_SET_ENTRY, ID_SET>::getInputSize()  {
	return _sizeBucket;
}

template <typename ID_T, typename ID_SET_ENTRY, typename ID_SET> 
void HashIndex<ID_T, ID_SET_ENTRY, ID_SET>::write(ostream& out) {
	out<<"content of the hash index "<<endl;
// 	//luoluo 3.16
// 	//experiment : output the indices of the Fiter or FREval
// 	
// 	for (hash_map<ID_T, Bucket*>::const_iterator hash_bucket=_PredBucketMap.begin();
// 		hash_bucket!=_PredBucketMap.end();hash_bucket++)
// 	{
// 		out<<"**1**"<<hash_bucket->second->_predIDs<<endl;
// 			out<<"**2**"<<hash_bucket->second->key<<"**3**"<<endl;
// 	}
}

template <typename ID_T, typename ID_SET_ENTRY, typename ID_SET> 
void HashIndex<ID_T, ID_SET_ENTRY, ID_SET>::writeStats(ostream& out) {
	HASH_TABLE_MONITOR(this->_hashTableMonitor->write(out));
}

template <typename ID_T, typename ID_SET_ENTRY, typename ID_SET> 
bool HashIndex<ID_T, ID_SET_ENTRY, ID_SET>::empty() {
	return _PredBucketMap.empty();
}

template <typename ID_T, typename ID_SET_ENTRY, typename ID_SET> 
size_t HashIndex<ID_T, ID_SET_ENTRY, ID_SET>::hash(const char* key) {
	//return 0;
	assert (_sizeBucket >= 4);

	//get byte 4 to 7 and modulo it with _numBuckets
	//size_t midKey = *(size_t*)(key+4);
	//return midKey % _numBuckets;

	//one at a time hash function
	size_t hash, i;
	for (hash = 0, i = 0; i < _sizeBucket; ++i) {
		hash += key[i];
		hash += (hash << 10);
		hash ^= (hash >> 6);
	}
	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);
	return hash % _numBuckets;
}

template <typename ID_T, typename ID_SET_ENTRY, typename ID_SET> 
size_t HashIndex<ID_T, ID_SET_ENTRY, ID_SET>::getID() {
	return _id;
}


template <typename ID_T, typename ID_SET_ENTRY, typename ID_SET> 
char HashIndex<ID_T, ID_SET_ENTRY, ID_SET>::gotkey() {
    return *(_buckets->key);
}