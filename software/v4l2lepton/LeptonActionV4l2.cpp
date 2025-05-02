#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>                /* low-level i/o */
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <stdint.h>

#include <linux/videodev2.h>

#include "LeptonActionV4l2.h"

LeptonActionV4l2::LeptonActionV4l2()
{
}

LeptonActionV4l2::~LeptonActionV4l2() {
    if (video_buf != NULL) {
        free(video_buf);
    }
}

void LeptonActionV4l2::init(char *dev_video, int w, int h)
{
    v4l2dev = dev_video;
    width = w;
    height = h;
    size_video_buf = width * height * 3;

    if (0 < v4l2sink) {
        close(v4l2sink);
    }
    v4l2sink = open(v4l2dev, O_WRONLY);
    if (v4l2sink < 0) {
        fprintf(stderr, "Failed to open v4l2sink device. (%s)\n", strerror(errno));
        exit(-2);
    }

    // setup video for proper format
    struct v4l2_format v;
    int stat;
    v.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
    stat = ioctl(v4l2sink, VIDIOC_G_FMT, &v);
    if (stat < 0) {
        fprintf(stderr, "[ERROR] Failed to ioctl v4l2sink VIDIOC_G_FMT. (%s)\n", strerror(errno));
        exit(stat);
    }
    v.fmt.pix.width = width;
    v.fmt.pix.height = height;
    v.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB24;

    //
    v.fmt.pix.sizeimage = size_video_buf;
    stat = ioctl(v4l2sink, VIDIOC_S_FMT, &v);
    if (stat < 0) {
        fprintf(stderr, "[ERROR] Failed to ioctl v4l2sink VIDIOC_S_FMT. (%s)\n", strerror(errno));
        exit(stat);
    }

    if (video_buf != NULL) {
        free(video_buf);
    }
    video_buf = (char*)malloc(size_video_buf);
}

void LeptonActionV4l2::send_video()
{
    int wsize = write(v4l2sink, video_buf, size_video_buf);
    if (wsize != size_video_buf) {
        fprintf(stderr, "[WARNING] Sent data size mismatch. (write %d/%d bytes)\n", wsize, size_video_buf);
    }
}

void LeptonActionV4l2::setPixel(int col, int row, int colorR, int colorG, int colorB)
{
    int ofs = row * width * 3 + col * 3;
    video_buf[ofs + 0] = colorR;
    video_buf[ofs + 1] = colorG;
    video_buf[ofs + 2] = colorB;
}
