/* $ANTLR 2.7.7 (2006-11-01): "CayugaQL.g" -> "P2.cpp"$ */
#include "P2.hpp"
#include <antlr/NoViableAltException.hpp>
#include <antlr/SemanticException.hpp>
#include <antlr/ASTFactory.hpp>
#line 1 "CayugaQL.g"
#line 8 "P2.cpp"
P2::P2(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf, int k)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(tokenBuf,k)
{
}

P2::P2(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(tokenBuf,4)
{
}

P2::P2(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer, int k)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(lexer,k)
{
}

P2::P2(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(lexer,4)
{
}

P2::P2(const ANTLR_USE_NAMESPACE(antlr)ParserSharedInputState& state)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(state,4)
{
}

void P2::query() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST query_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		{
		switch ( LA(1)) {
		case LITERAL_SELECT:
		case LITERAL_select:
		{
			selectClause();
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			break;
		}
		case STRING:
		case LITERAL_window:
		case LITERAL_WINDOW:
		case OPEN_PAREN:
		case LITERAL_FILTER:
		case LITERAL_filter:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		{
		switch ( LA(1)) {
		case LITERAL_window:
		case LITERAL_WINDOW:
		{
			windowClause();
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			break;
		}
		case STRING:
		case OPEN_PAREN:
		case LITERAL_FILTER:
		case LITERAL_filter:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		streamExpr();
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		{
		switch ( LA(1)) {
		case LITERAL_PUBLISH:
		case LITERAL_publish:
		{
			resultClause();
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			break;
		}
		case CLOSE_PAREN:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		query_AST = RefCayugaQLAST(currentAST.root);
#line 60 "CayugaQL.g"
		query_AST = RefCayugaQLAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(QUERY,"QUERY")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(query_AST))));
#line 110 "P2.cpp"
		currentAST.root = query_AST;
		if ( query_AST!=RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
			query_AST->getFirstChild() != RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			  currentAST.child = query_AST->getFirstChild();
		else
			currentAST.child = query_AST;
		currentAST.advanceChildToEnd();
		query_AST = RefCayugaQLAST(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_0);
	}
	returnAST = query_AST;
}

void P2::selectClause() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST selectClause_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		select();
		selectList();
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		from();
		selectClause_AST = RefCayugaQLAST(currentAST.root);
#line 71 "CayugaQL.g"
		selectClause_AST = RefCayugaQLAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(SELECT_CLAUSE,"SELECT")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(selectClause_AST))));
#line 140 "P2.cpp"
		currentAST.root = selectClause_AST;
		if ( selectClause_AST!=RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
			selectClause_AST->getFirstChild() != RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			  currentAST.child = selectClause_AST->getFirstChild();
		else
			currentAST.child = selectClause_AST;
		currentAST.advanceChildToEnd();
		selectClause_AST = RefCayugaQLAST(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_1);
	}
	returnAST = selectClause_AST;
}

void P2::windowClause() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST windowClause_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		window();
		match(OPEN_CUR_PAREN);
		{
		switch ( LA(1)) {
		case LITERAL_TIME:
		case LITERAL_time:
		case LITERAL_COUNT:
		case LITERAL_count:
		{
			type();
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			break;
		}
		case COMMA:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		match(COMMA);
		{
		switch ( LA(1)) {
		case INT:
		case FLOAT:
		{
			length();
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			break;
		}
		case COMMA:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		match(COMMA);
		{
		switch ( LA(1)) {
		case INT:
		case FLOAT:
		{
			step();
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			break;
		}
		case COMMA:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		match(COMMA);
		{
		switch ( LA(1)) {
		case INT:
		case FLOAT:
		{
			timeStart();
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			break;
		}
		case CLOSE_CUR_PAREN:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		match(CLOSE_CUR_PAREN);
		windowClause_AST = RefCayugaQLAST(currentAST.root);
#line 106 "CayugaQL.g"
		windowClause_AST = RefCayugaQLAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(WINDOW_CLAUSE,"window")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(windowClause_AST))));
#line 250 "P2.cpp"
		currentAST.root = windowClause_AST;
		if ( windowClause_AST!=RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
			windowClause_AST->getFirstChild() != RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			  currentAST.child = windowClause_AST->getFirstChild();
		else
			currentAST.child = windowClause_AST;
		currentAST.advanceChildToEnd();
		windowClause_AST = RefCayugaQLAST(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_2);
	}
	returnAST = windowClause_AST;
}

void P2::streamExpr() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST streamExpr_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		unaryExpr();
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		{ // ( ... )*
		for (;;) {
			if (((LA(1) >= LITERAL_NEXT && LA(1) <= LITERAL_fold))) {
				binaryOp();
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				unaryExpr();
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			else {
				goto _loop17;
			}
			
		}
		_loop17:;
		} // ( ... )*
		streamExpr_AST = RefCayugaQLAST(currentAST.root);
#line 89 "CayugaQL.g"
		streamExpr_AST = RefCayugaQLAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(STREAM_EXPR,"")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(streamExpr_AST))));
#line 293 "P2.cpp"
		currentAST.root = streamExpr_AST;
		if ( streamExpr_AST!=RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
			streamExpr_AST->getFirstChild() != RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			  currentAST.child = streamExpr_AST->getFirstChild();
		else
			currentAST.child = streamExpr_AST;
		currentAST.advanceChildToEnd();
		streamExpr_AST = RefCayugaQLAST(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_3);
	}
	returnAST = streamExpr_AST;
}

void P2::resultClause() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST resultClause_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		publish();
		streamName();
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		resultClause_AST = RefCayugaQLAST(currentAST.root);
#line 67 "CayugaQL.g"
		resultClause_AST->setType(RESULT_CLAUSE);
#line 322 "P2.cpp"
		resultClause_AST = RefCayugaQLAST(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_0);
	}
	returnAST = resultClause_AST;
}

void P2::publish() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST publish_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		switch ( LA(1)) {
		case LITERAL_PUBLISH:
		{
			RefCayugaQLAST tmp6_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp6_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp6_AST));
			match(LITERAL_PUBLISH);
			publish_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		case LITERAL_publish:
		{
			RefCayugaQLAST tmp7_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp7_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp7_AST));
			match(LITERAL_publish);
			publish_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_4);
	}
	returnAST = publish_AST;
}

void P2::streamName() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST streamName_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		RefCayugaQLAST tmp8_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
		tmp8_AST = astFactory->create(LT(1));
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp8_AST));
		match(STRING);
		streamName_AST = RefCayugaQLAST(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_5);
	}
	returnAST = streamName_AST;
}

void P2::select() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST select_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		switch ( LA(1)) {
		case LITERAL_SELECT:
		{
			RefCayugaQLAST tmp9_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp9_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp9_AST));
			match(LITERAL_SELECT);
			select_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		case LITERAL_select:
		{
			RefCayugaQLAST tmp10_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp10_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp10_AST));
			match(LITERAL_select);
			select_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_6);
	}
	returnAST = select_AST;
}

