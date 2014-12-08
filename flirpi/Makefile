CFLAGS = -Wall

all: leptbmp fblept leptgraypng

leptgraypng: leptgraypng.c leptsci.o
	gcc -o leptgraypng leptgraypng.c leptsci.o -lpng -Wall

leptbmp: leptbmp.c leptsci.o

fblmir: fblmir.c leptsci.o

fblept: fblept.c leptsci.o

leptsci.o: leptsci.c

clean:
	rm -f leptsci.o fblept leptbmp leptgraypng
