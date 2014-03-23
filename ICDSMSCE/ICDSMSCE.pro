#-------------------------------------------------
#
# Project created by QtCreator 2012-05-08T00:08:15
#
#-------------------------------------------------

QT       -= core gui

TARGET = ICDSMSCE
TEMPLATE = lib
CONFIG += staticlib

DEFINES += ICDSMSCE_LIBRARY

SOURCES += \
    SystemMsg.cpp \
    CayugaSystem.cpp \
    AutomatonCompiler/StartNodeString.cpp \
    AutomatonCompiler/StaAtomPredString.cpp \
    AutomatonCompiler/SchemaMapString.cpp \
    AutomatonCompiler/RebindEdgeString.cpp \
    AutomatonCompiler/QueryString.cpp \
    AutomatonCompiler/QueryBuilder.cpp \
    AutomatonCompiler/PredIndexSpecString.cpp \
    AutomatonCompiler/Predicates_DNFString.cpp \
    AutomatonCompiler/NormalAtomPredString.cpp \
    AutomatonCompiler/NodeString.cpp \
    AutomatonCompiler/NodeBuilder.cpp \
    AutomatonCompiler/IntermediateNodeString.cpp \
    AutomatonCompiler/ForwardEdgeString.cpp \
    AutomatonCompiler/FilterEdgeString.cpp \
    AutomatonCompiler/EdgeString.cpp \
    AutomatonCompiler/EdgeBuilder.cpp \
    AutomatonCompiler/DynAtomPredString.cpp \
    AutomatonCompiler/DurationPredString.cpp \
    AutomatonCompiler/convertConstToByteArray.cpp \
    AutomatonCompiler/ConjunctiveClauseString.cpp \
    AutomatonCompiler/ComplexAtomPredString.cpp \
    AutomatonCompiler/ByteCodeUtil.cpp \
    AutomatonCompiler/AtomPredString.cpp \
    AutomatonCompiler/PredExprEval/P.cpp \
    AutomatonCompiler/PredExprEval/L.cpp \
    AutomatonCompiler/PredExprEval/DataTypeAST.cpp \
    AutomatonCompiler/PredExprEval/BinaryCompArithOpAST.cpp \
    AutomatonCompiler/PredExprEval/AttrAST.cpp \
    BasicDataStructure/OutputStreamType.cpp \
    BasicDataStructure/CMessage.cpp \
    BasicDataStructure/Automaton/StartNode.cpp \
    BasicDataStructure/Automaton/RebindEdge.cpp \
    BasicDataStructure/Automaton/Query.cpp \
    BasicDataStructure/Automaton/Node.cpp \
    BasicDataStructure/Automaton/IntermediateNode.cpp \
    BasicDataStructure/Automaton/InstanceFinder.cpp \
    BasicDataStructure/Automaton/Instance.cpp \
    BasicDataStructure/Automaton/ForwardEdge.cpp \
    BasicDataStructure/Automaton/FilterEdge.cpp \
    BasicDataStructure/Automaton/Edge.cpp \
    BasicDataStructure/Event/EventFinder.cpp \
    BasicDataStructure/Event/Event.cpp \
    BasicDataStructure/Predicate/Predicates_DNF.cpp \
    BasicDataStructure/Predicate/ConjunctiveClause.cpp \
    Index/HashIndex.cpp \
    MemoryMgr/CY_IntStringImpl.cpp \
    MemoryMgr/CY_HeapMImpl.cpp \
    MemoryMgr/CY_HeapImpl.cpp \
    MemoryMgr/CY_ErrLogImpl.cpp \
    PriorityQueue/PriorityQueue.cpp \
    PriorityQueue/FiFoQueue.cpp \
    QueryProcessor/QueryProcessorImpl.cpp \
    QueryProcessor/QueryProcessor.cpp \
    QueryProcessor/Interpreter.cpp \
    QueryProcessor/FREvaluator.cpp \
    QueryProcessor/FilterEvaluator.cpp \
    SAXHandler/SIR_Handler.cpp \
    SAXHandler/SimpleHandler.cpp \
    SAXHandler/SAXHeader.cpp \
    SAXHandler/SAX2PrintHandlers.cpp \
    SAXHandler/ConfigMgr.cpp \
    SAXHandler/CAXParser.cpp \
    SAXHandler/AIR_Handler.cpp \
    SystemMgr/TraceMgr.cpp \
    SystemMgr/StreamSchemaMgr.cpp \
    SystemMgr/StreamSchema.cpp \
    SystemMgr/Options.cpp \
    SystemMgr/MeasureMgr.cpp \
    SystemMgr/FuncMgr.cpp \
    SystemMgr/DataTypeMgr.cpp \
    Utils/ValueIDMap.cpp \
    Utils/Timer.cpp \
    Utils/Split.cpp \
    Utils/RAssert.cpp \
    Utils/AttrMap.cpp \
    AutomatonCompiler/WindowString.cpp \
    AutomatonCompiler/WindowBuilder.cpp \
    AutomatonCompiler/predWindowString.cpp \
    AutomatonCompiler/predWindowBuilder.cpp \
    BasicDataStructure/Window/predWindow.cpp \
    BasicDataStructure/Window/inputWindow.cpp \
    BasicDataStructure/Window/outputWindow.cpp

