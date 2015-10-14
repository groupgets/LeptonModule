#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>                /* low-level i/o */
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include <pthread.h>
#include <semaphore.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <stdint.h>

#include "Palettes.h"
#include "SPI.h"
#include "Lepton_I2C.h"

#define PACKET_SIZE 164
#define PACKET_SIZE_UINT16 (PACKET_SIZE/2)
#define PACKETS_PER_FRAME 60
#define FRAME_SIZE_UINT16 (PACKET_SIZE_UINT16*PACKETS_PER_FRAME)
#define FPS 27;

static char const *v4l2dev = "/dev/video1";
static int v4l2sink = -1;
static int width = 80;                //640;    // Default for Flash
static int height = 60;        //480;    // Default for Flash
static char *vidsendbuf = NULL;
static int vidsendsiz = 0;

static int resets = 0;
static uint8_t result[PACKET_SIZE*PACKETS_PER_FRAME];
static uint16_t *frameBuffer;

static void init_device() {
    SpiOpenPort(0);
}

static void grab_frame() {

    resets = 0;
    for (int j = 0; j < PACKETS_PER_FRAME; j++) {
        read(spi_cs0_fd, result + sizeof(uint8_t) * PACKET_SIZE * j, sizeof(uint8_t) * PACKET_SIZE);
        int packetNumber = result[j * PACKET_SIZE + 1];
        if (packetNumber != j) {
            j = -1;
            resets += 1;
            usleep(1000);
            if (resets == 750) {
                SpiClosePort(0);
                usleep(750000);
                SpiOpenPort(0);
            }
        }
    }
    if (resets >= 30) {
        fprintf( stderr, "done reading, resets: \n" );
    }

    frameBuffer = (uint16_t *)result;
    int row, column;
    uint16_t value;
    uint16_t minValue = 65535;
    uint16_t maxValue = 0;

    for (int i = 0; i < FRAME_SIZE_UINT16; i++) {
        if (i % PACKET_SIZE_UINT16 < 2) {
            continue;
        }

        int temp = result[i * 2];
        result[i * 2] = result[i * 2 + 1];
        result[i * 2 + 1] = temp;

        value = frameBuffer[i];
        if (value > maxValue) {
            maxValue = value;
        }
        if (value < minValue) {
            minValue = value;
        }
        column = i % PACKET_SIZE_UINT16 - 2;
        row = i / PACKET_SIZE_UINT16;
    }

    float diff = maxValue - minValue;
    float scale = 255 / diff;
    for (int i = 0; i < FRAME_SIZE_UINT16; i++) {
        if (i % PACKET_SIZE_UINT16 < 2) {
            continue;
        }
        value = (frameBuffer[i] - minValue) * scale;
        const int *colormap = colormap_ironblack;
        column = (i % PACKET_SIZE_UINT16) - 2;
        row = i / PACKET_SIZE_UINT16;

        // Set video buffer pixel to scaled colormap value
        int idx = row * width * 3 + column * 3;
        vidsendbuf[idx + 0] = colormap[3 * value];
        vidsendbuf[idx + 1] = colormap[3 * value + 1];
        vidsendbuf[idx + 2] = colormap[3 * value + 2];
    }

    /*
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    memset( vidsendbuf, 0, 3);
    memcpy( vidsendbuf+3, vidsendbuf, vidsendsiz-3 );
    */
}

static void stop_device() {
    SpiClosePort(0);
}

static void open_vpipe()
{
    v4l2sink = open(v4l2dev, O_WRONLY);
    if (v4l2sink < 0) {
        fprintf(stderr, "Failed to open v4l2sink device. (%s)\n", strerror(errno));
        exit(-2);
    }
    // setup video for proper format
    struct v4l2_format v;
    int t;
    v.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
    t = ioctl(v4l2sink, VIDIOC_G_FMT, &v);
    if( t < 0 )
        exit(t);
    v.fmt.pix.width = width;
    v.fmt.pix.height = height;
    v.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB24;
    vidsendsiz = width * height * 3;
    v.fmt.pix.sizeimage = vidsendsiz;
    t = ioctl(v4l2sink, VIDIOC_S_FMT, &v);
    if( t < 0 )
        exit(t);
    vidsendbuf = (char*)malloc( vidsendsiz );
}

static pthread_t sender;
static sem_t lock1,lock2;
static void *sendvid(void *v)
{
    (void)v;
    for (;;) {
        sem_wait(&lock1);
        if (vidsendsiz != write(v4l2sink, vidsendbuf, vidsendsiz))
            exit(-1);
        sem_post(&lock2);
    }
}

int main(int argc, char **argv)
{
    struct timespec ts;

    if( argc == 2 )
        v4l2dev = argv[1];

    open_vpipe();

    // open and lock response
    if (sem_init(&lock2, 0, 1) == -1)
        exit(-1);
    sem_wait(&lock2);

    if (sem_init(&lock1, 0, 1) == -1)
        exit(-1);
    pthread_create(&sender, NULL, sendvid, NULL);

    for (;;) {
        // wait until a frame can be written
        fprintf( stderr, "Waiting for sink\n" );
        sem_wait(&lock2);
        // setup source
        init_device(); // open and setup SPI
        for (;;) {
            grab_frame();
            // push it out
            sem_post(&lock1);
            clock_gettime(CLOCK_REALTIME, &ts);
            ts.tv_sec += 2;
            // wait for it to get written (or is blocking)
            if (sem_timedwait(&lock2, &ts))
                break;
        }
        stop_device(); // close SPI
    }
    close(v4l2sink);
    return 0;
}