void P2::selectList() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST selectList_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		termOrAny();
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == COMMA)) {
				match(COMMA);
				termOrAny();
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			else {
				goto _loop9;
			}
			
		}
		_loop9:;
		} // ( ... )*
		selectList_AST = RefCayugaQLAST(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_7);
	}
	returnAST = selectList_AST;
}

void P2::from() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST from_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		switch ( LA(1)) {
		case LITERAL_FROM:
		{
			RefCayugaQLAST tmp12_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp12_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp12_AST));
			match(LITERAL_FROM);
			from_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		case LITERAL_from:
		{
			RefCayugaQLAST tmp13_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp13_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp13_AST));
			match(LITERAL_from);
			from_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_1);
	}
	returnAST = from_AST;
}

void P2::termOrAny() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST termOrAny_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		switch ( LA(1)) {
		case MUL:
		{
			any();
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			termOrAny_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		case STRING:
		case OPEN_PAREN:
		case MINUS:
		case ATTR_VAR_MARKER:
		case QUOTED_STRING:
		case INT:
		case FLOAT:
		{
			termExpr();
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			termOrAny_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_8);
	}
	returnAST = termOrAny_AST;
}

void P2::any() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST any_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		RefCayugaQLAST tmp14_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
		tmp14_AST = astFactory->create(LT(1));
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp14_AST));
		match(MUL);
		any_AST = RefCayugaQLAST(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_8);
	}
	returnAST = any_AST;
}

void P2::termExpr() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST termExpr_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		term();
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		{
		switch ( LA(1)) {
		case LITERAL_AS:
		case LITERAL_as:
		{
			as();
			attrName();
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			break;
		}
		case COMMA:
		case CLOSE_CUR_PAREN:
		case LITERAL_FROM:
		case LITERAL_from:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		termExpr_AST = RefCayugaQLAST(currentAST.root);
#line 80 "CayugaQL.g"
		termExpr_AST = RefCayugaQLAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(TERM_EXPR,"")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(termExpr_AST))));
#line 589 "P2.cpp"
		currentAST.root = termExpr_AST;
		if ( termExpr_AST!=RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
			termExpr_AST->getFirstChild() != RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			  currentAST.child = termExpr_AST->getFirstChild();
		else
			currentAST.child = termExpr_AST;
		currentAST.advanceChildToEnd();
		termExpr_AST = RefCayugaQLAST(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_9);
	}
	returnAST = termExpr_AST;
}

void P2::term() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST term_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		m_term();
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == PLUS || LA(1) == MINUS)) {
				{
				switch ( LA(1)) {
				case PLUS:
				{
					RefCayugaQLAST tmp15_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
					tmp15_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp15_AST));
					match(PLUS);
					break;
				}
				case MINUS:
				{
					RefCayugaQLAST tmp16_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
					tmp16_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp16_AST));
					match(MINUS);
					break;
				}
				default:
				{
					throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
				}
				}
				}
				m_term();
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			else {
				goto _loop70;
			}
			
		}
		_loop70:;
		} // ( ... )*
		term_AST = RefCayugaQLAST(currentAST.root);
#line 174 "CayugaQL.g"
		term_AST = RefCayugaQLAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(PLUS_MINUS_EXPR,"")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(term_AST))));
#line 654 "P2.cpp"
		currentAST.root = term_AST;
		if ( term_AST!=RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
			term_AST->getFirstChild() != RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			  currentAST.child = term_AST->getFirstChild();
		else
			currentAST.child = term_AST;
		currentAST.advanceChildToEnd();
		term_AST = RefCayugaQLAST(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_10);
	}
	returnAST = term_AST;
}

void P2::as() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST as_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		switch ( LA(1)) {
		case LITERAL_AS:
		{
			RefCayugaQLAST tmp17_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp17_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp17_AST));
			match(LITERAL_AS);
			as_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		case LITERAL_as:
		{
			RefCayugaQLAST tmp18_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp18_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp18_AST));
			match(LITERAL_as);
			as_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_4);
	}
	returnAST = as_AST;
}

void P2::attrName() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST attrName_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		RefCayugaQLAST tmp19_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
		tmp19_AST = astFactory->create(LT(1));
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp19_AST));
		match(STRING);
		attrName_AST = RefCayugaQLAST(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_11);
	}
	returnAST = attrName_AST;
}

void P2::unaryExpr() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST unaryExpr_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		switch ( LA(1)) {
		case LITERAL_FILTER:
		case LITERAL_filter:
		{
			unaryOps();
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			primaryExpr();
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			unaryExpr_AST = RefCayugaQLAST(currentAST.root);
#line 97 "CayugaQL.g"
			unaryExpr_AST = RefCayugaQLAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(UNARY_EXPR,"")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(unaryExpr_AST))));
#line 745 "P2.cpp"
			currentAST.root = unaryExpr_AST;
			if ( unaryExpr_AST!=RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
				unaryExpr_AST->getFirstChild() != RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				  currentAST.child = unaryExpr_AST->getFirstChild();
			else
				currentAST.child = unaryExpr_AST;
			currentAST.advanceChildToEnd();
			unaryExpr_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		case STRING:
		case OPEN_PAREN:
		{
			primaryExpr();
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			unaryExpr_AST = RefCayugaQLAST(currentAST.root);
#line 98 "CayugaQL.g"
			unaryExpr_AST = RefCayugaQLAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(UNARY_EXPR,"")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(unaryExpr_AST))));
#line 764 "P2.cpp"
			currentAST.root = unaryExpr_AST;
			if ( unaryExpr_AST!=RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
				unaryExpr_AST->getFirstChild() != RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				  currentAST.child = unaryExpr_AST->getFirstChild();
			else
				currentAST.child = unaryExpr_AST;
			currentAST.advanceChildToEnd();
			unaryExpr_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_5);
	}
	returnAST = unaryExpr_AST;
}

void P2::binaryOp() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST binaryOp_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		switch ( LA(1)) {
		case LITERAL_NEXT:
		case LITERAL_next:
		{
			sequenceOp();
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			binaryOp_AST = RefCayugaQLAST(currentAST.root);
#line 134 "CayugaQL.g"
			binaryOp_AST = RefCayugaQLAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(SEQ_OP,"")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(binaryOp_AST))));
#line 803 "P2.cpp"
			currentAST.root = binaryOp_AST;
			if ( binaryOp_AST!=RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
				binaryOp_AST->getFirstChild() != RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				  currentAST.child = binaryOp_AST->getFirstChild();
			else
				currentAST.child = binaryOp_AST;
			currentAST.advanceChildToEnd();
			binaryOp_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		case LITERAL_FOLD:
		case LITERAL_fold:
		{
			muOp();
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			binaryOp_AST = RefCayugaQLAST(currentAST.root);
#line 135 "CayugaQL.g"
			binaryOp_AST = RefCayugaQLAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MU_OP,"")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(binaryOp_AST))));
#line 822 "P2.cpp"
			currentAST.root = binaryOp_AST;
			if ( binaryOp_AST!=RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
				binaryOp_AST->getFirstChild() != RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				  currentAST.child = binaryOp_AST->getFirstChild();
			else
				currentAST.child = binaryOp_AST;
			currentAST.advanceChildToEnd();
			binaryOp_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_2);
	}
	returnAST = binaryOp_AST;
}

