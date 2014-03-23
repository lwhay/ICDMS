#-------------------------------------------------
#
# Project created by QtCreator 2012-05-08T01:30:31
#
#-------------------------------------------------

QT       -= core gui

TARGET = Sender
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    NetworkSenderWorker.cpp \
    NetworkSender.cpp

HEADERS += \
    Sender.h \
    NetworkSenderWorker.h \
    NetworkSenderServer.h \
    NetworkSender.h
unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

INCLUDEPATH += $$PWD/../
INCLUDEPATH += $$PWD/../ICDSMSCE/
INCLUDEPATH += $$PWD/../Sender/
DEPENDPATH += $$PWD/../EXTLIB/inc
DEPENDPATH += $$PWD/../Sender
DEPENDPATH += $$PWD/..
INCLUDEPATH += $$PWD/../BASELIB/
DEFINES += __MWERKS__
DEFINES += _CRT_SECURE_NO_DEPRECATE
CONFIG += link_prl
CONFIG += create_prl
CONFIG(debug) {
    DESTDIR = $$PWD/../LIB/Debug/
    TARGET = Sender_D
    LIBS += -L$$PWD/../LIB/Debug/ -lICDSMSCE_D
    LIBS += -L$$PWD/../LIB/Debug/ -lnetwork_D
    LIBS += -L$$PWD/../LIB/Debug/ -lcorelib_D
}
else {
    DESTDIR = $$PWD/../LIB/Release/
    TARGET = Sender
    LIBS += -L$$PWD/../LIB/Release/ -lICDSMSCE
    LIBS += -L$$PWD/../LIB/Release/ -lnetwork
    LIBS += -L$$PWD/../LIB/Release/ -lcorelib
}
##LIBS += -L$$PWD/../LIB/ -lantlr_D
#LIBS += -L$$PWD/../LIB/ -lcorelib_D
#LIBS += -L$$PWD/../LIB/ -lnetwork_D
##LIBS += -L$$PWD/../LIB/ -lxerces-c_D

#LIBS += -L$$PWD/../EXTLIB/lib/win32/ -lcorelib
#LIBS += -L$$PWD/../EXTLIB/lib/win32/ -lcorelib_D
#PRE_TARGETDEPS += $$PWD/../EXTLIB/lib/win32/corelib_D.lib
#LIBS += -L$$PWD/../EXTLIB/lib/win32/ -lnetwork
#PRE_TARGETDEPS += $$PWD/../EXTLIB/lib/win32/network.lib
#LIBS += -L$$PWD/../EXTLIB/lib/win32/ -lnetwork_D
#PRE_TARGETDEPS += $$PWD/../EXTLIB/lib/win32/network_D.lib


