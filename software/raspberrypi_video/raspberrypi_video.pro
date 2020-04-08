
TEMPLATE = app
QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = raspberrypi_video

RPI_LIBS = ../raspberrypi_libs
LEPTONSDK = leptonSDKEmb32PUB
LEPTONSPI = leptonSPI

PRE_TARGETDEPS += sdk spi
QMAKE_EXTRA_TARGETS += sdk sdkclean spi spiclean
sdk.commands = make -C $${RPI_LIBS}/$${LEPTONSDK}
sdkclean.commands = make -C $${RPI_LIBS}/$${LEPTONSDK} clean
spi.commands = make -C $${RPI_LIBS}/$${LEPTONSPI}
spiclean.commands = make -C $${RPI_LIBS}/$${LEPTONSPI} clean

DEPENDPATH += .
INCLUDEPATH += . $${RPI_LIBS} $${RPI_LIBS}/$${LEPTONSPI}

DESTDIR=.
OBJECTS_DIR=gen_objs
MOC_DIR=gen_mocs

HEADERS += *.h

SOURCES += *.cpp

unix:LIBS += -L$${RPI_LIBS}/$${LEPTONSDK}/Debug -L$${RPI_LIBS}/$${LEPTONSPI} -l$${LEPTONSPI} -lLEPTON_SDK

unix:QMAKE_CLEAN += -r $(OBJECTS_DIR) $${MOC_DIR}

