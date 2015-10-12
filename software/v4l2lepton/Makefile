CFLAGS = -Wall

all: v4l2lepton

v4l2lepton: v4l2lepton.o
	c++ -o v4l2lepton v4l2lepton.cpp leptsci.o -lpthread -Wall

leptsci.o: leptsci.c

clean:
	rm -f v4l2lepton.o v4l2lepton
