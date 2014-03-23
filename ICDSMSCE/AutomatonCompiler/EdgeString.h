/**
 * @file EdgeString.h
 * @date November 27, 2005
 * @brief Base class of all automaton edges read from AIR.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _EDGE_STRING_H
#define _EDGE_STRING_H

#include "BasicDataStructure/Automaton/EdgeID.h"
using Cayuga::BasicDataStructure::Automaton::EdgeID;

#include "BasicDataStructure/Automaton/EdgeType.h"
using Cayuga::BasicDataStructure::Automaton::EdgeType;

#include <iostream>
using namespace std;

namespace Cayuga {
//forward declaration
namespace BasicDataStructure {
namespace Automaton {
class Edge;
}
}

namespace AutomatonCompiler {
using Cayuga::BasicDataStructure::Automaton::Edge;
class Predicates_DNFString;
class Context;
Edge* makeEdge(EdgeType et, EdgeID nextForwardEdgeID);

/**
 * @brief An EdgeString object represents an automaton edge constructed from 
		  SAX events.	
 * @see	NodeString
 * @see QueryString
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 27, 2005
 * @version 1.0
 */
class EdgeString {
public:
	/**
	* @brief Default constructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	EdgeString();

	/**
	* @brief Virtual destructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual ~EdgeString();

	/**
	* @brief Compiles this edge into Cayuga engine, by translating
			 strings into integers with symbol tables, and so on.
	* @param context The compilation context
	* @return Pointer to the internalized edge.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual Edge* compile(Context& context) = 0;
	
	/**
	* @brief Output member variable information in XML format, aiming at 
			 reconstructing the XML piece in AIR which encodes this edge
	* @param out The output stream handle.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual void write(ostream& out = cout) const;

	/**
	* @brief Set the DNF edge predicate of this edge.
	* @param pDNF Pointer to the DNF edge predicate
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual void setDNF(Predicates_DNFString* pDNF);

	/**
	* @brief Return the DNF predicate on this edge. 
			Can return NULL if there is no predicate.
	* @return As above.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 17, 2007
	* @version 1.0
	*/
	Predicates_DNFString* getDNF() {
		return _pDNF;
	}

protected:
	/** 
	* This variable stores the pointer to DNF edge predicate,
	* a set of conjunctive predicate clauses.
	*/
	Predicates_DNFString* _pDNF;
}; //class EdgeString
}  //namespace AutomatonCompiler
}  //namespace Cayuga

#endif //_EDGE_STRING_H