void P2::unaryOps() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST unaryOps_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		{ // ( ... )+
		int _cnt32=0;
		for (;;) {
			if ((LA(1) == LITERAL_FILTER || LA(1) == LITERAL_filter)) {
				selectionOp();
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			else {
				if ( _cnt32>=1 ) { goto _loop32; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());}
			}
			
			_cnt32++;
		}
		_loop32:;
		}  // ( ... )+
		unaryOps_AST = RefCayugaQLAST(currentAST.root);
#line 122 "CayugaQL.g"
		unaryOps_AST = RefCayugaQLAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(UNARY_OPS,"")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(unaryOps_AST))));
#line 870 "P2.cpp"
		currentAST.root = unaryOps_AST;
		if ( unaryOps_AST!=RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
			unaryOps_AST->getFirstChild() != RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			  currentAST.child = unaryOps_AST->getFirstChild();
		else
			currentAST.child = unaryOps_AST;
		currentAST.advanceChildToEnd();
		unaryOps_AST = RefCayugaQLAST(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_12);
	}
	returnAST = unaryOps_AST;
}

void P2::primaryExpr() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST primaryExpr_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		switch ( LA(1)) {
		case STRING:
		{
			streamName();
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			primaryExpr_AST = RefCayugaQLAST(currentAST.root);
#line 118 "CayugaQL.g"
			primaryExpr_AST->setType(STREAM_NAME);
#line 901 "P2.cpp"
			primaryExpr_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		case OPEN_PAREN:
		{
			match(OPEN_PAREN);
			query();
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			match(CLOSE_PAREN);
			primaryExpr_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_5);
	}
	returnAST = primaryExpr_AST;
}

void P2::window() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST window_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		switch ( LA(1)) {
		case LITERAL_window:
		{
			RefCayugaQLAST tmp22_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp22_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp22_AST));
			match(LITERAL_window);
			window_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		case LITERAL_WINDOW:
		{
			RefCayugaQLAST tmp23_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp23_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp23_AST));
			match(LITERAL_WINDOW);
			window_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_13);
	}
	returnAST = window_AST;
}

void P2::type() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST type_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		switch ( LA(1)) {
		case LITERAL_TIME:
		{
			RefCayugaQLAST tmp24_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp24_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp24_AST));
			match(LITERAL_TIME);
			type_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		case LITERAL_time:
		{
			RefCayugaQLAST tmp25_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp25_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp25_AST));
			match(LITERAL_time);
			type_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		case LITERAL_COUNT:
		{
			RefCayugaQLAST tmp26_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp26_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp26_AST));
			match(LITERAL_COUNT);
			type_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		case LITERAL_count:
		{
			RefCayugaQLAST tmp27_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp27_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp27_AST));
			match(LITERAL_count);
			type_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_14);
	}
	returnAST = type_AST;
}

void P2::length() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST length_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		number();
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		length_AST = RefCayugaQLAST(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_14);
	}
	returnAST = length_AST;
}

void P2::step() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST step_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		number();
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		step_AST = RefCayugaQLAST(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_14);
	}
	returnAST = step_AST;
}

void P2::timeStart() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST timeStart_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		number();
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		timeStart_AST = RefCayugaQLAST(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_15);
	}
	returnAST = timeStart_AST;
}

void P2::number() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST number_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		switch ( LA(1)) {
		case INT:
		{
			RefCayugaQLAST tmp28_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp28_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp28_AST));
			match(INT);
			number_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		case FLOAT:
		{
			RefCayugaQLAST tmp29_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp29_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp29_AST));
			match(FLOAT);
			number_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_16);
	}
	returnAST = number_AST;
}

void P2::selectionOp() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST selectionOp_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		filter();
		match(OPEN_CUR_PAREN);
		predExpr();
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		match(CLOSE_CUR_PAREN);
		selectionOp_AST = RefCayugaQLAST(currentAST.root);
#line 125 "CayugaQL.g"
		selectionOp_AST = RefCayugaQLAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(FILTER_EXPR,"")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(selectionOp_AST))));
#line 1124 "P2.cpp"
		currentAST.root = selectionOp_AST;
		if ( selectionOp_AST!=RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
			selectionOp_AST->getFirstChild() != RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			  currentAST.child = selectionOp_AST->getFirstChild();
		else
			currentAST.child = selectionOp_AST;
		currentAST.advanceChildToEnd();
		selectionOp_AST = RefCayugaQLAST(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_2);
	}
	returnAST = selectionOp_AST;
}

void P2::filter() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST filter_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		switch ( LA(1)) {
		case LITERAL_FILTER:
		{
			RefCayugaQLAST tmp32_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp32_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp32_AST));
			match(LITERAL_FILTER);
			filter_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		case LITERAL_filter:
		{
			RefCayugaQLAST tmp33_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp33_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp33_AST));
			match(LITERAL_filter);
			filter_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_13);
	}
	returnAST = filter_AST;
}

void P2::predExpr() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST predExpr_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		andExpr();
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == LITERAL_OR || LA(1) == LITERAL_or)) {
				or_kw();
				andExpr();
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			else {
				goto _loop59;
			}
			
		}
		_loop59:;
		} // ( ... )*
		predExpr_AST = RefCayugaQLAST(currentAST.root);
#line 159 "CayugaQL.g"
		predExpr_AST = RefCayugaQLAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(PRED_EXPR,"")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(predExpr_AST))));
#line 1204 "P2.cpp"
		currentAST.root = predExpr_AST;
		if ( predExpr_AST!=RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
			predExpr_AST->getFirstChild() != RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			  currentAST.child = predExpr_AST->getFirstChild();
		else
			currentAST.child = predExpr_AST;
		currentAST.advanceChildToEnd();
		predExpr_AST = RefCayugaQLAST(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_17);
	}
	returnAST = predExpr_AST;
}

void P2::sequenceOp() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST sequenceOp_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		next();
		{
		switch ( LA(1)) {
		case LITERAL_predwindow:
		case LITERAL_PREDWINDOW:
		{
			predicateWindow();
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			break;
		}
		case OPEN_CUR_PAREN:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		match(OPEN_CUR_PAREN);
		{
		switch ( LA(1)) {
		case STRING:
		case OPEN_PAREN:
		case MINUS:
		case ATTR_VAR_MARKER:
		case QUOTED_STRING:
		case INT:
		case FLOAT:
		case LITERAL_DUR:
		case LITERAL_dur:
		{
			predExpr();
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			break;
		}
		case CLOSE_CUR_PAREN:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		match(CLOSE_CUR_PAREN);
		sequenceOp_AST = RefCayugaQLAST(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_2);
	}
	returnAST = sequenceOp_AST;
}

void P2::muOp() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST muOp_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		fold();
		{
		switch ( LA(1)) {
		case LITERAL_predwindow:
		case LITERAL_PREDWINDOW:
		{
			predicateWindow();
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			break;
		}
		case OPEN_CUR_PAREN:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		match(OPEN_CUR_PAREN);
		{
		switch ( LA(1)) {
		case COMMA:
		case STRING:
		case OPEN_PAREN:
		case MINUS:
		case ATTR_VAR_MARKER:
		case QUOTED_STRING:
		case INT:
		case FLOAT:
		case LITERAL_DUR:
		case LITERAL_dur:
		{
			muArgs();
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			break;
		}
		case CLOSE_CUR_PAREN:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		match(CLOSE_CUR_PAREN);
		muOp_AST = RefCayugaQLAST(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_2);
	}
	returnAST = muOp_AST;
}

void P2::next() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST next_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		switch ( LA(1)) {
		case LITERAL_NEXT:
		{
			RefCayugaQLAST tmp38_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp38_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp38_AST));
			match(LITERAL_NEXT);
			next_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		case LITERAL_next:
		{
			RefCayugaQLAST tmp39_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp39_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp39_AST));
			match(LITERAL_next);
			next_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_18);
	}
	returnAST = next_AST;
}

