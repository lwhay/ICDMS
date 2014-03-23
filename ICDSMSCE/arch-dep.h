/**
 * @file arch-dep.h
 * @date November 26, 2005
 * @brief Platform depedent code should go here.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _ARCH_DEP_H
#define _ARCH_DEP_H

#if defined(_MSC_VER)
#include <hash_set>
#include <hash_map>
using namespace stdext;

//on Windows, only _snprintf is defined.
//but GCC has snprintf
#define snprintf _snprintf  

#elif defined(__GNUC__)
//#elif defined(__GLIBCPP__) || defined(__GLIBCXX__)  // Using GNU C++ library?
#include <ext/hash_set>
#include <ext/hash_map>
using namespace __gnu_cxx;

namespace Cayuga {
namespace BasicDataStructure {
namespace Automaton {
class Node;
class IntermediateNode;
class Edge;
class Instance;

//namespace EventNS {
//class Event;
//}  //namespace EventNS
}  //namespace Automaton
}  //namespace BasicDataStructure

namespace Index {
	template <typename ID_T, typename ID_SET_ENTRY, typename ID_SET> 
		class HashIndex;
}

}  //namespace Cayuga

#include "BasicDataStructure/Automaton/EdgeID.h"
using Cayuga::BasicDataStructure::Automaton::EdgeID;

#include "BasicDataStructure/Automaton/NodeID.h"
using Cayuga::BasicDataStructure::Automaton::NodeID;

using Cayuga::BasicDataStructure::Automaton::Node;
using Cayuga::BasicDataStructure::Automaton::IntermediateNode;
using Cayuga::BasicDataStructure::Automaton::Edge;
using Cayuga::BasicDataStructure::Automaton::Instance;

//using Cayuga::BasicDataStructure::EventNS::Event;

#include "BasicDataStructure/Event/EventID.h"
using Cayuga::BasicDataStructure::EventNS::EventID;

//For the GNU implementation, the hash function for std:string is
//missing, and so hash_set<string> cannot be used!
namespace __gnu_cxx
{
	template<> struct hash< std::string >
	{
		size_t operator()( const std::string& x ) const
		{
			return hash< const char* >()( x.c_str() );
		}
	};
	
	template<> struct hash< const std::string >
	{
		size_t operator()( const std::string& x ) const
		{
			return hash< const char* >()( x.c_str() );
		}
	};	
	
	template<> struct hash<const size_t> {
		size_t operator()(const size_t __x) const { return (size_t)__x; }
	};

	//adapted from functions in ext/stl_hash_fun.h
	template<> struct hash<long long> {
		size_t operator()(long long __x) const { return (size_t)__x; }
	};

	//template<> struct hash<Instance*> {
	//	size_t operator()(Instance* __x) const { return (size_t)__x; }
	//};

	//template<> struct hash<Node*> {
	//	size_t operator()(Node* __x) const { return (size_t)__x; }
	//};

	//template<> struct hash<IntermediateNode*> {
	//	size_t operator()(IntermediateNode* __x) const { return (size_t)__x; }
	//};

	template<> struct hash<Edge*> {
		size_t operator()(Edge* __x) const { return (size_t)__x; }
	};

	template<> struct hash<std::pair<long, size_t>*> {
		size_t operator()(std::pair<long, size_t>* __x) const { 
			return (size_t)__x; 
		}
	};

	/*
	template<> struct hash<std::pair<NodeID, size_t>*> {
		size_t operator()(std::pair<NodeID, size_t>* __x) const { 
			return (size_t)__x; 
		}
	};

	template<> struct hash<Cayuga::Index::HashIndex<
		std::pair<EdgeID, size_t>*>*, 
		std::pair<EdgeID, size_t>*, hash_set<EdgeID, size_t>*> > {
		size_t operator()(Cayuga::Index::HashIndex<
						  std::pair<EdgeID, size_t>*>* __x) const {
			return (size_t)__x; 
		}
	};
	*/

	/*
	template<> struct hash<Cayuga::Index::HashIndex<
		std::pair<long, size_t>*>* > {
			size_t operator()(Cayuga::Index::HashIndex<
				std::pair<EdgeID, size_t>*>* __x) const {
					return (size_t)__x; 
			}
	};
	*/

	/*
	template<> struct hash<EventID> {
		size_t operator()(EventID __x) const { return (size_t)__x; }
	};
	*/

	/*
	template<> struct hash<std::pair<Node*, unsigned int>* > {
		size_t operator()(std::pair<Node*, unsigned int>* __x) const { 
			return (size_t)__x; 
		}
	};
	
	template<> struct hash<std::pair<Edge*, unsigned int>* > {
		size_t operator()(std::pair<Edge*, unsigned int>* __x) const { 
			return (size_t)__x; 
		}
	};
	*/
	
}
#endif //_MSC_VER

#endif //_ARCH_DEP_H
