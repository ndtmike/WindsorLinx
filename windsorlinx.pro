#
QT += widgets serialport

TARGET = Windsorlinx
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    console.cpp \
    parser.cpp \
    splash.cpp \
    Inst_Data.cpp

HEADERS += \
    mainwindow.h \
    console.h \
    parser.h \
    splash.h \
    Inst_Data.h

FORMS += \
    mainwindow.ui \
    splash.ui

RESOURCES += \
    terminal.qrc

TRANSLATIONS += Internationalization_de.ts \
               Internationalization_es.ts

DISTFILES +=

win32{
RC_ICONS += ../icon/ndt_windsor.ico
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
