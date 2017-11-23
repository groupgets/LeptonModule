#ifndef TEXTTHREAD
#define TEXTTHREAD

#include <ctime>
#include <stdint.h>

#include <QThread>
#include <QtCore>
#include <QPixmap>
#include <QImage>

#define PACKET_SIZE 3280
#define PACKET_SIZE_UINT16 1640 //(PACKET_SIZE/2)
#define PACKETS_PER_FRAME 12
#define FRAME_SIZE_UINT16 19680 //(PACKET_SIZE_UINT16*PACKETS_PER_FRAME)

class LeptonThread : public QThread
{
  Q_OBJECT;

public:
  LeptonThread();
  ~LeptonThread();

  void run();

public slots:
  void performFFC();

signals:
  void updateText(QString);
  void updateImage(QImage);

private:

  QImage myImage;

  uint8_t result[PACKET_SIZE*PACKETS_PER_FRAME];
//  uint16_t* frameBuffer;
  uint16_t frameBuffer[FRAME_SIZE_UINT16];

};

#endif