void P2::predicateWindow() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST predicateWindow_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		predwindow();
		match(OPEN_CUR_PAREN);
		{
		switch ( LA(1)) {
		case LITERAL_temporal:
		case LITERAL_TEMPORAL:
		{
			temporalClause();
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			break;
		}
		case COMMA:
		case CLOSE_CUR_PAREN:
		case LITERAL_COUNT:
		case LITERAL_count:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		{
		switch ( LA(1)) {
		case COMMA:
		{
			RefCayugaQLAST tmp41_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp41_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp41_AST));
			match(COMMA);
			break;
		}
		case CLOSE_CUR_PAREN:
		case LITERAL_COUNT:
		case LITERAL_count:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		{
		switch ( LA(1)) {
		case LITERAL_COUNT:
		case LITERAL_count:
		{
			countClause();
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			break;
		}
		case CLOSE_CUR_PAREN:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		match(CLOSE_CUR_PAREN);
		predicateWindow_AST = RefCayugaQLAST(currentAST.root);
#line 144 "CayugaQL.g"
		predicateWindow_AST = RefCayugaQLAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(PRED_WINDOW,"predwindow")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(predicateWindow_AST))));
#line 1461 "P2.cpp"
		currentAST.root = predicateWindow_AST;
		if ( predicateWindow_AST!=RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
			predicateWindow_AST->getFirstChild() != RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			  currentAST.child = predicateWindow_AST->getFirstChild();
		else
			currentAST.child = predicateWindow_AST;
		currentAST.advanceChildToEnd();
		predicateWindow_AST = RefCayugaQLAST(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_13);
	}
	returnAST = predicateWindow_AST;
}

void P2::fold() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST fold_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		switch ( LA(1)) {
		case LITERAL_FOLD:
		{
			RefCayugaQLAST tmp43_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp43_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp43_AST));
			match(LITERAL_FOLD);
			fold_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		case LITERAL_fold:
		{
			RefCayugaQLAST tmp44_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp44_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp44_AST));
			match(LITERAL_fold);
			fold_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_18);
	}
	returnAST = fold_AST;
}

void P2::muArgs() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST muArgs_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		{
		switch ( LA(1)) {
		case STRING:
		case OPEN_PAREN:
		case MINUS:
		case ATTR_VAR_MARKER:
		case QUOTED_STRING:
		case INT:
		case FLOAT:
		case LITERAL_DUR:
		case LITERAL_dur:
		{
			predExpr();
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			break;
		}
		case COMMA:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		RefCayugaQLAST tmp45_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
		tmp45_AST = astFactory->create(LT(1));
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp45_AST));
		match(COMMA);
		{
		switch ( LA(1)) {
		case STRING:
		case OPEN_PAREN:
		case MINUS:
		case ATTR_VAR_MARKER:
		case QUOTED_STRING:
		case INT:
		case FLOAT:
		case LITERAL_DUR:
		case LITERAL_dur:
		{
			predExpr();
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			break;
		}
		case COMMA:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		RefCayugaQLAST tmp46_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
		tmp46_AST = astFactory->create(LT(1));
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp46_AST));
		match(COMMA);
		{
		switch ( LA(1)) {
		case STRING:
		case OPEN_PAREN:
		case MINUS:
		case ATTR_VAR_MARKER:
		case QUOTED_STRING:
		case INT:
		case FLOAT:
		{
			muTerms();
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			break;
		}
		case CLOSE_CUR_PAREN:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		muArgs_AST = RefCayugaQLAST(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_15);
	}
	returnAST = muArgs_AST;
}

void P2::predwindow() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST predwindow_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		switch ( LA(1)) {
		case LITERAL_predwindow:
		{
			RefCayugaQLAST tmp47_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp47_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp47_AST));
			match(LITERAL_predwindow);
			predwindow_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		case LITERAL_PREDWINDOW:
		{
			RefCayugaQLAST tmp48_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp48_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp48_AST));
			match(LITERAL_PREDWINDOW);
			predwindow_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_13);
	}
	returnAST = predwindow_AST;
}

void P2::temporalClause() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST temporalClause_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		temporal();
		number();
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		time_unit();
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		temporalClause_AST = RefCayugaQLAST(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_19);
	}
	returnAST = temporalClause_AST;
}

void P2::countClause() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST countClause_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		count();
		number();
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		countClause_AST = RefCayugaQLAST(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_15);
	}
	returnAST = countClause_AST;
}

void P2::temporal() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST temporal_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		switch ( LA(1)) {
		case LITERAL_temporal:
		{
			RefCayugaQLAST tmp49_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp49_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp49_AST));
			match(LITERAL_temporal);
			temporal_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		case LITERAL_TEMPORAL:
		{
			RefCayugaQLAST tmp50_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp50_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp50_AST));
			match(LITERAL_TEMPORAL);
			temporal_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_20);
	}
	returnAST = temporal_AST;
}

void P2::time_unit() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST time_unit_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		switch ( LA(1)) {
		case LITERAL_YEAR:
		case LITERAL_year:
		{
			year();
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			time_unit_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		case LITERAL_MONTH:
		case LITERAL_month:
		{
			month();
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			time_unit_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		case LITERAL_DAY:
		case LITERAL_day:
		{
			day();
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			time_unit_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		case LITERAL_HOUR:
		case LITERAL_hour:
		{
			hour();
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			time_unit_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		case LITERAL_MIN:
		case LITERAL_min:
		{
			min();
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			time_unit_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		case LITERAL_SEC:
		case LITERAL_sec:
		{
			sec();
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			time_unit_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_21);
	}
	returnAST = time_unit_AST;
}

void P2::count() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST count_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		switch ( LA(1)) {
		case LITERAL_count:
		{
			RefCayugaQLAST tmp51_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp51_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp51_AST));
			match(LITERAL_count);
			count_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		case LITERAL_COUNT:
		{
			RefCayugaQLAST tmp52_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp52_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp52_AST));
			match(LITERAL_COUNT);
			count_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_20);
	}
	returnAST = count_AST;
}

void P2::muTerms() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST muTerms_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		termExpr();
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == COMMA)) {
				RefCayugaQLAST tmp53_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
				tmp53_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp53_AST));
				match(COMMA);
				termExpr();
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			else {
				goto _loop56;
			}
			
		}
		_loop56:;
		} // ( ... )*
		muTerms_AST = RefCayugaQLAST(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_15);
	}
	returnAST = muTerms_AST;
}

