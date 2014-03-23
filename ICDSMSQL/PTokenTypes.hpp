#ifndef INC_PTokenTypes_hpp_
#define INC_PTokenTypes_hpp_

/* $ANTLR 2.7.6 (20060413): "CayugaQL.g" -> "PTokenTypes.hpp"$ */

#ifndef CUSTOM_API
# define CUSTOM_API
#endif

#ifdef __cplusplus
struct CUSTOM_API PTokenTypes {
#endif
	enum {
		EOF_ = 1,
		OUTPUT_NAME = 4,
		BODY = 5,
		SIMPLE_QUERY = 6,
		STREAM_NAME = 7,
		STREAM_NAME_ALIAS = 8,
		OUTPUT_CLAUSE = 9,
		OPEN_PAREN = 10,
		CLOSE_PAREN = 11,
		STRING = 12,
		OPEN_CUR_PAREN = 13,
		CLOSE_CUR_PAREN = 14,
		EQ = 15,
		NE = 16,
		LE = 17,
		OP_LT = 18,
		GT = 19,
		GE = 20,
		PLUS = 21,
		MINUS = 22,
		MUL = 23,
		DIV = 24,
		EXP = 25,
		MOD = 26,
		INT_FLOAT = 27,
		QUOTED_STRING = 28,
		COMMA = 29,
		DOT = 30,
		LITERAL_CREATE = 31,
		LITERAL_create = 32,
		LITERAL_UNION = 33,
		LITERAL_union = 34,
		LITERAL_FROM = 35,
		LITERAL_from = 36,
		LITERAL_SELECT = 37,
		LITERAL_select = 38,
		LITERAL_NEXT = 39,
		LITERAL_next = 40,
		LITERAL_ITERATE = 41,
		LITERAL_iterate = 42,
		LITERAL_ALPHA = 43,
		LITERAL_alpha = 44,
		LITERAL_SUM = 45,
		LITERAL_sum = 46,
		LITERAL_MAX = 47,
		LITERAL_max = 48,
		LITERAL_MIN = 49,
		LITERAL_min = 50,
		LITERAL_AVG = 51,
		LITERAL_avg = 52,
		LITERAL_COUNT = 53,
		LITERAL_count = 54,
		LITERAL_OUTPUT = 55,
		LITERAL_output = 56,
		LITERAL_AS = 57,
		LITERAL_as = 58,
		LITERAL_AND = 59,
		LITERAL_and = 60,
		LITERAL_OR = 61,
		LITERAL_or = 62,
		LETTER = 63,
		DIGIT = 64,
		INT = 65,
		FLOAT = 66,
		Exponent = 67,
		NEWLINE = 68,
		WS = 69,
		NULL_TREE_LOOKAHEAD = 3
	};
#ifdef __cplusplus
};
#endif
#endif /*INC_PTokenTypes_hpp_*/
