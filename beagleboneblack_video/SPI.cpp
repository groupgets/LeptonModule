#include "SPI.h"

int spi_cs0_fd = -1;
int spi_cs1_fd = -1;

unsigned char spi_mode = SPI_MODE_3;
unsigned char spi_bitsPerWord = 8;
unsigned int spi_speed = 10000000;

int SpiOpenPort (int spi_device)
{
	int status_value = -1;
	int *spi_cs_fd;


	//----- SET SPI MODE -----
	//SPI_MODE_0 (0,0)  CPOL=0 (Clock Idle low level), CPHA=0 (SDO transmit/change edge active to idle)
	//SPI_MODE_1 (0,1)  CPOL=0 (Clock Idle low level), CPHA=1 (SDO transmit/change edge idle to active)
	//SPI_MODE_2 (1,0)  CPOL=1 (Clock Idle high level), CPHA=0 (SDO transmit/change edge active to idle)
	//SPI_MODE_3 (1,1)  CPOL=1 (Clock Idle high level), CPHA=1 (SDO transmit/change edge idle to active)
	spi_mode = SPI_MODE_3;

	//----- SET BITS PER WORD -----
	spi_bitsPerWord = 8;

	//----- SET SPI BUS SPEED -----
	spi_speed = 10000000;				//1000000 = 1MHz (1uS per bit)


	if (spi_device)
		spi_cs_fd = &spi_cs1_fd;
	else
		spi_cs_fd = &spi_cs0_fd;


	if (spi_device)
		*spi_cs_fd = open(std::string("/dev/spidev1.0").c_str(), O_RDWR);
	else
		*spi_cs_fd = open(std::string("/dev/spidev1.0").c_str(), O_RDWR);

	if (*spi_cs_fd < 0)
	{
		perror("Error - Could not open SPI device");
		exit(1);
	}

	status_value = ioctl(*spi_cs_fd, SPI_IOC_WR_MODE, &spi_mode);
	if(status_value < 0)
	{
		perror("Could not set SPIMode (WR)...ioctl fail");
		exit(1);
	}

	status_value = ioctl(*spi_cs_fd, SPI_IOC_RD_MODE, &spi_mode);
	if(status_value < 0)
	{
		perror("Could not set SPIMode (RD)...ioctl fail");
		exit(1);
	}

	status_value = ioctl(*spi_cs_fd, SPI_IOC_WR_BITS_PER_WORD, &spi_bitsPerWord);
	if(status_value < 0)
	{
		perror("Could not set SPI bitsPerWord (WR)...ioctl fail");
		exit(1);
	}

	status_value = ioctl(*spi_cs_fd, SPI_IOC_RD_BITS_PER_WORD, &spi_bitsPerWord);
	if(status_value < 0)
	{
		perror("Could not set SPI bitsPerWord(RD)...ioctl fail");
		exit(1);
	}

	status_value = ioctl(*spi_cs_fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed);
	if(status_value < 0)
	{
		perror("Could not set SPI speed (WR)...ioctl fail");
		exit(1);
	}

	status_value = ioctl(*spi_cs_fd, SPI_IOC_RD_MAX_SPEED_HZ, &spi_speed);
	if(status_value < 0)
	{
		perror("Could not set SPI speed (RD)...ioctl fail");
		exit(1);
	}
	return(status_value);
}

int SpiClosePort(int spi_device)
{
		int status_value = -1;
	int *spi_cs_fd;

	if (spi_device)
		spi_cs_fd = &spi_cs1_fd;
	else
		spi_cs_fd = &spi_cs0_fd;


	status_value = close(*spi_cs_fd);
	if(status_value < 0)
	{
		perror("Error - Could not close SPI device");
		exit(1);
	}
	return(status_value);
}
