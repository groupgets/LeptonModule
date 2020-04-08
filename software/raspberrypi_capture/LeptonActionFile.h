#ifndef DEF_LeptonActionFile_h
#define DEF_LeptonActionFile_h

#include <stdint.h>

#include "LeptonAction.h"

class LeptonActionFile : public LeptonAction
{

public:
  LeptonActionFile();
  LeptonActionFile(int, int);
  ~LeptonActionFile();

  virtual void create(int, int);
  virtual char * getDefaultFilename(char *);
  virtual char * getFileExt(char *);
  virtual void saveBasename(char *);
  virtual void save();
  virtual void save(char *);
  virtual void setPixel(int, int, int, int, int);

private:

};

#endif
