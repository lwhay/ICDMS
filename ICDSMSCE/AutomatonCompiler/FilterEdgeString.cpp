/**
 * @file FilterEdgeString.cpp
 * @date November 27, 2005
 * @brief Implementation file of FilterEdgeString class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "AutomatonCompiler/FilterEdgeString.h"
using namespace Cayuga::AutomatonCompiler;

FilterEdgeString::FilterEdgeString () : EdgeString() {
}

FilterEdgeString::~FilterEdgeString() {
}

Edge* FilterEdgeString::compile(Context& context) {
	Edge* pNewEdge = EdgeString::compile(context);
	return pNewEdge;
}

void FilterEdgeString::write(ostream& out) const {
	out<<"<FilterEdge>"<<endl;
	EdgeString::write(out);
	out<<"</FilterEdge>"<<endl;
}
