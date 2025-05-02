#ifndef TEXTLEPTONACTIONQT
#define TEXTLEPTONACTIONQT

#include "LeptonAction.h"

class LeptonActionQt : public LeptonAction
{

public:
  LeptonActionQt();
  ~LeptonActionQt();

  void setQImage(QImage *);
  virtual void setPixel(int, int, int, int, int);

private:
  QImage *myImage;

};

#endif
