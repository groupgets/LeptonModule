
TEMPLATE = app
#QT += core gui
#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = v4l2lepton

RPI_LIBS = ../raspberrypi_libs
LEPTONSDK = leptonSDKEmb32PUB

PRE_TARGETDEPS += sdk
QMAKE_EXTRA_TARGETS += sdk sdkclean
sdk.commands = make -C $${RPI_LIBS}/$${LEPTONSDK}
sdkclean.commands = make -C $${RPI_LIBS}/$${LEPTONSDK} clean

DEPENDPATH += .
INCLUDEPATH += . $${RPI_LIBS} $${RPI_LIBS}/leptonSPI

DESTDIR=.
OBJECTS_DIR=gen_objs
MOC_DIR=gen_mocs

HEADERS += *.h

SOURCES += *.cpp

unix:LIBS += -L$${RPI_LIBS}/$${LEPTONSDK}/Debug -L$${RPI_LIBS}/leptonSPI -lleptonSPI -lLEPTON_SDK

unix:QMAKE_CLEAN += -r $(OBJECTS_DIR) $${MOC_DIR}

