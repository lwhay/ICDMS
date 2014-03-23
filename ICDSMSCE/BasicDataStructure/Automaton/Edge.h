/**
 * @file Edge.h
 * @date November 27, 2005
 * @brief Based class of automaton edges in Cayuga engine.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _EDGE_H
#define _EDGE_H

#include "BasicDataStructure/Automaton/EdgeID.h"
using Cayuga::BasicDataStructure::Automaton::EdgeID;

#include <vector>
#include <iostream>
#include <typeinfo>
using namespace std;


namespace Cayuga {

namespace QueryProcessor {
class FilterEvaluator;
class FilterIndex;
}

namespace BasicDataStructure {

namespace Predicate {
class Predicates_DNF;
}

namespace Automaton {

using Cayuga::BasicDataStructure::Predicate::Predicates_DNF;

/**
 * @brief An Edge object represents an automaton edge in Cayuga engine.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 27, 2005
 * @version 1.0
 */
class Edge {
	friend class Cayuga::QueryProcessor::FilterIndex;
public:
	/**
	* @brief Default constructor
	* @param thisEdgeID the edge ID assigned to this edge
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	Edge(EdgeID thisEdgeID);

	/**
	* @brief Virtual destructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual ~Edge() = 0;

	/**
	* @brief Output member variable information in XML format.
	* @param out The output stream handle.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual void write(ostream& out = cout) const;

	/**
	* @brief Get the type of this edge.
	* @return As above.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual const type_info& getEdgeType() const;

	/**
	* @brief Set the DNF predicate on this edge.
	* @param pDNF The DNF predicate to set to
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual void setDNF(Predicates_DNF* pDNF);

	/**
	* @brief Get the DNF predicate on this edge.
	* @return As above.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual Predicates_DNF* getDNF() const;

	/**
	* @brief Get the number of conjunctive clauses in this DNF predicate.
	* @return As above.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual size_t getNumClauses() const;

	/**
	* @brief Equality comparison operator.
	* @param rhs The input edge for this edge to compare to
	* @return True iff this edge has the same DNF predicate as that of rhs
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual bool operator == (const Edge& rhs) const;
	
	/**
	* @brief Inequality comparison operator.
	* @param rhs The input edge for this edge to compare to
	* @return True iff this edge does not have the same DNF predicate as 
	*		  that of rhs
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual bool operator != (const Edge& rhs) const;

	/**
	* @brief Get the ID of this edge
	* @return As above
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 16, 2006
	* @version 1.0
	*/
	EdgeID getID();

	/**
	* @brief Set the ID of this edge
	* @param thisEdgeID ID of this edge
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 16, 2006
	* @version 1.0
	*/
	void setID(EdgeID thisEdgeID);

protected:	/**
	* This variable stores the ID of this edge assigned by Cayuga.
	*/
	EdgeID _edgeID;


	/** 
	* This variable stores the pointer to the DNF predicate.
	*/
	Predicates_DNF* _pDNF;

	/**
	* This variable stores the number of edge IDs used by Cayuga.
	*/
	//static unsigned _edgeIDCnt;

}; //class Edge
}  //namespace Automaton
}  //namespace BasicDataStructure
}  //namespace Cayuga

#endif //_EDGE_H
