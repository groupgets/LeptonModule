#ifndef TEXTTHREAD
#define TEXTTHREAD

#include <ctime>
#include <stdint.h>

#include <QThread>
#include <QtCore>
#include <QPixmap>
#include <QImage>

#include "Lepton.h"

class LeptonThread : public QThread
{
  Q_OBJECT;

public:
  LeptonThread();
  ~LeptonThread();

  void setLepton(Lepton *);
  void run();

public slots:
  void performFFC();

signals:
  void updateText(QString);
  void updateImage(QImage);

private:

  Lepton *myLepton;

  int myImageWidth;
  int myImageHeight;
  QImage myImage;

};

#endif
