#ifndef DEF_LeptonActionFileCsv_h
#define DEF_LeptonActionFileCsv_h

#include <stdint.h>

#include "LeptonActionFile.h"

class LeptonActionFileCsv : public LeptonActionFile
{

public:
  LeptonActionFileCsv();
  LeptonActionFileCsv(int, int);
  ~LeptonActionFileCsv();

  virtual void create(int, int);
  virtual char * getDefaultFilename(char *);
  virtual char * getFileExt(char *);
  virtual void saveBasename(char *);
  virtual void save();
  virtual void save(char *);
  virtual void setRawValue(int, int, uint16_t);

private:
  uint16_t *image_csv = NULL;
  int image_width;
  int image_height;

};

#endif
