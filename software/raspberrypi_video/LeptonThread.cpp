#include "LeptonThread.h"
#include "stdio.h"
#include "Palettes.h"
#include "SPI.h"
#include "Lepton_I2C.h"
#include <errno.h>

//We are using a struct for the entire image, made up of 4 segments, 60 lines
//each, with 164 bytes of data. Allows easy use of sizeof() in code.
typedef struct __attribute__((packed)) _packet {
	uint16_t packet_number;
	uint16_t crc;
	uint8_t video_src[160];
} packet;

typedef struct __attribute__((packed)) _segment {
	packet line[60];
} segment;

typedef struct __attribute__((packed)) _image {
	segment seg[4];
} image;


#define FPS (27)

#define HAND_TEMP_THRESHOLD 8050

int readyToToggle = 1;

LeptonThread::LeptonThread() : QThread(){}

LeptonThread::~LeptonThread() {}

void LeptonThread::run()
{
	//Create the initial image and open the Spi port.
	uint16_t spi_port = 1;
	myImage = QImage(160, 120, QImage::Format_RGB888);
	SpiOpenPort(spi_port);
	uint32_t totalCounts = 0;
	uint16_t minValue = 65535;
	uint16_t maxValue = 0;
	int resets = 0;
	
	//Camera Loop
	while (true) {
		int spi_port_fd = spi_port == 1 ? spi_cs1_fd : spi_cs0_fd;
		image img;
		bool flag = false;	
		for (int segNum = 0; segNum < 4; segNum++) {
			
			//Read through until the 0th packet.
			int packet_num = 1;
			while (packet_num != 0) {
				read(spi_port_fd, &img.seg[segNum], sizeof(packet));
				packet_num = img.seg[segNum].line[0].packet_number>>8;
			}
			
			//Read the rest of this 1/3 segment.
			read(spi_port_fd, &img.seg[segNum].line[1], 19*sizeof(packet));
			
			//Read the remainder of the segment.
			for (int part=1;part<3;part++) {
				read(spi_port_fd, &img.seg[segNum].line[20*(part%3)], 20*sizeof(packet));
			}
			
			//If the segment number is 0, the frame is invalid.
			if ((img.seg[segNum].line[20].packet_number & 0xff) == 0) {
				flag = true;
				if ((img.seg[segNum].line[20].packet_number >> 8) != 0x14) {
					resets++;
				}
				break;
			}
		}
		if (flag) {
			continue;
		}
		
		//Check to make sure packet number is correct, or else the Lepton is out of sync.
		//We have 750 'resets' before re-syncing.
		if (resets == 750) {
			SpiClosePort(spi_port);
			usleep(750000);
			SpiOpenPort(spi_port);
			resets = 0;
			continue;
		}
			


		int row, column;
		uint16_t value;

		//Iterate through the current segment pixel at a time. In this loop, we find the
		//minimum and maximum values of the image for AGC.
		int lineNum = 0;
		int pixelNum = 0;
		int segNum = 0;
		for (unsigned i=0;i<sizeof(image)/2;i++) {
			
			//Skip the header of each line
			if (i % (sizeof(packet)/2) < 2) {
				continue;
			}

			//80 pixels per line, 60 lines per segment.
			pixelNum++;
			if (pixelNum == 80) {
				pixelNum = 0;
				lineNum++;
			}
			if (lineNum == 60) {
				lineNum = 0;
				segNum++;
			}
			
			//Flip the MSB and LSB for correct coloring.
			frameBuffer[i] = img.seg[segNum].
					line[lineNum].
					video_src[2*pixelNum] << 8 | 
					img.seg[segNum].
					line[lineNum].
					video_src[2*pixelNum+1] << 0;
			
			value = frameBuffer[i];
			totalCounts += value;
			if (value > maxValue) {
				maxValue = value;
			}
			if (value < minValue && value > 0) {
				minValue = value;
			}
		}
		
		//If the difference between Max and Min is 0, we need to get a new frame before emitting.
		float diff = maxValue - minValue;
		if (diff != 0) {
			float scale = 255/diff;
			QRgb color;
			
			//Iterates through the entire frame, one pixel at a time, for colorization.
			for (unsigned i=0;i<sizeof(image)/2;i++) {
				
				//Skip the header of each line.
				if (i % (sizeof(packet)/2) < 2) {
					continue; 
				}

				value = (frameBuffer[i] - minValue) * scale;
				const int *colormap = colormap_ironblack;
				if (value > 255) { 
					value = 255;
				}
	
				color = qRgb(colormap[3*value], colormap[3*value+1], colormap[3*value+2]);
				
				
				column = (i % (sizeof(packet)/2)) - 2;
				row = i / (sizeof(packet)/2);
				int newColumn = (row % 2 == 0) ? column : column + 80 ;
				int newRow = row/2;
				myImage.setPixel(newColumn, newRow, color);
			}

			//Emit the finalized image for update.
			emit updateImage(myImage);
		}
		minValue = 65535;
		maxValue = 0;
		totalCounts = 0;
		resets = 0;
	}
	
	SpiClosePort(spi_port);
}

void LeptonThread::performFFC() {
	lepton_perform_ffc();
}
