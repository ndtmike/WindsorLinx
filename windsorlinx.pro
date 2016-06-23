#
QT += widgets serialport

TARGET = Windsorlinx
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    console.cpp \
    parser.cpp \
    datapacket.cpp \
    splash.cpp \
    instrumentdata.cpp

HEADERS += \
    mainwindow.h \
    console.h \
    parser.h \
    datapacket.h \
    splash.h \
    instrumentdata.h

FORMS += \
    mainwindow.ui \
    splash.ui

RESOURCES += \
    terminal.qrc

DISTFILES +=

win32{
RC_ICONS += ../icon/NDT_agg1.ico
VERSION = 3.0.0.0
QMAKE_TARGET_COMPANY = James Instruments Inc.
QMAKE_TARGET_PRODUCT = Windsorlinx
QMAKE_TARGET_DESCRIPTION = Upload Software for James Instruments Windsor Probe HP
QMAKE_TARGET_COPYRIGHT = @2016 James Instruments Inc.
}

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../qwt-6.1.2-MinGw/lib/ -lqwt
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../qwt-6.1.2-MinGw/lib/ -lqwtd

#INCLUDEPATH += $$PWD/../../../../../../qwt-6.1.2-MinGw/include
#DEPENDPATH += $$PWD/../../../../../../qwt-6.1.2-MinGw/include
