#include "Lepton_I2C.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <errno.h>
#include <cstdio>

//local variables
int _fd;
char _buffer[12];
bool _connected;

int lepton_connect(int address) {
	_fd = open("/dev/i2c-1", O_RDWR);

	if(_fd < 0) {
		return _fd;
	}
	int ret = ioctl(_fd, I2C_SLAVE, address);
	if(ret < 0) {
		return ret;
	}

	_connected = true;
	return 0;
}

void lepton_read_bytes(char *buffer, int length) {
	if(!_connected) {
		lepton_connect(0x2a);
	}
	read(_fd, buffer, length);
}

void lepton_write_bytes(char *buffer, int length) {
	if(!_connected) {
		lepton_connect(0x2a);
	}
	write(_fd, buffer, length);
}

void lepton_reboot() {
	_buffer[0] = 0x00;
	_buffer[1] = 0x06;
	_buffer[2] = 0x00;
	_buffer[3] = 0x00;
	lepton_write_bytes(_buffer, 4);
	
	_buffer[0] = 0x00;
	_buffer[1] = 0x04;
	_buffer[2] = 0x48;
	_buffer[3] = 0x42;
	lepton_write_bytes(_buffer, 4);
}
void lepton_perform_ffc() {
	_buffer[0] = 0x00;
	_buffer[1] = 0x06;
	_buffer[2] = 0x00;
	_buffer[3] = 0x00;
	lepton_write_bytes(_buffer, 4);

	_buffer[0] = 0x00;
	_buffer[1] = 0x04;
	_buffer[2] = 0x48;
	_buffer[3] = 0x46;
	lepton_write_bytes(_buffer, 4);
}
