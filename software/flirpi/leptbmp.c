#include <unistd.h>
#include <stdlib.h>

static unsigned short img[60][80];

static char hhead[] = "HTTP/1.1 200 OK\r\nContent-Type: image/x-bmp\r\n\r\n";
static unsigned char bmphead[] = {
    0x42, 0x4D, 0xF6, 0x16, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x04, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x50, 0x00, 0x00,
    0x00, 0x3C, 0x00, 0x00, 0x00, 0x01, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static void writebmp(void)
{
    int x, y;
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

    write(1, hhead, sizeof(hhead) - 1); // no trailing null
    write(1, bmphead, sizeof(bmphead));
    int b, r, g, a = 0, val;
    for (val = 0; val < 256; val++) {

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
#else
        b = r = g = val;
#endif
        write(1, &b, 1);
        write(1, &g, 1);
        write(1, &r, 1);
        write(1, &a, 1);
    }

    for (y = 59; y >= 0; y--)
        for (x = 0; x < 80; x++) {
            val = ((img[y][x] - minval) * 255) / (maxval);
            val &= 0xff;
            write(1, &val, 1);
        }
}

#include "leptsci.h"

int main(int argc, char *argv[])
{
    if (leptopen() || leptget((unsigned short *) img))
        return -1;
    leptclose();
    writebmp();
    return 0;
}