HEADERS += \
    SystemMsg.h \
    CayugaSystem.h \
    arch-dep.h \
    AutomatonCompiler/StartNodeString.h \
    AutomatonCompiler/StaAtomPredString.h \
    AutomatonCompiler/SchemaMapString.h \
    AutomatonCompiler/RebindEdgeString.h \
    AutomatonCompiler/QueryString.h \
    AutomatonCompiler/QueryBuilder.h \
    AutomatonCompiler/PredIndexSpecString.h \
    AutomatonCompiler/Predicates_DNFString.h \
    AutomatonCompiler/NormalAtomPredString.h \
    AutomatonCompiler/NodeString.h \
    AutomatonCompiler/NodeBuilder.h \
    AutomatonCompiler/IntermediateNodeString.h \
    AutomatonCompiler/ForwardEdgeString.h \
    AutomatonCompiler/FilterEdgeString.h \
    AutomatonCompiler/EdgeString.h \
    AutomatonCompiler/EdgeBuilder.h \
    AutomatonCompiler/DynAtomPredString.h \
    AutomatonCompiler/DurationPredString.h \
    AutomatonCompiler/convertConstToByteArray.h \
    AutomatonCompiler/Context.h \
    AutomatonCompiler/ConjunctiveClauseString.h \
    AutomatonCompiler/ComplexAtomPredString.h \
    AutomatonCompiler/ByteCodeUtils.h \
    AutomatonCompiler/AtomPredString.h \
    AutomatonCompiler/PredExprEval/CayugaPred.g \
    AutomatonCompiler/PredExprEval/PTokenTypes.hpp \
    AutomatonCompiler/PredExprEval/P.hpp \
    AutomatonCompiler/PredExprEval/L.hpp \
    AutomatonCompiler/PredExprEval/DataTypeAST.h \
    AutomatonCompiler/PredExprEval/BinaryCompArithOpAST.h \
    AutomatonCompiler/PredExprEval/BasicAST.h \
    AutomatonCompiler/PredExprEval/AttrAST.h \
    BasicDataStructure/UnionDataType.h \
    BasicDataStructure/Timestamp.h \
    BasicDataStructure/StreamID.h \
    BasicDataStructure/OutputStreamType.h \
    BasicDataStructure/CMessage.h \
    BasicDataStructure/ByteCode.h \
    BasicDataStructure/Automaton/StartNode.h \
    BasicDataStructure/Automaton/RebindEdge.h \
    BasicDataStructure/Automaton/Query.h \
    BasicDataStructure/Automaton/NodeType.h \
    BasicDataStructure/Automaton/NodeID.h \
    BasicDataStructure/Automaton/node.h \
    BasicDataStructure/Automaton/IntermediateNode.h \
    BasicDataStructure/Automaton/InstanceFinder.h \
    BasicDataStructure/Automaton/Instance.h \
    BasicDataStructure/Automaton/ForwardEdge.h \
    BasicDataStructure/Automaton/FilterEdge.h \
    BasicDataStructure/Automaton/EdgeType.h \
    BasicDataStructure/Automaton/EdgeID.h \
    BasicDataStructure/Automaton/Edge.h \
    BasicDataStructure/Event/EventID.h \
    BasicDataStructure/Event/EventFinder.h \
    BasicDataStructure/Event/Event.h \
    BasicDataStructure/Predicate/Predicates_DNF.h \
    BasicDataStructure/Predicate/DurationStruct.h \
    BasicDataStructure/Predicate/ConjunctiveClause.h \
    BasicDataStructure/Predicate/AttributeID.h \
    Index/Index.h \
    Index/HashIndexDef.h \
    Index/HashIndex.h \
    MemoryMgr/CY_IntStringFace.h \
    MemoryMgr/CY_HeapPvt.h \
    MemoryMgr/CY_HeapFace.h \
    MemoryMgr/CY_GCFace.h \
    MemoryMgr/CY_ErrLog.h \
    PriorityQueue/PriorityQueue.h \
    PriorityQueue/GenericQueue.h \
    PriorityQueue/FiFoQueueDef.h \
    PriorityQueue/FiFoQueue.h \
    QueryProcessor/QueryProcessorImpl.h \
    QueryProcessor/QueryProcessor.h \
    QueryProcessor/Interpreter.h \
    QueryProcessor/IndexMap.h \
    QueryProcessor/FREvaluator.h \
    QueryProcessor/FilterEvaluator.h \
    SAXHandler/SIR_Handler.h \
    SAXHandler/SimpleHandler.h \
    SAXHandler/SAXEventType.h \
    SAXHandler/SAX2PrintHandlers.hpp \
    SAXHandler/SAX2Print.hpp \
    SAXHandler/DataSource.h \
    SAXHandler/ConfigMgr.h \
    SAXHandler/CAXParser.h \
    SAXHandler/AIR_Handler.h \
    SystemMgr/TraceMgr.h \
    SystemMgr/StreamSchemaMgr.h \
    SystemMgr/StreamSchema.h \
    SystemMgr/Options.h \
    SystemMgr/MeasureMgr.h \
    SystemMgr/FuncMgr.h \
    SystemMgr/DataTypeMgr.h \
    Utils/ValueIDMapDef.h \
    Utils/ValueIDMap.h \
    Utils/Timer.h \
    Utils/StringMap.h \
    Utils/split.h \
    Utils/Singleton.h \
    Utils/RAssert.h \
    Utils/AttrMap.h \
    AutomatonCompiler/WindowString.h \
    AutomatonCompiler/WindowBuilder.h \
    AutomatonCompiler/predWindowString.h \
    AutomatonCompiler/predWindowBuilder.h \
    BasicDataStructure/Window/WindowThread.h \
    BasicDataStructure/Window/WindowSchema.h \
    BasicDataStructure/Window/Window.h \
    BasicDataStructure/Window/predWindowSchema.h \
    BasicDataStructure/Window/predWindow.h \
    BasicDataStructure/Window/outputWindow.h \
    BasicDataStructure/Window/inputWindow.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE139F215
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = ICDSMSCE.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

