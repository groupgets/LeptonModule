#ifndef TEXTTHREAD
#define TEXTTHREAD

#include <ctime>
#include <stdint.h>

#include <QThread>
#include <QtCore>
#include <QPixmap>
#include <QImage>

#include "leptoncontrol.h"

class LeptonThread : public QThread
{
  Q_OBJECT;

public:
  LeptonThread();
  ~LeptonThread();

  void run();
  bool LoadPalette(const char *file_name);//çàãðóçèòü ïàëèòðó
  void CreateImage(void);

public slots:
  void performFFC();

signals:
  void updateText(QString);
  void updateImage(QImage);

private:

  QImage myImage;
  uint16_t *frameBuffer;
  unsigned long ColorMap_R[256];//ïàëèòðà R
  unsigned long ColorMap_G[256];//ïàëèòðà G
  unsigned long ColorMap_B[256];//ïàëèòðà B

};

#endif
