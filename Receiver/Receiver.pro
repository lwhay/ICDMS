#-------------------------------------------------
#
# Project created by QtCreator 2012-05-08T00:22:29
#
#-------------------------------------------------

QT       -= core gui

TARGET = Receiver
TEMPLATE = lib
CONFIG += staticlib

DEFINES += RECEIVER_LIBRARY

SOURCES += \
    NetworkReceiverWorker.cpp \
    ER/RelER.cpp \
    ER/NetworkRelER.cpp \
    ER/FileRelER.cpp \
    CR/NetworkCR.cpp \
    CR/makeCommand.cpp \
    CR/Command_AddSchema.cpp \
    CR/Command_AddQuery.cpp \
    CR/Command.cpp

HEADERS += receiver.h \
    ReceiverOptions.h \
    Receiver_global.h \
    NetworkReceiverWorkerThread.h \
    NetworkReceiverWorker.h \
    NetworkReceiverServerThread.h \
    Options/NetworkEROptions.h \
    Options/NetworkCROptions.h \
    Options/FileReceiverOptions.h \
    ER/RelER.h \
    ER/NetworkRelER.h \
    ER/FileRelER.h \
    ER/EROptions.h \
    CR/NetworkCR.h \
    CR/CommandQueue.h \
    CR/Command_AddSchema.h \
    CR/Command_AddQuery.h \
    CR/Command.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE2FFABDD
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = Receiver.dll
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
INCLUDEPATH += $$PWD/../ICDSMSCE/
INCLUDEPATH += $$PWD/../BASELIB/
DEFINES += __MWERKS__
DEFINES += _CRT_SECURE_NO_DEPRECATE
CONFIG += link_prl
CONFIG += create_prl
CONFIG(debug) {
    DESTDIR = $$PWD/../LIB/Debug/
    TARGET = Receiver_D
    LIBS += -L$$PWD/../LIB/Debug/ -lICDSMSCE_D
    LIBS += -L$$PWD/../LIB/Debug/ -lnetwork_D
    LIBS += -L$$PWD/../LIB/Debug/ -lcorelib_D
}
else {
    DESTDIR = $$PWD/../LIB/Release/
    TARGET = Receiver
    LIBS += -L$$PWD/../LIB/Release/ -lICDSMSCE
    LIBS += -L$$PWD/../LIB/Release/ -lnetwork
    LIBS += -L$$PWD/../LIB/Release/ -lcorelib
}
##LIBS += -L$$PWD/../LIB/ -lantlr_D
#LIBS += -L$$PWD/../LIB/ -lcorelib_D
#LIBS += -L$$PWD/../LIB/ -lnetwork_D
##LIBS += -L$$PWD/../LIB/ -lxerces-c_D
##LIBS += -L$$PWD/../EXTLIB/lib/win32/ -lcorelib
##LIBS += -L$$PWD/../EXTLIB/lib/win32/ -lcorelib_D
##PRE_TARGETDEPS += $$PWD/../EXTLIB/lib/win32/corelib_D.lib
##LIBS += -L$$PWD/../EXTLIB/lib/win32/ -lnetwork
##PRE_TARGETDEPS += $$PWD/../EXTLIB/lib/win32/network.lib
##LIBS += -L$$PWD/../EXTLIB/lib/win32/ -lnetwork_D
##PRE_TARGETDEPS += $$PWD/../EXTLIB/lib/win32/network_D.lib
