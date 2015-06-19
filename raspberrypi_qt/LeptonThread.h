#ifndef LEPTONTHREAD
#define LEPTONTHREAD

#define HAVE_LEPTON true
#define DEBUG_LEPTON false

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>

#if HAVE_LEPTON
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#endif

#if DEBUG_LEPTON
#include <list>
#include <utility>
#endif

#include <QThread>
#include <QDebug>
#include <QVector>


class LeptonThread : public QThread {
    Q_OBJECT

    QVector<unsigned char> result;
    QVector<unsigned short> rawData;

#if HAVE_LEPTON
    static const char *device;
    static unsigned char mode, bits;
    static unsigned int speed;
    static unsigned short delay;
    static QVector<unsigned char> tx;

    int fd;
    struct spi_ioc_transfer _tr;
#endif

#if DEBUG_LEPTON
    std::list< std::pair<int, int> > sequence; // ...of packet #'s received from Lepton, for troubleshooting
#endif

    bool initLepton();
    int getPacket(int iRow, unsigned char *packetData);

public:
    enum {
        FrameWidth = 80,
        FrameHeight = 60,
        RowPacketWords = FrameWidth + 2,
        RowPacketBytes = 2*RowPacketWords,
        FrameWords = FrameWidth*FrameHeight
    };

    LeptonThread();
    ~LeptonThread();

    void run();

signals:
    void updateImage(unsigned short *, int, int);
};

#endif