void P2::andExpr() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST andExpr_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		literal();
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == LITERAL_AND || LA(1) == LITERAL_and)) {
				and_kw();
				literal();
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			else {
				goto _loop62;
			}
			
		}
		_loop62:;
		} // ( ... )*
		andExpr_AST = RefCayugaQLAST(currentAST.root);
#line 162 "CayugaQL.g"
		andExpr_AST = RefCayugaQLAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(AND_EXPR,"")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(andExpr_AST))));
#line 1894 "P2.cpp"
		currentAST.root = andExpr_AST;
		if ( andExpr_AST!=RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
			andExpr_AST->getFirstChild() != RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			  currentAST.child = andExpr_AST->getFirstChild();
		else
			currentAST.child = andExpr_AST;
		currentAST.advanceChildToEnd();
		andExpr_AST = RefCayugaQLAST(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_22);
	}
	returnAST = andExpr_AST;
}

void P2::or_kw() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST or_kw_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		switch ( LA(1)) {
		case LITERAL_OR:
		{
			RefCayugaQLAST tmp54_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp54_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp54_AST));
			match(LITERAL_OR);
			or_kw_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		case LITERAL_or:
		{
			RefCayugaQLAST tmp55_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp55_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp55_AST));
			match(LITERAL_or);
			or_kw_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_23);
	}
	returnAST = or_kw_AST;
}

void P2::literal() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST literal_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		switch ( LA(1)) {
		case STRING:
		case OPEN_PAREN:
		case MINUS:
		case ATTR_VAR_MARKER:
		case QUOTED_STRING:
		case INT:
		case FLOAT:
		{
			term();
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			RefCayugaQLAST tmp56_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp56_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp56_AST));
			match(COMP_OP);
			term();
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			literal_AST = RefCayugaQLAST(currentAST.root);
#line 167 "CayugaQL.g"
			literal_AST = RefCayugaQLAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(ATOM_PRED,"")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(literal_AST))));
#line 1975 "P2.cpp"
			currentAST.root = literal_AST;
			if ( literal_AST!=RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
				literal_AST->getFirstChild() != RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				  currentAST.child = literal_AST->getFirstChild();
			else
				currentAST.child = literal_AST;
			currentAST.advanceChildToEnd();
			literal_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		case LITERAL_DUR:
		case LITERAL_dur:
		{
			dur();
			RefCayugaQLAST tmp57_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp57_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp57_AST));
			match(COMP_OP);
			number();
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			{ // ( ... )*
			for (;;) {
				if (((LA(1) >= LITERAL_YEAR && LA(1) <= LITERAL_sec))) {
					time_unit();
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
				else {
					goto _loop65;
				}
				
			}
			_loop65:;
			} // ( ... )*
			literal_AST = RefCayugaQLAST(currentAST.root);
#line 169 "CayugaQL.g"
			literal_AST = RefCayugaQLAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(DUR_PRED,"")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(literal_AST))));
#line 2012 "P2.cpp"
			currentAST.root = literal_AST;
			if ( literal_AST!=RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
				literal_AST->getFirstChild() != RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				  currentAST.child = literal_AST->getFirstChild();
			else
				currentAST.child = literal_AST;
			currentAST.advanceChildToEnd();
			literal_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_24);
	}
	returnAST = literal_AST;
}

void P2::and_kw() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST and_kw_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		switch ( LA(1)) {
		case LITERAL_AND:
		{
			RefCayugaQLAST tmp58_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp58_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp58_AST));
			match(LITERAL_AND);
			and_kw_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		case LITERAL_and:
		{
			RefCayugaQLAST tmp59_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp59_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp59_AST));
			match(LITERAL_and);
			and_kw_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_23);
	}
	returnAST = and_kw_AST;
}

void P2::dur() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST dur_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		switch ( LA(1)) {
		case LITERAL_DUR:
		{
			RefCayugaQLAST tmp60_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp60_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp60_AST));
			match(LITERAL_DUR);
			dur_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		case LITERAL_dur:
		{
			RefCayugaQLAST tmp61_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp61_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp61_AST));
			match(LITERAL_dur);
			dur_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_25);
	}
	returnAST = dur_AST;
}

void P2::year() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST year_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		switch ( LA(1)) {
		case LITERAL_YEAR:
		{
			RefCayugaQLAST tmp62_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp62_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp62_AST));
			match(LITERAL_YEAR);
			year_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		case LITERAL_year:
		{
			RefCayugaQLAST tmp63_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp63_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp63_AST));
			match(LITERAL_year);
			year_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_21);
	}
	returnAST = year_AST;
}

void P2::month() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST month_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		switch ( LA(1)) {
		case LITERAL_MONTH:
		{
			RefCayugaQLAST tmp64_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp64_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp64_AST));
			match(LITERAL_MONTH);
			month_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		case LITERAL_month:
		{
			RefCayugaQLAST tmp65_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp65_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp65_AST));
			match(LITERAL_month);
			month_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_21);
	}
	returnAST = month_AST;
}

void P2::day() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST day_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		switch ( LA(1)) {
		case LITERAL_DAY:
		{
			RefCayugaQLAST tmp66_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp66_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp66_AST));
			match(LITERAL_DAY);
			day_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		case LITERAL_day:
		{
			RefCayugaQLAST tmp67_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp67_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp67_AST));
			match(LITERAL_day);
			day_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_21);
	}
	returnAST = day_AST;
}

void P2::hour() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST hour_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		switch ( LA(1)) {
		case LITERAL_HOUR:
		{
			RefCayugaQLAST tmp68_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp68_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp68_AST));
			match(LITERAL_HOUR);
			hour_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		case LITERAL_hour:
		{
			RefCayugaQLAST tmp69_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp69_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp69_AST));
			match(LITERAL_hour);
			hour_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_21);
	}
	returnAST = hour_AST;
}

void P2::min() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST min_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		switch ( LA(1)) {
		case LITERAL_MIN:
		{
			RefCayugaQLAST tmp70_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp70_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp70_AST));
			match(LITERAL_MIN);
			min_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		case LITERAL_min:
		{
			RefCayugaQLAST tmp71_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp71_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp71_AST));
			match(LITERAL_min);
			min_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_21);
	}
	returnAST = min_AST;
}

void P2::sec() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST sec_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		switch ( LA(1)) {
		case LITERAL_SEC:
		{
			RefCayugaQLAST tmp72_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp72_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp72_AST));
			match(LITERAL_SEC);
			sec_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		case LITERAL_sec:
		{
			RefCayugaQLAST tmp73_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp73_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp73_AST));
			match(LITERAL_sec);
			sec_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_21);
	}
	returnAST = sec_AST;
}

void P2::m_term() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST m_term_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		factor();
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == MUL || LA(1) == DIV)) {
				{
				switch ( LA(1)) {
				case MUL:
				{
					RefCayugaQLAST tmp74_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
					tmp74_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp74_AST));
					match(MUL);
					break;
				}
				case DIV:
				{
					RefCayugaQLAST tmp75_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
					tmp75_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp75_AST));
					match(DIV);
					break;
				}
				default:
				{
					throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
				}
				}
				}
				factor();
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			else {
				goto _loop74;
			}
			
		}
		_loop74:;
		} // ( ... )*
		m_term_AST = RefCayugaQLAST(currentAST.root);
