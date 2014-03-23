#-------------------------------------------------
#
# Project created by QtCreator 2012-05-07T13:46:36
#
#-------------------------------------------------

QT       -= core gui

TARGET = ICDSMSQL
TEMPLATE = lib
CONFIG += staticlib

DEFINES += ICDSMSQL_LIBRARY

SOURCES += \
    cayugaqlcompiler.cpp \
    CayugaQLAST.cpp \
    ASTSchema.cpp \
    ASTWalker/TypeChecker.cpp \
    ASTWalker/StreamExprTransformer.cpp \
    ASTWalker/SchemaMapBuilder.cpp \
    ASTWalker/NFAGen.cpp \
    ASTWalker/DNFPredGen.cpp \
    ASTWalker/CayugaQLASTWalker.cpp \
    ASTGen/P2.cpp \
    ASTGen/L2.cpp \
    predWindowGen.cpp \
    WindowGen.cpp

HEADERS += \
    PTokenTypes.hpp \
    CayugaQLCompiler.h \
    CayugaQLAST.h \
    ASTSchema.h \
    ASTWalker/TypeChecker.h \
    ASTWalker/StreamExprTransformer.h \
    ASTWalker/SchemaMapBuilder.h \
    ASTWalker/NFAGen.h \
    ASTWalker/DNFPredGen.h \
    ASTWalker/CayugaQLASTWalker.h \
    ASTGen/P2TokenTypes.hpp \
    ASTGen/P2.hpp \
    ASTGen/L2.hpp \
    WindowGen.h \
    predWindowGen.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE130330E
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = ICDSMSQL.dll
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

INCLUDEPATH += $$PWD/ASTGen
INCLUDEPATH += $$PWD/ASTWalker
DEPENDPATH += $$PWD/ASTGen
DEPENDPATH += $$PWD/ASTWalker
INCLUDEPATH += $$PWD/../ICDSMSCE/
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
    TARGET = ICDSMSQL_D
    LIBS += -L$$PWD/../LIB/Debug/ -lICDSMSCE_D
    LIBS += -L$$PWD/../LIB/Debug/ -lnetwork_D
    LIBS += -L$$PWD/../LIB/Debug/ -lcorelib_D
    LIBS += -L$$PWD/../LIB/Debug/ -lantlr_D
#    LIBS += -L$$PWD/../LIB/Debug/ -lxerces-c_D
}
else {
    DESTDIR = $$PWD/../LIB/Release/
    TARGET = ICDSMSQL
    LIBS += -L$$PWD/../LIB/Release/ -lICDSMSCE
    LIBS += -L$$PWD/../LIB/Release/ -lnetwork
    LIBS += -L$$PWD/../LIB/Release/ -lcorelib
    LIBS += -L$$PWD/../LIB/Release/ -lantlr
#    LIBS += -L$$PWD/../LIB/Release/ -lxerces-c
}

##win32: LIBS += -L$$PWD/../LIB/ -lICDSMSCE_D
#INCLUDEPATH += $$PWD/../LIB
#DEPENDPATH += $$PWD/../LIB
#win32: PRE_TARGETDEPS += $$PWD/../LIB/ICDSMSCE_D.lib
##win32: LIBS += -L$$PWD/../LIB/ -lSender_D
#INCLUDEPATH += $$PWD/../LIB
#DEPENDPATH += $$PWD/../LIB
#win32: PRE_TARGETDEPS += $$PWD/../LIB/Sender_D.lib
##win32: LIBS += -L$$PWD/../LIB/ -lReceiver_D
#INCLUDEPATH += $$PWD/../LIB
#DEPENDPATH += $$PWD/../LIB
#win32: PRE_TARGETDEPS += $$PWD/../LIB/Receiver_D.lib
#LIBS += -L$$PWD/../LIB/ -lantlr_D
##LIBS += -L$$PWD/../LIB/ -lICDSMSCE_D
#LIBS += -L$$PWD/../LIB/ -lcorelib_D
#LIBS += -L$$PWD/../LIB/ -lnetwork_D
#LIBS += -L$$PWD/../LIB/ -lxerces-c_D

