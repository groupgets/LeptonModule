#include "SPI.h"

char *spi_dev = DEFAULT_SPI_DEVICE;
int spi_fd = -1;

unsigned char spi_mode = SPI_MODE_3;
unsigned char spi_bitsPerWord = 8;
unsigned int spi_speed = 20 * 1000 * 1000;	// 20MHz

void SpiSetDevice (char *useSpiDevice)
{
    spi_dev = useSpiDevice;
}

void SpiSetSpeed (unsigned int useSpiSpeed)
{
    spi_speed = useSpiSpeed;
}

int SpiOpenPort ()
{
	int status_value = -1;

	//----- SET SPI MODE -----
	//SPI_MODE_0 (0,0)  CPOL=0 (Clock Idle low level), CPHA=0 (SDO transmit/change edge active to idle)
	//SPI_MODE_1 (0,1)  CPOL=0 (Clock Idle low level), CPHA=1 (SDO transmit/change edge idle to active)
	//SPI_MODE_2 (1,0)  CPOL=1 (Clock Idle high level), CPHA=0 (SDO transmit/change edge active to idle)
	//SPI_MODE_3 (1,1)  CPOL=1 (Clock Idle high level), CPHA=1 (SDO transmit/change edge idle to active)
	spi_mode = SPI_MODE_3;

	//----- SET BITS PER WORD -----
	spi_bitsPerWord = 8;

	//
	spi_fd = open(spi_dev, O_RDWR);
	if (spi_fd < 0)
	{
		perror("Error - Could not open SPI device");
		exit(1);
	}

	status_value = ioctl(spi_fd, SPI_IOC_WR_MODE, &spi_mode);
	if(status_value < 0)
	{
		perror("Could not set SPIMode (WR)...ioctl fail");
		exit(1);
	}

	status_value = ioctl(spi_fd, SPI_IOC_RD_MODE, &spi_mode);
	if(status_value < 0)
	{
		perror("Could not set SPIMode (RD)...ioctl fail");
		exit(1);
	}

	status_value = ioctl(spi_fd, SPI_IOC_WR_BITS_PER_WORD, &spi_bitsPerWord);
	if(status_value < 0)
	{
		perror("Could not set SPI bitsPerWord (WR)...ioctl fail");
		exit(1);
	}

	status_value = ioctl(spi_fd, SPI_IOC_RD_BITS_PER_WORD, &spi_bitsPerWord);
	if(status_value < 0)
	{
		perror("Could not set SPI bitsPerWord(RD)...ioctl fail");
		exit(1);
	}

	status_value = ioctl(spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed);
	if(status_value < 0)
	{
		perror("Could not set SPI speed (WR)...ioctl fail");
		exit(1);
	}

	status_value = ioctl(spi_fd, SPI_IOC_RD_MAX_SPEED_HZ, &spi_speed);
	if(status_value < 0)
	{
		perror("Could not set SPI speed (RD)...ioctl fail");
		exit(1);
	}
	return(status_value);
}

int SpiOpenPort (char *useSpiDevice)
{
	SpiSetDevice(useSpiDevice);
	int stat = SpiOpenPort();

	return stat;
}

int SpiOpenPort (char *useSpiDevice, int useSpiSpeed)
{
	SpiSetDevice(useSpiDevice);
	SpiSetSpeed(useSpiSpeed);
	int stat = SpiOpenPort();

	return stat;
}

size_t SpiRead(void *buf, size_t size)
{
	size_t rsize = read(spi_fd, buf, size);

	return rsize;
}

int SpiClosePort()
{
	int status_value = -1;

	status_value = close(spi_fd);
	if(status_value < 0)
	{
		perror("Error - Could not close SPI device");
		exit(1);
	}
	return(status_value);
}
