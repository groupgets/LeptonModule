#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <linux/fb.h>

static void pabort(const char *s)
{
    perror(s);
    abort();
}

static struct fb_var_screeninfo vinfo;
static struct fb_fix_screeninfo finfo;
static char *fbp = 0;

static unsigned short img[60][80];
static unsigned char mag = 1;

static void writefb(void)
{
    int x, y, xb, yb;
    long int loc = 0;
    int stride = vinfo.bits_per_pixel >> 3;
    unsigned short minval = 0xffff, maxval = 0;
    for (y = 0; y < 60; y++)
        for (x = 0; x < 80; x++) {
            if (img[y][x] > maxval)
                maxval = img[y][x];
            if (img[y][x] < minval)
                minval = img[y][x];
        }
    maxval -= minval;           // span
    //    printf("%d + %d\n", minval, maxval);

    for (y = 0; y < 60; y++)
        for (x = 0; x < 80; x++) {

            int val = ((img[y][x] - minval) * 255) / (maxval);
            val &= 0xff;

            int b = val;
            int r = val;
            int g = val;
#define COLOR
#ifdef COLOR
            switch (val >> 6) {
            case 0:
                b = 255;
                g = 0;
                r = 255 - (val << 2);
                break;
            case 1:
                r = 0;
                b = 255 - (val << 2);
                g = (val << 2);
                break;
            case 2:
                b = 0;
                g = 255;
                r = (val << 2);
                break;
            case 3:
                b = 0;
                r = 255;
                g = 255 - (val << 2);
                break;
            default:
                break;
            }
#endif
            unsigned short int t = ((r & 0xf8) << 8) | ((g & 0xfc) << 3) | ((b & 0xf8) >> 3);

            for (yb = 0; yb < mag; yb++) {
                loc = (x * mag + vinfo.xoffset) * stride + (yb + y * mag + vinfo.yoffset) * finfo.line_length;
                for (xb = 0; xb < mag; xb++) {
                    if (vinfo.bits_per_pixel == 32) {
                        *(fbp + loc++) = b;     // Some blue
                        *(fbp + loc++) = g;     // A little green
                        *(fbp + loc++) = r;     // A lot of red
                        *(fbp + loc++) = 0;     // No transparency
                    }
                    else {      //assume 16bpp
                        *(fbp + loc++) = t;
                        *(fbp + loc++) = t >> 8;
                    }
                }
            }
        }
}
#include "leptsci.h"

int main(int argc, char *argv[])
{
    int fbfd = 0;
    long int screensize = 0;
    fbfd = open("/dev/fb0", O_RDWR);
    if (fbfd == -1)
        pabort("Error: cannot open framebuffer device");
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1)
        pabort("Error reading fixed information");
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1)
        pabort("Error reading variable information");
    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
    fbp = (char *) mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    if ((int) fbp == -1)
        pabort("Error: failed to map framebuffer device to memory");
    printf("The framebuffer device was mapped to memory successfully.\n");
    mag = vinfo.xres / 80;
    if (vinfo.yres / 60 < mag)
        mag = vinfo.yres / 60;
    if (leptopen())
        return -7;
    for (;;) {
        if (leptget((unsigned short *)img))
            return -8;
        writefb();
        usleep(125000);
    }
    leptclose();
    munmap(fbp, screensize);
    close(fbfd);
    return 0;
}
