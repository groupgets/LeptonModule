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

static void pabort(const char *s)
{
    perror(s);
    abort();
}

static struct fb_var_screeninfo vinfo;
static struct fb_fix_screeninfo finfo;
static char *fbp = 0;

#define WD 80
#define HT 60

static unsigned short img[HT][80];
static unsigned char mag = 0, xo, yo;
static unsigned contour = 0;
static void writefb(void)
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

            int val;
            if (!contour)
                val = ((img[y][x] - minval) * 255) / (maxval);
            else
                val = img[y][x] << 2;   // - minval;
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

char *spidev = "/dev/spidev0.0";

void usage(char *exec)
{
    printf("Usage: %s [options]\n"
           "Options:\n"
           "  -c | --contour           Set contour to 1\n"
           "  -d | --device name       spidev device name (%s by default)\n"
           "  -h | --help              Print this message\n"
           "  -m | --magnify factor    Force given factor as magnify factor "
               "instead of auto-calculated one\n"
           "", exec, spidev);
}

static const char short_options [] = "cd:hm:";

static const struct option long_options [] = {
    { "contour", no_argument,       NULL, 'c' },
    { "device",  required_argument, NULL, 'd' },
    { "help",    no_argument,       NULL, 'h' },
    { "magnify", required_argument, NULL, 'm' },
    { 0, 0, 0, 0 }
};

int main(int argc, char *argv[])
{
    int fbfd = 0;
    long int screensize = 0;

    /* Processing command line parameters */
    for (;;) {
        int index;
        int c;

        c = getopt_long(argc, argv,
                        short_options, long_options,
                        &index);

        if (-1 == c)
            break;

        switch (c) {
            case 0: /* getopt_long() flag */
                break;

            case 'c':
                contour = 1;
                break;

            case 'd':
                spidev = optarg;
                break;

            case 'h':
                usage(argv[0]);
                exit(EXIT_SUCCESS);

            case 'm':
                mag = strtoul(optarg, 0, 10);
                break;

            default:
                usage(argv[0]);
                exit(EXIT_FAILURE);
        }
    }

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
    //printf("The framebuffer device was mapped to memory successfully.\n");
    memset( fbp, 0, vinfo.xres*vinfo.yres*(vinfo.bits_per_pixel/8));
    if (!mag) {
        mag = vinfo.xres / 80;
    }
    if (vinfo.yres / HT < mag)
        mag = vinfo.yres / HT;
    printf("Using magnify factor of %dx\n", mag);

    xo = (vinfo.xres - WD * mag) / 2 + vinfo.xoffset;
    yo = (vinfo.yres - HT * mag) / 2 + vinfo.yoffset;
printf( "%d,%d,%d,%d\n",xo,yo,80*mag,60*mag );
    if (leptopen(spidev))
        return -7;

    for (;;) {
        if (leptget((unsigned short *) img))
            return -8;
        writefb();
        //usleep(125000);
    }
    leptclose();
    munmap(fbp, screensize);
    close(fbfd);
    return 0;
}
