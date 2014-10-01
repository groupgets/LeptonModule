#include "LeptonThread.h"

#include "Palettes.h"
#include "SPI.h"

#define PACKET_SIZE 164
#define PACKET_SIZE_UINT16 (PACKET_SIZE/2)
#define PACKETS_PER_FRAME 60
#define FRAME_SIZE_UINT16 (PACKET_SIZE_UINT16*PACKETS_PER_FRAME)

LeptonThread::LeptonThread() : QThread()
{
}

LeptonThread::~LeptonThread() {
}

void LeptonThread::run()
{
	//create the initial image
	myImage = QImage(80, 60, QImage::Format_RGB888);

	//open spi port
	int res = SpiOpenPort(0);
	qDebug() << "status: " << res;

	//measuring timing
	int elapsedMicroseconds = 0;
	clock_t deltaClock;
	int idealFPS = 15;

	while(true) {
		startClock = clock();

		//read data packets from lepton over SPI
		//int resets = 0;
		qDebug() << "reading now";
		deltaClock = clock();
		for(int j=0;j<PACKETS_PER_FRAME;j++) {
			//if it's a drop packet, reset j to 0, set to -1 so he'll be at 0 again loop
			read(spi_cs0_fd, result+sizeof(uint8_t)*PACKET_SIZE*j, sizeof(uint8_t)*PACKET_SIZE);
			int packetNumber = result[j*PACKET_SIZE+1];
			if(packetNumber != j) {
				j = -1;
			}
		}
		int delta = (clock() - deltaClock)/(CLOCKS_PER_SEC/1000);
		qDebug() << "done reading: " << delta;

		deltaClock = clock();

		frameBuffer = (uint16_t *)result;
		int row, column;
		uint16_t value;
		uint16_t minValue = 65535;
		uint16_t maxValue = 0;

		
		for(int i=0;i<FRAME_SIZE_UINT16;i++) {
			//skip the first 2 uint16_t's of every packet, they're 4 header bytes
			if(i % PACKET_SIZE_UINT16 < 2) {
				continue;
			}
			
			//flip the MSB and LSB at the last second
			int temp = result[i*2];
			result[i*2] = result[i*2+1];
			result[i*2+1] = temp;
			
			value = frameBuffer[i];
			if(value > maxValue) {
				maxValue = value;
			}
			if(value < minValue) {
				minValue = value;
			}
			column = i % PACKET_SIZE_UINT16 - 2;
			row = i / PACKET_SIZE_UINT16 ;
		}
		delta = (clock() - deltaClock)/(CLOCKS_PER_SEC/1000);
		qDebug() << "processed: " << delta;

		deltaClock = clock();
		float diff = maxValue - minValue;
		float scale = 255/diff;
		QRgb color;
		for(int i=0;i<FRAME_SIZE_UINT16;i++) {
			if(i % PACKET_SIZE_UINT16 < 2) {
				continue;
			}
			value = (frameBuffer[i] - minValue) * scale;
			const int *colormap = colormap_ironblack;
			color = qRgb(colormap[3*value], colormap[3*value+1], colormap[3*value+2]);
			column = (i % PACKET_SIZE_UINT16 ) - 2;
			row = i / PACKET_SIZE_UINT16;
			myImage.setPixel(column, row, color);
		}

		delta = (clock() - deltaClock)/(CLOCKS_PER_SEC/1000);
		qDebug() << "coloring: " << delta;

		//lets emit the signal for update
		emit updateImage(myImage);

		//old code dealing with sleeping or emitting less frequently
		//scrapped cause SPI started chugging when we didn't refresh fast enough

		//figure out how long to sleep
		delta = (clock() - startClock)/(CLOCKS_PER_SEC/1000);
		int microseconds = delta*1000;
		int idealMicroseconds = 1000000/idealFPS;
		//int sleepTime = idealMicroseconds - microseconds;
		//if(sleepTime > 0) {
		//	usleep( sleepTime );
		//}
	
		elapsedMicroseconds += microseconds;
		if(elapsedMicroseconds >= idealMicroseconds) {
			elapsedMicroseconds -= idealMicroseconds;
			//emit updateImage(myImage);
		}
		qDebug() << "total: " << delta;
	}
	
	//finally, close SPI port just bcuz
	SpiClosePort(0);
}
