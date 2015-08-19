#include "LeptonThread.h"

#include <QString>
#include <QTextStream>

LeptonThread::LeptonThread()
    : QThread()
    , result(RowPacketBytes*FrameHeight)
    , rawData(FrameWords) { }

LeptonThread::~LeptonThread() { }

#if HAVE_LEPTON
const char *LeptonThread::device = "/dev/spidev0.1"; // Change to 0.0 if necessary!
unsigned char LeptonThread::mode = 0, LeptonThread::bits = 8;
unsigned int LeptonThread::speed = 16000000;
unsigned short LeptonThread::delay = 0;
QVector<unsigned char> LeptonThread::tx(LeptonThread::RowPacketBytes, 0);

bool LeptonThread::initLepton() {
    fd = open(device, O_RDWR);
    if (fd < 0)
        qDebug() << "Can't open device";
    else if (-1 == ioctl(fd, SPI_IOC_WR_MODE, &mode))
        qDebug() << "Can't set SPI mode";
    else if (-1 == ioctl(fd, SPI_IOC_RD_MODE, &mode))
        qDebug() << "Can't get SPI mode";
    else if (-1 == ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits))
        qDebug() << "Can't set bits per word";
    else if (-1 == ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits))
        qDebug() << "Can't get bits per word";
    else if (-1 == ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed))
        qDebug() << "Can't set max speed";
    else if (-1 == ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed))
        qDebug() << "Can't get max speed";
    else
        return true;
    return false;
}
#else
static int counter = 0;
#endif

int LeptonThread::getPacket(int iRow, unsigned char *packetData) {
#if HAVE_LEPTON
    _tr.rx_buf = (unsigned long) packetData;
    return ioctl(fd, SPI_IOC_MESSAGE(1), &_tr);
#else
    packetData[0] = 0;
    packetData[1] = iRow;
    for (int i = 4; i < RowPacketBytes; i += 2)
        *(short *)(packetData+i) = ((iRow+counter) % 20) + ((i/2-2) % 26);
    return 1;
#endif
}

void LeptonThread::run() {    
#if HAVE_LEPTON
    if (!initLepton()) return;

    usleep(250000);

    _tr.tx_buf = (unsigned long) &tx[0];
    _tr.len = RowPacketBytes;
    _tr.delay_usecs = delay;
    _tr.speed_hz = speed;
    _tr.bits_per_word = bits;
#endif

    int resets = 0; // Number of times we've reset the 0...59 loop for packets
    int errors = 0; // Number of error-packets received
    while (true) {
        int iRow;
        for (iRow = 0; iRow < FrameHeight; ) {
            unsigned char *packet = &result[iRow*RowPacketBytes];

            if (getPacket(iRow, packet) < 1) {
                qDebug() << "Error transferring SPI packet";
                return;
            }

            int packetNumber;
            if ((packet[0] & 0xf)==0xf)
                packetNumber = -1;
            else
                packetNumber = packet[1];

#if DEBUG_LEPTON
            if (sequence.empty() || sequence.back().first!=packetNumber)
                sequence.push_back(std::make_pair(packetNumber, 1));
            else
                ++sequence.back().second;
#endif

            if (packetNumber==-1) {
                usleep(1000);
                if (++errors > 300) break;
                continue;
            }

            if (packetNumber != iRow) {
                usleep(1000);
                break;
            }

            ++iRow;
        }

        if (iRow < FrameHeight) {
            if (++resets >= 750) {
                qDebug() << "Packet reset counter hit 750";
                resets = 0;
                usleep(750000);
            }
            continue;
        }

#if DEBUG_LEPTON
        QString msg;
        QTextStream os(&msg);
        bool chain = false, first = true; int chain0, chain1;
        for (std::list< std::pair<int, int> >::iterator iSeq = sequence.begin(); iSeq != sequence.end(); ++iSeq) {
            if (chain && iSeq->first==chain1+1) { ++chain1; continue; }
            if (chain && chain1!=chain0) os << "-" << chain1;
            if (iSeq->first >= 0 && !chain) { chain = true; chain0 = chain1 = iSeq->first; }
            if (first) first = false; else os << " ";
            if (iSeq->first==-1) os << "*"; else os << iSeq->first;
            if (iSeq->second!=1) { os << "^" << iSeq->second; chain = false; }
        }
        if (chain && chain1!=chain0) os << "-" << chain1;
        qDebug() << msg;
        sequence.clear();
        // qDebug() << resets << "resets," << errors << "errors";
#endif

        resets = 0; errors = 0;

        uint16_t minValue = 65535;
        uint16_t maxValue = 0;
        unsigned char *in = &result[0];
        unsigned short *out = &rawData[0];
        for (int iRow = 0; iRow < FrameHeight; ++iRow) {
            in += 4;
            for (int iCol = 0; iCol < FrameWidth; ++iCol) {
                unsigned short value = in[0];
                value <<= 8;
                value |= in[1];
                in += 2;
                if (value > maxValue) maxValue = value;
                if (value < minValue) minValue = value;
                *(out++) = value;
            }
        }

        emit updateImage(&rawData[0], minValue, maxValue);

#if !HAVE_LEPTON
        usleep(50000);  // Need to slow things down if no ioctl call!
        counter = (counter + 1)%520;
#endif
    }
}
