/**
 * @file RebindEdge.h
 * @date November 27, 2005
 * @brief Stores a rebind edge in Cayuga engine.

   RebindEdge class inherits FilterEdge, since it has some additional stuff
   other than what's in a FilterEdge. Specifically, it's the schema map program.

 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */


#ifndef _REBIND_EDGE_H
#define _REBIND_EDGE_H

#include "FilterEdge.h"

#include "AutomatonCompiler/SchemaMapString.h"
using Cayuga::AutomatonCompiler::SchemaMapString;

namespace Cayuga {
namespace QueryProcessor {
class FREvaluator;
}

namespace BasicDataStructure {
namespace Automaton {
/**
 * @brief A RebindEdge object represents a rebind edge in Cayuga engine.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 27, 2005
 * @version 1.0
 */
class RebindEdge : public FilterEdge {
	friend class Cayuga::QueryProcessor::FREvaluator;

public:
	/**
	* @brief Default constructor
	* @param thisEdgeID the edge ID assigned to this edge
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	RebindEdge (EdgeID thisEdgeID);

	/**
	* @brief Virtual destructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual ~RebindEdge();

	/**
	* @brief Output member variable information in XML format.
	* @param out The output stream handle.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual void write(ostream& out = cout) const;

	/**
	* @brief Set the schema map program of this edge. 
	* @param program The schema map program to set to.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual void setSchemaMapProgram(char* program);
	
	/**
	* @brief Set the next sword edge of his edge.

	A sword edge is an edge whose static part of the predicate is satisfied.

	* @param pEdge The next sword edge to set to
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual void setNextSwordEdge(RebindEdge* pEdge);

	/**
	* @brief Get the next sword edge of this edge. 
	* @return As above.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual RebindEdge* getNextSwordEdge();

	/**
	* @brief Set a particular sword clause of the DNF.

	A sword clause is a clause in DNF whose static part is satisfied.

	* @param clauseNo The clause number to set.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual void setSwordDNFClause(size_t clauseNo);

	/**
	* @brief Clear the sword edges reachable from this one.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 20, 2006
	* @version 1.0
	*/
	virtual void clearSwordEdges();

	/**
	* @brief Equality comparison operator.
	* @param rhs The input edge for this edge to compare to
	* @return True iff this edge has the same DNF/del predicate and schema map 
			  program as that of rhs
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual bool operator == (const Edge& rhs) const;
	
	
	/**
	* @brief Inequality comparison operator.
	* @param rhs The input edge for this edge to compare to
	* @return True iff this edge does not have the same DNF/del predicate and 
			  schema map program as that of rhs
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual bool operator != (const Edge& rhs) const;

	/**
	* @brief Sets the variable _hasDel
	* @param hasDel whether this edge has a del operator on it.
	* @author Varun Sharma (varun13687@yahoo.com)
	* @date June 19, 2006
	* @version 1.0
	*/
	void setDel(const bool& hasDel);

	/**
	* @brief returns the boolean value of _hasDel
	* @return returns true iff the edge has a del operator on it.
	* @author Varun Sharma (varun13687@yahoo.com)
	* @date June 19, 2006
	* @version 1.0
	*/
	bool hasDel() const;

	/**
	* @brief Set the schema map of this edge.
	* @param schemaMap The input schema map for the edge to store.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 3, 2007
	* @version 1.0
	*/
	void setSchemaMap(const SchemaMapString& schemaMap);

protected:
	/** 
	* This variable stores the schema map program of this edge.
	*/
	char* _schemaMapProgram;
	
	/**
	* This variable stores the schema map on this edge. It is to be used in
	* the write() member function only.
	*/
	SchemaMapString _schemaMap;

	/**
	* This variable stores whether the edge has a del attribute on it.
	*/
	bool _hasDel;

	/** 
	* This variable stores the pointer to the next sword edge.
	*/
	RebindEdge* _pNextSwordEdge;
}; //class RebindEdge
}  //namespace Automaton
}  //namespace BasicDataStructure
}  //namespace Cayuga

#endif //_REBIND_EDGE_H
