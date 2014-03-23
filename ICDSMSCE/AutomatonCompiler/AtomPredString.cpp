/**
 * @file AtomPredString.cpp
 * @date November 27, 2005
 * @brief Implementation file of AtomPredString.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "AutomatonCompiler/AtomPredString.h"
using namespace Cayuga::AutomatonCompiler;

using namespace Cayuga::BasicDataStructure::ByteCodeNS;

#include <cassert>
using namespace std;

AtomPredString::AtomPredString(const string& op) : _op(op) {
}

AtomPredString::~AtomPredString() {
}

void AtomPredString::write(ostream& out) {
	out<<"OP=\""<<_fixXML(_op)<<"\" ";
}

const string AtomPredString::_fixXML(const string& op) {
	if (op == "<") {
		return "&lt;";
	}
	else if (op == "<=") {
		return "&lt;=";
	}
	else {
		assert(op.find("<") == string::npos);
		return op;
	}
}

ByteCode AtomPredString::getOp() {
	ByteCode opType;
	if (_op == "=") {
		opType = EQ;
	}
	else if (_op == "!=") {
		opType = NE;
	}
	else if (_op == "<=") {
		opType = LE;
	}
	else if (_op == "<") {
		opType = LT;
	}
	else if (_op == ">=") {
		opType = GE;
	}
	else if (_op == ">") {
		opType = GT;
	}
	else {
		assert (false);
	}
	return opType;
}
