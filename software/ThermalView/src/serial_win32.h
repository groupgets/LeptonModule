#ifndef serial_h
#define serial_h

union filedescriptor
{
  int ifd;
  void *pfd;
};


void xSerialPortInitMinimal(char * port, long baud, union filedescriptor *fdp);
int xSerialGetChar(union filedescriptor *fd, signed char *pcRxedChar, long xBlockTime);
int xSerialPutChar( union filedescriptor *fd, signed char cOutChar, long xBlockTime );
int vSerialPutString(union filedescriptor *fd, const char * pcString, int xBlockTime);

void xSerialPortClose(union filedescriptor *fd);
int ser_setspeed(union filedescriptor *fd, long baud);


#endif /* serial_h */