#line 178 "CayugaQL.g"
		m_term_AST = RefCayugaQLAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MUL_DIV_EXPR,"")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(m_term_AST))));
#line 2388 "P2.cpp"
		currentAST.root = m_term_AST;
		if ( m_term_AST!=RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
			m_term_AST->getFirstChild() != RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			  currentAST.child = m_term_AST->getFirstChild();
		else
			currentAST.child = m_term_AST;
		currentAST.advanceChildToEnd();
		m_term_AST = RefCayugaQLAST(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_26);
	}
	returnAST = m_term_AST;
}

void P2::factor() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST factor_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		factor2();
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == EXP || LA(1) == MOD)) {
				{
				switch ( LA(1)) {
				case EXP:
				{
					RefCayugaQLAST tmp76_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
					tmp76_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp76_AST));
					match(EXP);
					break;
				}
				case MOD:
				{
					RefCayugaQLAST tmp77_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
					tmp77_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp77_AST));
					match(MOD);
					break;
				}
				default:
				{
					throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
				}
				}
				}
				factor2();
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			else {
				goto _loop78;
			}
			
		}
		_loop78:;
		} // ( ... )*
		factor_AST = RefCayugaQLAST(currentAST.root);
#line 182 "CayugaQL.g"
		factor_AST = RefCayugaQLAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(EXP_MOD_EXPR,"")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(factor_AST))));
#line 2453 "P2.cpp"
		currentAST.root = factor_AST;
		if ( factor_AST!=RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
			factor_AST->getFirstChild() != RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			  currentAST.child = factor_AST->getFirstChild();
		else
			currentAST.child = factor_AST;
		currentAST.advanceChildToEnd();
		factor_AST = RefCayugaQLAST(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_27);
	}
	returnAST = factor_AST;
}

void P2::factor2() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST factor2_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		switch ( LA(1)) {
		case MINUS:
		{
			RefCayugaQLAST tmp78_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp78_AST = astFactory->create(LT(1));
			astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp78_AST));
			match(MINUS);
			factor2();
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			factor2_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		case QUOTED_STRING:
		case INT:
		case FLOAT:
		{
			const_lit();
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			factor2_AST = RefCayugaQLAST(currentAST.root);
#line 187 "CayugaQL.g"
			factor2_AST = RefCayugaQLAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(CONST_LIT_EXPR,"")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(factor2_AST))));
#line 2497 "P2.cpp"
			currentAST.root = factor2_AST;
			if ( factor2_AST!=RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
				factor2_AST->getFirstChild() != RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				  currentAST.child = factor2_AST->getFirstChild();
			else
				currentAST.child = factor2_AST;
			currentAST.advanceChildToEnd();
			factor2_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		case OPEN_PAREN:
		{
			match(OPEN_PAREN);
			predExpr();
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			match(CLOSE_PAREN);
			factor2_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		default:
			if ((LA(1) == STRING || LA(1) == ATTR_VAR_MARKER) && (_tokenSet_28.member(LA(2)))) {
				attrVar();
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				factor2_AST = RefCayugaQLAST(currentAST.root);
			}
			else if ((LA(1) == STRING) && (LA(2) == OPEN_PAREN)) {
				func();
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				match(OPEN_PAREN);
				{
				switch ( LA(1)) {
				case STRING:
				case OPEN_PAREN:
				case MINUS:
				case ATTR_VAR_MARKER:
				case QUOTED_STRING:
				case INT:
				case FLOAT:
				{
					term();
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
					break;
				}
				case COMMA:
				case CLOSE_PAREN:
				{
					break;
				}
				default:
				{
					throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
				}
				}
				}
				{ // ( ... )*
				for (;;) {
					if ((LA(1) == COMMA)) {
						RefCayugaQLAST tmp82_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
						tmp82_AST = astFactory->create(LT(1));
						astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp82_AST));
						match(COMMA);
						term();
						astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
					}
					else {
						goto _loop82;
					}
					
				}
				_loop82:;
				} // ( ... )*
				match(CLOSE_PAREN);
				factor2_AST = RefCayugaQLAST(currentAST.root);
#line 189 "CayugaQL.g"
				factor2_AST = RefCayugaQLAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(FUNC_EXPR,"")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(factor2_AST))));
#line 2573 "P2.cpp"
				currentAST.root = factor2_AST;
				if ( factor2_AST!=RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
					factor2_AST->getFirstChild() != RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
					  currentAST.child = factor2_AST->getFirstChild();
				else
					currentAST.child = factor2_AST;
				currentAST.advanceChildToEnd();
				factor2_AST = RefCayugaQLAST(currentAST.root);
			}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_11);
	}
	returnAST = factor2_AST;
}

void P2::const_lit() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST const_lit_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		switch ( LA(1)) {
		case INT:
		case FLOAT:
		{
			number();
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			const_lit_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		case QUOTED_STRING:
		{
			RefCayugaQLAST tmp84_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp84_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp84_AST));
			match(QUOTED_STRING);
			const_lit_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_11);
	}
	returnAST = const_lit_AST;
}

void P2::attrVar() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST attrVar_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		{
		switch ( LA(1)) {
		case ATTR_VAR_MARKER:
		{
			RefCayugaQLAST tmp85_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp85_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp85_AST));
			match(ATTR_VAR_MARKER);
			{
			switch ( LA(1)) {
			case INT:
			case FLOAT:
			{
				number();
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				break;
			}
			case DOT:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			match(DOT);
			break;
		}
		case STRING:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		attrName();
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		attrVar_AST = RefCayugaQLAST(currentAST.root);
#line 193 "CayugaQL.g"
		attrVar_AST = RefCayugaQLAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(ATTR_EXPR,"")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(attrVar_AST))));
#line 2683 "P2.cpp"
		currentAST.root = attrVar_AST;
		if ( attrVar_AST!=RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
			attrVar_AST->getFirstChild() != RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			  currentAST.child = attrVar_AST->getFirstChild();
		else
			currentAST.child = attrVar_AST;
		currentAST.advanceChildToEnd();
		attrVar_AST = RefCayugaQLAST(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_11);
	}
	returnAST = attrVar_AST;
}

void P2::func() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST func_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		RefCayugaQLAST tmp87_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
		tmp87_AST = astFactory->create(LT(1));
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp87_AST));
		match(STRING);
		func_AST = RefCayugaQLAST(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_29);
	}
	returnAST = func_AST;
}

void P2::union_kw() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST union_kw_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		switch ( LA(1)) {
		case LITERAL_UNION:
		{
			RefCayugaQLAST tmp88_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp88_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp88_AST));
			match(LITERAL_UNION);
			union_kw_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		case LITERAL_union:
		{
			RefCayugaQLAST tmp89_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp89_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp89_AST));
			match(LITERAL_union);
			union_kw_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_30);
	}
	returnAST = union_kw_AST;
}

