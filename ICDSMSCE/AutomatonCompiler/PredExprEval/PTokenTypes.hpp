#ifndef INC_PTokenTypes_hpp_
#define INC_PTokenTypes_hpp_

/* $ANTLR 2.7.6 (20060413): "CayugaPred.g" -> "PTokenTypes.hpp"$ */

#ifndef CUSTOM_API
# define CUSTOM_API
#endif

#ifdef __cplusplus
struct CUSTOM_API PTokenTypes {
#endif
	enum {
		EOF_ = 1,
		EQ = 4,
		NE = 5,
		LE = 6,
		OP_LT = 7,
		GE = 8,
		GT = 9,
		PLUS = 10,
		MINUS = 11,
		MUL = 12,
		DIV = 13,
		MOD = 14,
		EXP = 15,
		INT = 16,
		FLOAT = 17,
		STRING = 18,
		ATTR_OR_FUNC_NAME = 19,
		OPEN_PAREN = 20,
		CLOSE_PAREN = 21,
		DOT = 22,
		COMMA = 23,
		LETTER = 24,
		DIGIT = 25,
		Exponent = 26,
		Number = 27,
		NEWLINE = 28,
		WS = 29,
		NULL_TREE_LOOKAHEAD = 3
	};
#ifdef __cplusplus
};
#endif
#endif /*INC_PTokenTypes_hpp_*/
