#
CC      = gcc
CXX     = g++
LINK    = ar

#
CFLAGS        = -pipe -O2 -Wall -W -D_REENTRANT $(DEFINES) -I. -I..
CXXFLAGS      = -pipe -O2 -Wall -W -D_REENTRANT $(DEFINES) -I. -I..

#
TARGET  = libleptonSPI.a
OBJECTS = LeptonAction.o Lepton.o Lepton_I2C.o Palettes.o SPI.o

%: %.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $<

$(TARGET): $(OBJECTS)
	$(LINK) rcs $(TARGET) $(OBJECTS)

all: $(TARGET)

$(TARTGET): %.o

clean:
	rm *.o *.a