void P2::not_kw() {
	returnAST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefCayugaQLAST not_kw_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		switch ( LA(1)) {
		case LITERAL_NOT:
		{
			RefCayugaQLAST tmp90_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp90_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp90_AST));
			match(LITERAL_NOT);
			not_kw_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		case LITERAL_not:
		{
			RefCayugaQLAST tmp91_AST = RefCayugaQLAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp91_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp91_AST));
			match(LITERAL_not);
			not_kw_AST = RefCayugaQLAST(currentAST.root);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_30);
	}
	returnAST = not_kw_AST;
}

void P2::initializeASTFactory( ANTLR_USE_NAMESPACE(antlr)ASTFactory& factory )
{
	factory.setMaxNodeType(104);
}
const char* P2::tokenNames[] = {
	"<0>",
	"EOF",
	"<2>",
	"NULL_TREE_LOOKAHEAD",
	"QUERY",
	"RESULT_CLAUSE",
	"SELECT_CLAUSE",
	"TERM_EXPR",
	"STREAM_EXPR",
	"WINDOW_CLAUSE",
	"STREAM_NAME",
	"UNARY_EXPR",
	"UNARY_OPS",
	"FILTER_EXPR",
	"SEQ_OP",
	"MU_OP",
	"PRED_WINDOW",
	"PRED_EXPR",
	"AND_EXPR",
	"ATOM_PRED",
	"DUR_PRED",
	"PLUS_MINUS_EXPR",
	"MUL_DIV_EXPR",
	"EXP_MOD_EXPR",
	"CONST_LIT_EXPR",
	"ATTR_EXPR",
	"ATTR_NAME",
	"STREAM_ATTR_NAME",
	"FUNC_EXPR",
	"COMMA",
	"STRING",
	"OPEN_CUR_PAREN",
	"CLOSE_CUR_PAREN",
	"\"TIME\"",
	"\"time\"",
	"\"COUNT\"",
	"\"count\"",
	"\"window\"",
	"\"WINDOW\"",
	"OPEN_PAREN",
	"CLOSE_PAREN",
	"\"temporal\"",
	"\"TEMPORAL\"",
	"\"predwindow\"",
	"\"PREDWINDOW\"",
	"COMP_OP",
	"PLUS",
	"MINUS",
	"MUL",
	"DIV",
	"EXP",
	"MOD",
	"ATTR_VAR_MARKER",
	"DOT",
	"QUOTED_STRING",
	"INT",
	"FLOAT",
	"\"PUBLISH\"",
	"\"publish\"",
	"\"UNION\"",
	"\"union\"",
	"\"SELECT\"",
	"\"select\"",
	"\"AS\"",
	"\"as\"",
	"\"FROM\"",
	"\"from\"",
	"\"NEXT\"",
	"\"next\"",
	"\"FOLD\"",
	"\"fold\"",
	"\"FILTER\"",
	"\"filter\"",
	"\"AND\"",
	"\"and\"",
	"\"OR\"",
	"\"or\"",
	"\"NOT\"",
	"\"not\"",
	"\"DUR\"",
	"\"dur\"",
	"\"YEAR\"",
	"\"year\"",
	"\"MONTH\"",
	"\"month\"",
	"\"DAY\"",
	"\"day\"",
	"\"HOUR\"",
	"\"hour\"",
	"\"MIN\"",
	"\"min\"",
	"\"SEC\"",
	"\"sec\"",
	"LE",
	"OP_LT",
	"GE",
	"GT",
	"EQ",
	"NE",
	"LETTER",
	"DIGIT",
	"INT_FLOAT",
	"Exponent",
	"NEWLINE",
	"WS",
	0
};

