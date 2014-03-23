INCLUDES = -I$(top_srcdir)/Cayuga/ -I$(top_srcdir)/Cayuga/inc/	\
-I$(top_srcdir)/Cayuga/inc/AutomatonCompiler					\
-I$(top_srcdir)/extlib/inc -I PredExprEval

noinst_LTLIBRARIES = libAutomatonCompiler.la

libAutomatonCompiler_la_SOURCES = AtomPredString.cpp ByteCodeUtil.cpp	\
ComplexAtomPredString.cpp ConjunctiveClauseString.cpp			\
convertConstToByteArray.cpp DurationPredString.cpp			\
DynAtomPredString.cpp EdgeBuilder.cpp EdgeString.cpp			\
FilterEdgeString.cpp ForwardEdgeString.cpp ForwardEdgeString.cpp	\
IntermediateNodeString.cpp NodeBuilder.cpp NodeString.cpp		\
NormalAtomPredString.cpp Predicates_DNFString.cpp			\
PredIndexSpecString.cpp QueryBuilder.cpp QueryString.cpp		\
RebindEdgeString.cpp SchemaMapString.cpp StaAtomPredString.cpp		\
StartNodeString.cpp PredExprEval/L.cpp PredExprEval/P.cpp		\
PredExprEval/AttrAST.cpp PredExprEval/BinaryCompArithOpAST.cpp		\
PredExprEval/DataTypeAST.cpp
