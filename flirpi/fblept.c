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
#include <linux/fb.h>
#include <string.h>

static struct fb_var_screeninfo vinfo;
static struct fb_fix_screeninfo finfo;
static char *fbp = 0;

#define WD 80
#define HT 60

#include "ironbow.c"

static unsigned short img[HT][80];
static unsigned char mag = 1,xo,yo;
static void writefb(int pal, int contour, int hmirror, int vmirror)
{
    int x, y, xb, yb;
    long int loc = 0;
    int stride = vinfo.bits_per_pixel >> 3;
    unsigned short minval = 0xffff, maxval = 0;
    for (y = 0; y < HT; y++)
        for (x = 0; x < 80; x++) {
            if (img[y][x] > maxval)
                maxval = img[y][x];
            if (img[y][x] < minval)
                minval = img[y][x];
        }
    maxval -= minval;           // span
    //    printf("%d + %d\n", minval, maxval);

    for (y = 0; y < HT; y++)
        for (x = 0; x < WD; x++) {
            int ym = vmirror? HT - 1 - y : y;
            int xm = hmirror? WD - 1 - x : x;
            unsigned short imgval = img[ym][xm];
            int val;
            if (!contour)
                val = ((imgval - minval) * 255) / (maxval);
            else
                val = imgval << 2;   // - minval;
            val &= 0xff;

            int r,b,g;
            switch(pal) {
                case 0:
                    b = ironbow[val][2];
                    r = ironbow[val][0];
                    g = ironbow[val][1];
                    break;
                case 1:
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
                    break;
                default: // grayscale
                    b=r=g= val;
                    break;
            }
            unsigned short int t = ((r & 0xf8) << 8) | ((g & 0xfc) << 3) | ((b & 0xf8) >> 3);

            for (yb = 0; yb < mag; yb++) {
                loc = (x * mag + xo) * stride + (yb + y * mag + yo) * finfo.line_length;
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
    int pal=0,cont=0,hmir=1,vmir=0,spiport=1;

    fbfd = open("/dev/fb1", O_RDWR);
    if (fbfd == -1)
        fbfd = open("/dev/fb0", O_RDWR);
    if (fbfd == -1)
        return -1;

    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1)
        return -2;
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1)
        return -3;
    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
    fbp = (char *) mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    if ((int) fbp == -1)
        return -4;
    memset( fbp, 0, vinfo.xres*vinfo.yres*(vinfo.bits_per_pixel/8));
    mag = vinfo.xres / 80;
    if (vinfo.yres / HT < mag)
        mag = vinfo.yres / HT;

    xo = (vinfo.xres - WD * mag) / 2 + vinfo.xoffset;
    yo = (vinfo.yres - HT * mag) / 2 + vinfo.yoffset;

    printf( "%d,%d,%d,%d\n",xo,yo,80*mag,60*mag );

    if (leptopen(spiport))
        return -7;
    for (;;) {
        if (leptget((unsigned short *) img))
            return -8;
        writefb(pal,cont,hmir,vmir);
    }
    leptclose();
    munmap(fbp, screensize);
    close(fbfd);
    return 0;
}
