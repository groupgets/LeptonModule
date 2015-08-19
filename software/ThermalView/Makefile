
TARGET = ThermalView.exe

HEADERS = include/callbacks.h res/resource.h 
OBJS =	obj/winmain.o obj/resource.o obj/guicon.o obj/serial_win32.o
INCLUDE_DIRS = -I.\include -I.\res

WARNS = -Wall

CC = gcc
CFLAGS = -funsigned-char -fsigned-bitfields -fpack-struct -fshort-enums -mwindows  -mconsole  -lsetupapi -O2 -std=c99 -D UNICODE -D _UNICODE -D _WIN32_IE=0x0500 -D WINVER=0x500 ${WARNS} 
LDFLAGS = -s -lsetupapi -lcomctl32 -mconsole -mwindows -Wl,--subsystem,windows  #
RC = windres

all : $(TARGET)

$(TARGET) : ${OBJS}
	${CC} -o "$@" ${OBJS} ${LDFLAGS}

clean :
	rm -f $(TARGET) $(OBJS)

obj/%.o : src/%.c ${HEADERS}
	${CC} ${CFLAGS} ${INCLUDE_DIRS} -c $< -o $@

obj/resource.o : res/main_dialog.rc res/Application.manifest res/Application.ico res/resource.h
	${RC} -I.\include -I.\res -i $< -o $@
