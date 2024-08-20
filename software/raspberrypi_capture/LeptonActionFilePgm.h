#ifndef DEF_LeptonActionFilePgm_h
#define DEF_LeptonActionFilePgm_h

#include <stdint.h>

#include "LeptonActionFile.h"

class LeptonActionFilePgm : public LeptonActionFile
{

public:
  LeptonActionFilePgm();
  LeptonActionFilePgm(int, int);
  ~LeptonActionFilePgm();

  virtual void create(int, int);
  virtual char * getDefaultFilename(char *);
  virtual char * getFileExt(char *);
  virtual void saveBasename(char *);
  virtual void save();
  virtual void save(char *);
  virtual void setRawValue(int, int, uint16_t);

private:
  uint16_t *image_pgm = NULL;
  int image_width;
  int image_height;

};

#endif
