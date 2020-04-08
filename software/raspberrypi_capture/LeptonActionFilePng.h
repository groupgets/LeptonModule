#ifndef DEF_LeptonActionFilePng_h
#define DEF_LeptonActionFilePng_h

#include <stdint.h>
#include <png++/png.hpp>

#include "LeptonActionFile.h"

class LeptonActionFilePng : public LeptonActionFile
{

public:
  LeptonActionFilePng();
  LeptonActionFilePng(int, int);
  ~LeptonActionFilePng();

  virtual void create(int, int);
  virtual char * getDefaultFilename(char *);
  virtual char * getFileExt(char *);
  virtual void saveBasename(char *);
  virtual void save();
  virtual void save(char *);
  virtual void setPixel(int, int, int, int, int);

private:
  png::image < png::rgb_pixel > image_png;

};

#endif
