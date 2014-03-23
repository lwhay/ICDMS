/**
 * @file ValueIDMap.h
 * @date November 27, 2005
 * @brief Defines a utility calss for constructing a symbol table of 
		  value/integer ID pairs that allows fast look-up in both directions. 

		  Microsoft C++ Compiler (MSVC 8) has better template support. To define
		  a template class there, one only needs to put the class declaration,
		  along with instantiations in the header file, and then put method
		  function definitions in a separate source file as usually.
		  
		  However, for GCC, we have to inline all the definitions of member
		  functions. This increases compilation time and complicates 
		  maintenance.

 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */


#ifndef _VALUE_ID_MAP_H
#define _VALUE_ID_MAP_H

//disable the following non-sense MSVC compiler warning:
//no suitable definition provided for explicit template instantiation request
#ifdef _MSC_VER
#  pragma warning(disable:4661)
#endif

#include <vector>
#include <string>
#include <iostream>
using namespace std;

#include "arch-dep.h"

namespace Cayuga {
	
namespace BasicDataStructure {
namespace Automaton {
class Node;
class IntermediateNode;
}
}
namespace AutomatonCompiler {
class SchemaMapString;
}

namespace Utils {
using Cayuga::BasicDataStructure::Automaton::Node;
using Cayuga::BasicDataStructure::Automaton::IntermediateNode;
using Cayuga::AutomatonCompiler::SchemaMapString;

template <typename T> 
bool operator ==(const vector<T>& lhs,
				 const vector<T>& rhs);


/**
 * @brief A ValueIDMap object represents a symbol table of value/integer ID 
		  pairs that allows fast look-up in both directions. 
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 27, 2005
 * @version 1.0
 */
template <typename T>
class ValueIDMap {
public:
	friend class Cayuga::AutomatonCompiler::SchemaMapString;
	//friend class Cayuga::QLCompiler::CayugaQLASTVisitor;

	typedef vector<T> ValueSequence;
	typedef hash_map<const T, unsigned> ValueMap;


	/**
	* @brief Default constructor.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	ValueIDMap();

	/**
	* @brief Destructor.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	~ValueIDMap();

	/**
	* @brief Output member variable information in non-XML format.
	* @param out The output stream handle.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	void write(ostream& out = cout) const;

	/**
	* @brief Get the ID of a given value.
	* @param rhs The value given to look up the ID
	* @return The ID of the input value
	* @invariant The ID, value pair must exist
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	const unsigned getID(const T& rhs) const;
	
	/**
	* @brief Get the value of a given ID.
	* @param id The ID given to look up the value
	* @return The value of the input ID
	* @invariant The ID, value pair must exist
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	T getValue(const unsigned id) const;

	/**
	* @brief Put the value at the end of this map. A new ID will be assigned to
			 to it.
	* @param rhs The value to append to this map
	* @return The ID of the input value
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	unsigned putValueAtEnd(const T& rhs);

	/**
	* @brief Clear this map
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	void clear();

	/**
	* @brief Equality comparison operator.
	* @param rhs The input map for this map to compare to
	* @return True iff this map has the same sequence of values as that of rhs
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	bool operator == (const ValueIDMap<T>& rhs) const;
	
	/**
	* @brief Inequality comparison operator.
	* @param rhs The input map for this map to compare to
	* @return True iff this map does not have the same sequence of values as 
			  that of rhs
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	bool operator != (const ValueIDMap<T>& rhs) const;

protected:
	/**
	* This variable stores the sequence of values in this map.
	*/
	ValueSequence _values;
	
	/**
	* This variable stores the map from values to IDs in this map.
	*/
	ValueMap _valueIDMap;

}; //class ValueIDMap
}  //namespace Utils
}  //namespace Cayuga

#endif //_VALUE_ID_MAP_H
