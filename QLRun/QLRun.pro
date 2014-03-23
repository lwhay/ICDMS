
QT       -= core gui

TARGET = QLTEST
CONFIG   += console
CONFIG   -= app_bundle
CONFIG  += static

TEMPLATE = app

SOURCES += \
    Main.cpp \
    ../ICDSMSCE/UDFs.cpp

INCLUDEPATH += $$PWD/../BASELIB/
INCLUDEPATH += $$PWD/../BASELIB/antlr/
INCLUDEPATH += $$PWD/../
INCLUDEPATH += $$PWD/../ICDSMSQL/
INCLUDEPATH += $$PWD/../ICDSMSCE/

DEFINES += __MWERKS__
DEFINES += _CRT_SECURE_NO_DEPRECATE
CONFIG += link_prl

CONFIG(debug) {
    DESTDIR = $$PWD/../LIB/Debug/
    TARGET = QLRun_D
#    QMAKE_LFLAGS += --start-group
    LIBS += -L$$PWD/../LIB/Debug/ -lICDSMSCE_D
    LIBS += -L$$PWD/../LIB/Debug/ -lReceiver_D
    LIBS += -L$$PWD/../LIB/Debug/ -lSender_D
    LIBS += -L$$PWD/../LIB/Debug/ -lICDSMSQL_D
    LIBS += -L$$PWD/../LIB/Debug/ -lICDSMSCE_D
    LIBS += -L$$PWD/../LIB/Debug/ -lReceiver_D
    LIBS += -L$$PWD/../LIB/Debug/ -lSender_D
    LIBS += -L$$PWD/../LIB/Debug/ -lICDSMSQL_D
    LIBS += -L$$PWD/../LIB/Debug/ -lICDSMSCE_D
    LIBS += -L$$PWD/../LIB/Debug/ -lnetwork_D
    LIBS += -L$$PWD/../LIB/Debug/ -lcorelib_D
    LIBS += -L$$PWD/../LIB/Debug/ -lantlr_D
    LIBS += -L$$PWD/../LIB/Debug/ -lxerces-c2_7_0
#    QMAKE_LFLAGS = -static -enable-stdcall-fixup -Wl,-enable-auto-import -Wl,-enable-runtime-pseudo-reloc
#    QMAKE_LFLAGS += -L$$PWD/../LIB/Debug/ -lReceiver_D -lSender_D -lICDSMSQL_D -lICDSMSCE_D -lnetwork_D -lcorelib_D -lantlr_D -lxerces-c2_7_0
#    QMAKE_LFLAGS += --end-group
win32 {
    LIBS += -lws2_32
} else {
#    QMAKE_LFLAGS += --start-group --end-group
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
}
