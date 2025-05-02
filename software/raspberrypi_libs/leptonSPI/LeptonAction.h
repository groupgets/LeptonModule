#ifndef TEXTLEPTONACTION
#define TEXTLEPTONACTION

class LeptonAction
{

public:
  LeptonAction();
  ~LeptonAction();

  virtual void setRawValue(int, int, uint16_t);
  virtual void setPixel(int, int, int, int, int);

private:

};

#endif
