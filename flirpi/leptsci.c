#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <stdint.h>

static uint8_t bits = 8;
static uint32_t speed = 10000000;       //16
static uint16_t delay = 0;
static int leptfd;

int leptopen(int port)
{
    uint8_t mode = 0;
    const char *device = port ? "/dev/spidev0.1" : "/dev/spidev0.0";
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
#include <string.h>
int leptget(unsigned short *img)
{
#define VOSPISIZ (164)
#define BUFMAX (4096/VOSPISIZ) //24
    memset( img, 8192>>8, 80*60*2);
    int row = -1;
    while( row != 59 ) {
        int cnt;
        uint8_t lepacket[VOSPISIZ * BUFMAX], *lpp;
        uint8_t tx[VOSPISIZ * BUFMAX] = { 0, };
        int maxpkts = row > 0 ? 59 - row : 60;
        if( maxpkts > BUFMAX )
            maxpkts = BUFMAX;
        struct spi_ioc_transfer tr = {
            .tx_buf = (unsigned long) tx,
            .rx_buf = (unsigned long) lepacket,
            .len = VOSPISIZ * maxpkts,
            .delay_usecs = delay,
            .speed_hz = speed,
            .bits_per_word = bits,
        };

        int i;
        i = ioctl(leptfd, SPI_IOC_MESSAGE(1), &tr);
        if (i < 1) {
            fprintf(stderr, "SPI_ERR");
            continue;
        }
        for( lpp = lepacket, cnt = 0; cnt < maxpkts; cnt++, lpp += VOSPISIZ ) {
            if (((lpp[0] & 0xf) == 0x0f)) 
                continue;
            row = lpp[1];
            if (row >= 60) {
                row = -1;
                continue;
                break;
            }
            for (i = 0; i < 80; i++)
                img[row * 80 + i]
                    = (lpp[2 * i + 4] << 8) | lpp[2 * i + 5];
            if( row == 59 )
                break;
        }
        //fprintf( stderr, "R:%d C:%d\n", row, cnt );
    }
    return 0;
}
