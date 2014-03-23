/**
 * @file FilterEdge.cpp
 * @date November 27, 2005
 * @brief Implementation file of FilterEdge class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "BasicDataStructure/Automaton/FilterEdge.h"
using namespace Cayuga::BasicDataStructure::Automaton;

FilterEdge::FilterEdge (EdgeID thisEdgeID) : Edge(thisEdgeID) {
}

FilterEdge::~FilterEdge() {
}

void FilterEdge::write(ostream& out) const {
	out<<"<FilterEdge>"<<endl;
	Edge::write(out);
	out<<"</FilterEdge>"<<endl;
}
