
TEMPLATE = app
QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 
DEPENDPATH += .
INCLUDEPATH += .

DESTDIR=.
MOC_DIR=gen_mocs
OBJECTS_DIR=gen_objs

HEADERS += *.h

SOURCES += *.cpp

unix:LIBS += -LleptonSDKEmb32PUB/Debug -lLEPTON_SDK

unix:QMAKE_CLEAN += -r $(OBJECTS_DIR) 

