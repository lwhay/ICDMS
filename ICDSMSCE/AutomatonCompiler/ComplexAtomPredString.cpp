/**
 * @file ComplexAtomPredString.cpp
 * @date April 15, 2006
 * @brief Implementation file of ComplexAtomPredString class.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "AutomatonCompiler/ComplexAtomPredString.h"	
#include "AutomatonCompiler/Context.h"
using namespace Cayuga::AutomatonCompiler;

#include "AutomatonCompiler/PredExprEval/L.hpp"
#include "AutomatonCompiler/PredExprEval/P.hpp"
#include "AutomatonCompiler/PredExprEval/BasicAST.h"

#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;


ComplexAtomPredString::ComplexAtomPredString(const string& expr) :
StaAtomPredString("", "", ""), _expr(expr), _bIndexedPred(false) {
}

ComplexAtomPredString::~ComplexAtomPredString() {
}

void ComplexAtomPredString::write(ostream& out) {
	out<<"<ComplexPred";
	out<<" Expr=\""<<_fixXML(_expr)<<"\"";
	out<<"/>"<<endl;
}

const string ComplexAtomPredString::_fixXML(const string& expr) const {
	string::size_type pos = expr.find("<");

	if (pos != expr.npos) {
		string prefix = expr.substr(0, pos);
		string postfix = expr.substr(pos+1);
		//expr.replace(pos, 1, "&lt;");

		//expr should contain at most one <		
		assert (string(prefix + "&lt;" + postfix).find("<") == string::npos);

		return prefix + "&lt;" + postfix;
	}
	
	return expr;
}

RefBinaryCompArithOpAST ComplexAtomPredString::_buildAST(
	const string& expr) const {
	try {
		istringstream in(expr);
		L lexer(in);
		P parser(lexer);

		ASTFactory my_factory("BasicAST", BasicAST::factory);
		parser.initializeASTFactory(my_factory);
		parser.setASTFactory(&my_factory);

		// invoke the parser...
		parser.startRule();
		//cout<<"parsing successful!"<<endl;

		//get AST
		//RefBasicAST ast = RefBasicAST(parser.getAST());
		return RefBinaryCompArithOpAST(parser.getAST());
	}
	catch (exception& e) {
		cerr << "exception: " << e.what() << endl;
		return NULL;
	}
}

char* ComplexAtomPredString::compile(Context& context) {
	try {
		//istringstream in(_expr);
		//L lexer(in);
		//P parser(lexer);

		//ASTFactory my_factory("BasicAST", BasicAST::factory);
		//parser.initializeASTFactory(my_factory);
		//parser.setASTFactory(&my_factory);

		//// invoke the parser...
		//parser.startRule();
		////cout<<"parsing successful!"<<endl;

		////get AST
		////RefBasicAST ast = RefBasicAST(parser.getAST());
		//RefBinaryCompArithOpAST ast = RefBinaryCompArithOpAST(parser.getAST());

		RefBinaryCompArithOpAST ast = _buildAST(_expr);

		//cout<<"print AST"<<endl;
		//if (ast)
		//	cout << ast->toStringList() << endl;
		//else
		//	cout << "null AST" << endl;

		_op = ast->getText();

		//cout<<typeid(*(ast->getFirstChild())).name()<<endl;
		//test whether this predicate is actually an indexable predicate
		//RefBasicAST firstChild = ast->getFirstChild();
		//if (typeid(*(firstChild)) == typeid(AttrAST)
		//	&& _op == "="
		//	&& typeid(*(firstChild->getNextSibling()))
		//		== typeid(DataTypeAST)) {
		//	_eventAttr = ast->getFirstChild()->getText();

		//	string str = firstChild->getNextSibling()->
		//		getText();
		//	string strWithoutQuotes = str.substr(1, str.size()-2);
		//	_rhs = strWithoutQuotes;

		//	_bIndexedPred = true;
		//}

		//cout<<"compiling stuff!"<<endl;
		vector<pair<char*, size_t> > program;
		ast->compile(context, program);

		//cout<<"compiling successful, output program!"<<endl;
		size_t szInstr = 0;
		for (size_t i = 0; i < program.size(); i++) {
			szInstr += program[i].second;
		}

		context._predInstructionLength = szInstr;
		char* instruction = new char[szInstr];
		size_t ip = 0;
		for (size_t i = 0; i < program.size(); i++) {
			memcpy(instruction+ip, program[i].first, program[i].second);
			ip += program[i].second;
			delete[] program[i].first;
		}


		return instruction;
	}
	catch (exception& e) {
		cerr << "exception: " << e.what() << endl;
		return NULL;
	}
}

bool ComplexAtomPredString::canBeIndexed() {
	return _bIndexedPred;
}


AtomPredString* ComplexAtomPredString::clone() {
	return new ComplexAtomPredString(_expr);
}

void ComplexAtomPredString::getReferredAttributes(
	set<string>& referredAttrs) const {
	RefBinaryCompArithOpAST ast = _buildAST(_expr);
	ast->getReferredAttributes(referredAttrs);
}

AtomPredString* ComplexAtomPredString::renameAttrRefInPredsToAttrIn(
	const SchemaMapString* schemaMap) {
	RefBinaryCompArithOpAST ast = _buildAST(_expr);
	ast->renameAttrRef(schemaMap);

	ostringstream ostr;
	ast->write(ostr);
	_expr = ostr.str();
	return NULL;
}
