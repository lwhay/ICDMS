#-------------------------------------------------
#
# Project created by QtCreator 2012-05-08T01:46:48
#
#-------------------------------------------------

QT       -= core gui

TARGET = QLTEST
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    ../ICDSMSCE/main.cpp \
    ../ICDSMSCE/UDFs.cpp

#INCLUDEPATH += $$PWD/../EXTLIB/inc
#DEPENDPATH += $$PWD/../EXTLIB/inc
INCLUDEPATH += $$PWD/../BASELIB/
INCLUDEPATH += $$PWD/../ICDSMSCE/
INCLUDEPATH += $$PWD/../IEDSMSQL/

DEFINES += __MWERKS__
DEFINES += _CONSOLE
DEFINES += _CRT_SECURE_NO_DEPRECATE
CONFIG(debug) {
win32 {
    DESTDIR = $$PWD/../LIB/Debug/
    TARGET = QLRun_D
    LIBS += -L$$PWD/../LIB/Debug/ -lICDSMSCE_D
    LIBS += -L$$PWD/../LIB/Debug/ -lReceiver_D
    LIBS += -L$$PWD/../LIB/Debug/ -lSender_D
    LIBS += -L$$PWD/../LIB/Debug/ -lICDSMSQL_D
    LIBS += -L$$PWD/../LIB/Debug/ -lICDSMSCE_D
    LIBS += -L$$PWD/../LIB/Debug/ -lReceiver_D
    LIBS += -L$$PWD/../LIB/Debug/ -lSender_D
    LIBS += -L$$PWD/../LIB/Debug/ -lICDSMSQL_D
    LIBS += -L$$PWD/../LIB/Debug/ -lnetwork_D
    LIBS += -L$$PWD/../LIB/Debug/ -lcorelib_D
    LIBS += -L$$PWD/../LIB/Debug/ -lantlr_D
#    LIBS += -L$$PWD/../LIB/Debug/ -lxerces-c_D
    LIBS += -L$$PWD/../LIB/Debug/ -lxerces-c2_7_0
    LIBS += -lws2_32
} else {
    DESTDIR = $$PWD/../LIB/Debug/
    TARGET = QLRun_D
    LIBS += -L$$PWD/../LIB/Debug/ -lReceiver_D
    LIBS += -L$$PWD/../LIB/Debug/ -lSender_D
    LIBS += -L$$PWD/../LIB/Debug/ -lICDSMSQL_D
    LIBS += -L$$PWD/../LIB/Debug/ -lICDSMSCE_D
    LIBS += -L$$PWD/../LIB/Debug/ -lnetwork_D
    LIBS += -L$$PWD/../LIB/Debug/ -lcorelib_D
    LIBS += -L$$PWD/../LIB/Debug/ -lantlr_D
    LIBS += -L$$PWD/../LIB/Debug/ -lxerces-c2_7_0
#    LIBS += -lsocket
}
}
else {
    DESTDIR = $$PWD/../LIB/Release/
    TARGET = QLRun
    LIBS += -L$$PWD/../LIB/Release/ -lICDSMSCE
    LIBS += -L$$PWD/../LIB/Release/ -lReceiver
    LIBS += -L$$PWD/../LIB/Release/ -lSender
    LIBS += -L$$PWD/../LIB/Release/ -lICDSMSQL
    LIBS += -L$$PWD/../LIB/Release/ -lICDSMSCE
    LIBS += -L$$PWD/../LIB/Release/ -lcorelib
    LIBS += -L$$PWD/../LIB/Release/ -lnetwork
    LIBS += -L$$PWD/../LIB/Release/ -lantlr
#    LIBS += -L$$PWD/../LIB/Release/ -lxerces-c
    LIBS += -lws2_32
}