const unsigned long P2::_tokenSet_0_data_[] = { 0UL, 256UL, 0UL, 0UL };
// CLOSE_PAREN 
const ANTLR_USE_NAMESPACE(antlr)BitSet P2::_tokenSet_0(_tokenSet_0_data_,4);
const unsigned long P2::_tokenSet_1_data_[] = { 1073741824UL, 224UL, 384UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// STRING "window" "WINDOW" OPEN_PAREN "FILTER" "filter" 
const ANTLR_USE_NAMESPACE(antlr)BitSet P2::_tokenSet_1(_tokenSet_1_data_,8);
const unsigned long P2::_tokenSet_2_data_[] = { 1073741824UL, 128UL, 384UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// STRING OPEN_PAREN "FILTER" "filter" 
const ANTLR_USE_NAMESPACE(antlr)BitSet P2::_tokenSet_2(_tokenSet_2_data_,8);
const unsigned long P2::_tokenSet_3_data_[] = { 0UL, 100663552UL, 0UL, 0UL };
// CLOSE_PAREN "PUBLISH" "publish" 
const ANTLR_USE_NAMESPACE(antlr)BitSet P2::_tokenSet_3(_tokenSet_3_data_,4);
const unsigned long P2::_tokenSet_4_data_[] = { 1073741824UL, 0UL, 0UL, 0UL };
// STRING 
const ANTLR_USE_NAMESPACE(antlr)BitSet P2::_tokenSet_4(_tokenSet_4_data_,4);
const unsigned long P2::_tokenSet_5_data_[] = { 0UL, 100663552UL, 120UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// CLOSE_PAREN "PUBLISH" "publish" "NEXT" "next" "FOLD" "fold" 
const ANTLR_USE_NAMESPACE(antlr)BitSet P2::_tokenSet_5(_tokenSet_5_data_,8);
const unsigned long P2::_tokenSet_6_data_[] = { 1073741824UL, 30507136UL, 0UL, 0UL };
// STRING OPEN_PAREN MINUS MUL ATTR_VAR_MARKER QUOTED_STRING INT FLOAT 
const ANTLR_USE_NAMESPACE(antlr)BitSet P2::_tokenSet_6(_tokenSet_6_data_,4);
const unsigned long P2::_tokenSet_7_data_[] = { 0UL, 0UL, 6UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// "FROM" "from" 
const ANTLR_USE_NAMESPACE(antlr)BitSet P2::_tokenSet_7(_tokenSet_7_data_,8);
const unsigned long P2::_tokenSet_8_data_[] = { 536870912UL, 0UL, 6UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// COMMA "FROM" "from" 
const ANTLR_USE_NAMESPACE(antlr)BitSet P2::_tokenSet_8(_tokenSet_8_data_,8);
const unsigned long P2::_tokenSet_9_data_[] = { 536870912UL, 1UL, 6UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// COMMA CLOSE_CUR_PAREN "FROM" "from" 
const ANTLR_USE_NAMESPACE(antlr)BitSet P2::_tokenSet_9(_tokenSet_9_data_,8);
const unsigned long P2::_tokenSet_10_data_[] = { 536870912UL, 2147492097UL, 7687UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// COMMA CLOSE_CUR_PAREN CLOSE_PAREN COMP_OP "AS" "as" "FROM" "from" "AND" 
// "and" "OR" "or" 
const ANTLR_USE_NAMESPACE(antlr)BitSet P2::_tokenSet_10(_tokenSet_10_data_,8);
const unsigned long P2::_tokenSet_11_data_[] = { 536870912UL, 2148524289UL, 7687UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// COMMA CLOSE_CUR_PAREN CLOSE_PAREN COMP_OP PLUS MINUS MUL DIV EXP MOD 
// "AS" "as" "FROM" "from" "AND" "and" "OR" "or" 
const ANTLR_USE_NAMESPACE(antlr)BitSet P2::_tokenSet_11(_tokenSet_11_data_,8);
const unsigned long P2::_tokenSet_12_data_[] = { 1073741824UL, 128UL, 0UL, 0UL };
// STRING OPEN_PAREN 
const ANTLR_USE_NAMESPACE(antlr)BitSet P2::_tokenSet_12(_tokenSet_12_data_,4);
const unsigned long P2::_tokenSet_13_data_[] = { 2147483648UL, 0UL, 0UL, 0UL };
// OPEN_CUR_PAREN 
const ANTLR_USE_NAMESPACE(antlr)BitSet P2::_tokenSet_13(_tokenSet_13_data_,4);
const unsigned long P2::_tokenSet_14_data_[] = { 536870912UL, 0UL, 0UL, 0UL };
// COMMA 
const ANTLR_USE_NAMESPACE(antlr)BitSet P2::_tokenSet_14(_tokenSet_14_data_,4);
const unsigned long P2::_tokenSet_15_data_[] = { 0UL, 1UL, 0UL, 0UL };
// CLOSE_CUR_PAREN 
const ANTLR_USE_NAMESPACE(antlr)BitSet P2::_tokenSet_15(_tokenSet_15_data_,4);
const unsigned long P2::_tokenSet_16_data_[] = { 536870912UL, 2150621441UL, 536747527UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// COMMA CLOSE_CUR_PAREN CLOSE_PAREN COMP_OP PLUS MINUS MUL DIV EXP MOD 
// DOT "AS" "as" "FROM" "from" "AND" "and" "OR" "or" "YEAR" "year" "MONTH" 
// "month" "DAY" "day" "HOUR" "hour" "MIN" "min" "SEC" "sec" 
const ANTLR_USE_NAMESPACE(antlr)BitSet P2::_tokenSet_16(_tokenSet_16_data_,8);
const unsigned long P2::_tokenSet_17_data_[] = { 536870912UL, 257UL, 0UL, 0UL };
// COMMA CLOSE_CUR_PAREN CLOSE_PAREN 
const ANTLR_USE_NAMESPACE(antlr)BitSet P2::_tokenSet_17(_tokenSet_17_data_,4);
const unsigned long P2::_tokenSet_18_data_[] = { 2147483648UL, 6144UL, 0UL, 0UL };
// OPEN_CUR_PAREN "predwindow" "PREDWINDOW" 
const ANTLR_USE_NAMESPACE(antlr)BitSet P2::_tokenSet_18(_tokenSet_18_data_,4);
const unsigned long P2::_tokenSet_19_data_[] = { 536870912UL, 25UL, 0UL, 0UL };
// COMMA CLOSE_CUR_PAREN "COUNT" "count" 
const ANTLR_USE_NAMESPACE(antlr)BitSet P2::_tokenSet_19(_tokenSet_19_data_,4);
const unsigned long P2::_tokenSet_20_data_[] = { 0UL, 25165824UL, 0UL, 0UL };
// INT FLOAT 
const ANTLR_USE_NAMESPACE(antlr)BitSet P2::_tokenSet_20(_tokenSet_20_data_,4);
const unsigned long P2::_tokenSet_21_data_[] = { 536870912UL, 281UL, 536747520UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// COMMA CLOSE_CUR_PAREN "COUNT" "count" CLOSE_PAREN "AND" "and" "OR" "or" 
// "YEAR" "year" "MONTH" "month" "DAY" "day" "HOUR" "hour" "MIN" "min" 
// "SEC" "sec" 
const ANTLR_USE_NAMESPACE(antlr)BitSet P2::_tokenSet_21(_tokenSet_21_data_,8);
const unsigned long P2::_tokenSet_22_data_[] = { 536870912UL, 257UL, 6144UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// COMMA CLOSE_CUR_PAREN CLOSE_PAREN "OR" "or" 
const ANTLR_USE_NAMESPACE(antlr)BitSet P2::_tokenSet_22(_tokenSet_22_data_,8);
const unsigned long P2::_tokenSet_23_data_[] = { 1073741824UL, 30441600UL, 98304UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// STRING OPEN_PAREN MINUS ATTR_VAR_MARKER QUOTED_STRING INT FLOAT "DUR" 
// "dur" 
const ANTLR_USE_NAMESPACE(antlr)BitSet P2::_tokenSet_23(_tokenSet_23_data_,8);
const unsigned long P2::_tokenSet_24_data_[] = { 536870912UL, 257UL, 7680UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// COMMA CLOSE_CUR_PAREN CLOSE_PAREN "AND" "and" "OR" "or" 
const ANTLR_USE_NAMESPACE(antlr)BitSet P2::_tokenSet_24(_tokenSet_24_data_,8);
const unsigned long P2::_tokenSet_25_data_[] = { 0UL, 8192UL, 0UL, 0UL };
// COMP_OP 
const ANTLR_USE_NAMESPACE(antlr)BitSet P2::_tokenSet_25(_tokenSet_25_data_,4);
const unsigned long P2::_tokenSet_26_data_[] = { 536870912UL, 2147541249UL, 7687UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// COMMA CLOSE_CUR_PAREN CLOSE_PAREN COMP_OP PLUS MINUS "AS" "as" "FROM" 
// "from" "AND" "and" "OR" "or" 
const ANTLR_USE_NAMESPACE(antlr)BitSet P2::_tokenSet_26(_tokenSet_26_data_,8);
const unsigned long P2::_tokenSet_27_data_[] = { 536870912UL, 2147737857UL, 7687UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// COMMA CLOSE_CUR_PAREN CLOSE_PAREN COMP_OP PLUS MINUS MUL DIV "AS" "as" 
// "FROM" "from" "AND" "and" "OR" "or" 
const ANTLR_USE_NAMESPACE(antlr)BitSet P2::_tokenSet_27(_tokenSet_27_data_,8);
const unsigned long P2::_tokenSet_28_data_[] = { 536870912UL, 2175787265UL, 7687UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// COMMA CLOSE_CUR_PAREN CLOSE_PAREN COMP_OP PLUS MINUS MUL DIV EXP MOD 
// DOT INT FLOAT "AS" "as" "FROM" "from" "AND" "and" "OR" "or" 
const ANTLR_USE_NAMESPACE(antlr)BitSet P2::_tokenSet_28(_tokenSet_28_data_,8);
const unsigned long P2::_tokenSet_29_data_[] = { 0UL, 128UL, 0UL, 0UL };
// OPEN_PAREN 
const ANTLR_USE_NAMESPACE(antlr)BitSet P2::_tokenSet_29(_tokenSet_29_data_,4);
const unsigned long P2::_tokenSet_30_data_[] = { 2UL, 0UL, 0UL, 0UL };
// EOF 
const ANTLR_USE_NAMESPACE(antlr)BitSet P2::_tokenSet_30(_tokenSet_30_data_,4);


