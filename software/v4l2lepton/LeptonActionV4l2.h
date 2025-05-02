#ifndef TEXTLEPTONACTIONV4L2
#define TEXTLEPTONACTIONV4L2

#include "LeptonAction.h"

#define DEV_VIDEO "/dev/video1"

class LeptonActionV4l2 : public LeptonAction
{

public:
  LeptonActionV4l2();
  ~LeptonActionV4l2();

  virtual void setPixel(int, int, int, int, int);

  void init(char *dev_video, int w, int h);
  void send_video();

private:
  char *v4l2dev = DEV_VIDEO;
  int v4l2sink = -1;

  int width;
  int height;
  int size_video_buf;
  char *video_buf = NULL;

};

#endif
