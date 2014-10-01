#include <windows.h>
#include <stdio.h>
#include <commctrl.h>
#include <ctype.h>   /* for isprint */

#include "serial_win32.h"


#define W32SERBUFSIZE 1024

struct baud_mapping {
	long baud;
	DWORD speed;
};

struct baud_mapping baud_lookup_table [] = {
	{ 1200,   CBR_1200 },
	{ 2400,   CBR_2400 },
	{ 4800,   CBR_4800 },
	{ 9600,   CBR_9600 },
	{ 19200,  CBR_19200 },
	{ 38400,  CBR_38400 },
	{ 57600,  CBR_57600 },
	{ 115200, CBR_115200 },
	{ 256000, CBR_256000 },
	{ 0,      0 }                 /* Terminator. */
};

DWORD serial_baud_lookup(long baud)
{
	struct baud_mapping *map = baud_lookup_table;

	while (map->baud) {
		if (map->baud == baud)
			return map->speed;
		map++;
	}

	printf("\n\rUsing  non standard baud rate: %d\n\r",baud);
	return baud;
}

BOOL serial_w32SetTimeOut(HANDLE hComPort, DWORD timeout) // in ms
{
	COMMTIMEOUTS ctmo;
	ZeroMemory (&ctmo, sizeof(COMMTIMEOUTS));
	ctmo.ReadIntervalTimeout = timeout;
	ctmo.ReadTotalTimeoutMultiplier = timeout;
	ctmo.ReadTotalTimeoutConstant = timeout;

	return SetCommTimeouts(hComPort, &ctmo);
}

int ser_setspeed(union filedescriptor *fd, long baud)
{
	DCB dcb;
	HANDLE hComPort = (HANDLE)fd->pfd;

	ZeroMemory (&dcb, sizeof(DCB));
	dcb.DCBlength = sizeof(DCB);
	dcb.BaudRate = serial_baud_lookup (baud);
	
	dcb.fBinary = 1;
	dcb.fDtrControl = DTR_CONTROL_DISABLE;
	dcb.fRtsControl = RTS_CONTROL_DISABLE;
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;

	if (!SetCommState(hComPort, &dcb))
		return -1;

	return 0;
}

////////////////Freertos Versions, note not exactly the same, but good enough for testing//////////////////////////////

void xSerialPortInitMinimal(char * port, long baud, union filedescriptor *fdp)
{
	LPVOID lpMsgBuf;
	HANDLE hComPort=INVALID_HANDLE_VALUE;
	char fixedport[32];

	/*
	 * If the port is of the form "net:<host>:<port>", then
	 * handle it as a TCP connection to a terminal server.
	 *
	 * This is curently not implemented for Win32.
	 */
	if (strncmp(port, "net:", strlen("net:")) == 0) {
		//	fprintf(stderr,
		//		"%s: ser_open(): network connects are currently not"
		//		"implemented for Win32 environments\n",
		//		progname);
		exit(1);
	}

	/* if (hComPort!=INVALID_HANDLE_VALUE) 
	   fprintf(stderr, "%s: ser_open(): \"%s\" is already open\n",
	   progname, port);
	   */

	sprintf(fixedport,"\\\\.\%s", port);
	//hComPort = CreateFile(fixedport, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	hComPort = CreateFile(fixedport, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);

	if (hComPort == INVALID_HANDLE_VALUE) {
		FormatMessage( 
				FORMAT_MESSAGE_ALLOCATE_BUFFER | 
				FORMAT_MESSAGE_FROM_SYSTEM | 
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				GetLastError(),
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
				(LPTSTR) &lpMsgBuf,
				0,
				NULL);
		fprintf(stderr, "ser_open(): can't open device \"%s\": %s\n",fixedport, (char*)lpMsgBuf);
		LocalFree( lpMsgBuf );
		//exit(1);
	}

	if (!SetupComm(hComPort, W32SERBUFSIZE, W32SERBUFSIZE))
	{
		CloseHandle(hComPort);
		fprintf(stderr, "ser_open(): can't set buffers for \"%s\"\n", fixedport);
		//exit(1);
	}

	fdp->pfd = (void *)hComPort;
	if (ser_setspeed(fdp, baud) != 0)
	{
		CloseHandle(hComPort);
		fprintf(stderr, "ser_open(): can't set com-state for \"%s\"\n",fixedport);
		//exit(1);
	}

	if (!serial_w32SetTimeOut(hComPort,0))
	{
		CloseHandle(hComPort);
		fprintf(stderr, "ser_open(): can't set initial timeout for \"%s\"\n", fixedport);
		//exit(1);
	}
}

void xSerialPortClose(union filedescriptor *fd)
{
	HANDLE hComPort=(HANDLE)fd->pfd;
	if (hComPort != INVALID_HANDLE_VALUE)
		CloseHandle (hComPort);

	hComPort = INVALID_HANDLE_VALUE;
}

int xSerialGetChar(union filedescriptor *fd, signed char *pcRxedChar, long xBlockTime)
{
	unsigned char buf[1];
	DWORD read;

	HANDLE hComPort=(HANDLE)fd->pfd;

	if (hComPort == INVALID_HANDLE_VALUE) {
		//exit(1);
	}

	serial_w32SetTimeOut(hComPort, xBlockTime);

	if (!ReadFile(hComPort, buf, 1, &read, NULL)) {
		LPVOID lpMsgBuf;
		FormatMessage( 
				FORMAT_MESSAGE_ALLOCATE_BUFFER | 
				FORMAT_MESSAGE_FROM_SYSTEM | 
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				GetLastError(),
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
				(LPTSTR) &lpMsgBuf,
				0,
				NULL 	);
		fprintf(stderr, "ser_recv(): read error: %s\n",(char*)lpMsgBuf);
		LocalFree( lpMsgBuf );
		//exit(1);
	}

	*pcRxedChar = buf[0];

	return read;
}

int xSerialPutChar( union filedescriptor *fd, signed char cOutChar, long xBlockTime )
{
	DWORD written;
	//unsigned char * b = buf;
	unsigned char buf[1];

	buf[0] = cOutChar;

	HANDLE hComPort=(HANDLE)fd->pfd;

	if (hComPort == INVALID_HANDLE_VALUE) {
		fprintf(stderr, "ser_send(): port not open\n"); 
		//exit(1);
	}

	serial_w32SetTimeOut(hComPort,xBlockTime);

	if (!WriteFile (hComPort, buf, 1, &written, NULL)) {
		fprintf(stderr, "ser_send(): write error: %s\n", "sorry no info avail"); // TODO
		//exit(1);
	}

	return written;
}

int vSerialPutString(union filedescriptor *fd, const char * pcString, int xBlockTime)
{
	size_t len;
	unsigned char c='\0';
	DWORD written;

	len = strlen(pcString);

	HANDLE hComPort=(HANDLE)fd->pfd;

	if (hComPort == INVALID_HANDLE_VALUE) {
		//		fprintf(stderr, "%s: ser_send(): port not open\n",
		//            progname); 
		//exit(1);
	}

	if (!len)
		return 0;

	serial_w32SetTimeOut(hComPort,xBlockTime);

	if (!WriteFile (hComPort, pcString, len, &written, NULL)) {
		fprintf(stderr, "ser_send(): write error: %s\n", "sorry no info avail"); // TODO
		//exit(1);
	}

	return written;
}