#INCLUDEPATH += $$PWD/../EXTLIB/inc
#DEPENDPATH += $$PWD/../EXTLIB/inc
INCLUDEPATH += $$PWD/../BASELIB/
INCLUDEPATH += $$PWD/../BASELIB/antlr/
INCLUDEPATH += $$PWD/../BASELIB/corelib/
INCLUDEPATH += $$PWD/../BASELIB/xercesc/

DEFINES += __MWERKS__
DEFINES += _CRT_SECURE_NO_DEPRECATE
CONFIG += link_prl
CONFIG += create_prl

CONFIG(debug) {
    DESTDIR = $$PWD/../LIB/Debug/
    TARGET = ICDSMSCE_D
    LIBS += -L$$PWD/../LIB/Debug/ -lnetwork_D
    LIBS += -L$$PWD/../LIB/Debug/ -lcorelib_D
    LIBS += -L$$PWD/../LIB/Debug/ -lantlr_D
#    LIBS += -L$$PWD/../LIB/Debug/ -lxerces-c_D
}
else {
    DESTDIR = $$PWD/../LIB/Release/
    TARGET = ICDSMSCE
    LIBS += -L$$PWD/../LIB/Release/ -lnetwork
    LIBS += -L$$PWD/../LIB/Release/ -lcorelib
    LIBS += -L$$PWD/../LIB/Release/ -lantlr
#    LIBS += -L$$PWD/../LIB/Release/ -lxerces-c
}

#LIBS += -L$$PWD/../EXTLIB/lib/win32/ -lcorelib
#LIBS += -L$$PWD/../EXTLIB/lib/win32/ -lnetwork
#LIBS += -L$$PWD/../EXTLIB/lib/win32/ -lantlr
#LIBS += -L$$PWD/../EXTLIB/lib/win32/ -lxerces-c_2
#LIBS += -L$$PWD/../LIB/ -lantlr_D
#LIBS += -L$$PWD/../LIB/ -lcorelib_D
#LIBS += -L$$PWD/../LIB/ -lnetwork_D
#LIBS += -L$$PWD/../LIB/ -lxerces-c_D
#LIBS += -L$$PWD/../LIB/ -lReceiver_D
#LIBS += -L$$PWD/../LIB/ -lSender_D



























