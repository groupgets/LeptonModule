#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <stdint.h>

static uint8_t bits = 8;
static uint32_t speed = 16000000;       //16
static uint16_t delay = 0;
static int leptfd;

int leptget(unsigned short *img)
{
#define VOSPI_FRAME_SIZE (164)
    int row = -1;
    do {

        uint8_t lepacket[VOSPI_FRAME_SIZE];
        uint8_t tx[VOSPI_FRAME_SIZE] = { 0, };
        struct spi_ioc_transfer tr = {
            .tx_buf = (unsigned long) tx,
            .rx_buf = (unsigned long) lepacket,
            .len = VOSPI_FRAME_SIZE,
            .delay_usecs = delay,
            .speed_hz = speed,
            .bits_per_word = bits,
        };

        int i;
        i = ioctl(leptfd, SPI_IOC_MESSAGE(1), &tr);
        if (i < 1)
            continue;
        //return -1;
        if (((lepacket[0] & 0xf) == 0x0f))
            continue;
        //return -2;
        row = lepacket[1];
        if (row >= 60)
            continue;
        for (i = 0; i < 80; i++)
            img[row * 80 + i]
              = (lepacket[2 * i + 4] << 8) | lepacket[2 * i + 5];
        //printf ("%d ",img[row*81]);
    } while (row != 59);
    return 0;
}

int leptopen()
{
    uint8_t mode = 0;
    const char device[] = "/dev/spidev0.0";
    leptfd = open(device, O_RDWR);
    if (leptfd < 0)
        return -1;
    if (-1 == ioctl(leptfd, SPI_IOC_WR_MODE, &mode))
        return -2;
    if (-1 == ioctl(leptfd, SPI_IOC_RD_MODE, &mode))
        return -3;
    if (-1 == ioctl(leptfd, SPI_IOC_WR_BITS_PER_WORD, &bits))
        return -4;
    if (-1 == ioctl(leptfd, SPI_IOC_RD_BITS_PER_WORD, &bits))
        return -5;
    if (-1 == ioctl(leptfd, SPI_IOC_WR_MAX_SPEED_HZ, &speed))
        return -6;
    if (-1 == ioctl(leptfd, SPI_IOC_RD_MAX_SPEED_HZ, &speed))
        return -7;
    return 0;
}

void leptclose()
{
    close(leptfd);
}
