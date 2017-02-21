#include "LeptonThread.h"
#include "stdio.h"
#include "Palettes.h"
#include "SPI.h"
#include "Lepton_I2C.h"

#define PACKET_SIZE 164
#define PACKET_SIZE_UINT16 (PACKET_SIZE/2)
#define PACKETS_PER_FRAME 60
#define FRAME_SIZE_UINT16 (PACKET_SIZE_UINT16*PACKETS_PER_FRAME)
#define FPS 27

#define HAND_TEMP_THRESHOLD 8050

int readyToToggle = 1;

LeptonThread::LeptonThread() : QThread(){}

LeptonThread::~LeptonThread() {}

void LeptonThread::run()
{
	//Create the initial image and open the Spi port.
	myImage = QImage(160, 120, QImage::Format_RGB888);
	SpiOpenPort(0);
	uint32_t totalCounts = 0;
	uint16_t minValue = 65535;
	uint16_t maxValue = 0;
	int checkSegment = 1;
	//Camera Loop
	while(true) {
		int resets = 0;

		//Read through one segment, line by line.	
		for(int j=0;j<PACKETS_PER_FRAME;j++) {
			read(spi_cs0_fd, result+sizeof(uint8_t)*PACKET_SIZE*j, sizeof(uint8_t)*PACKET_SIZE);
			int packetNumber = result[j*PACKET_SIZE+1];

			//Check to make sure packet number is correct, or else the Lepton is out of sync.
			//We have 750 'resets' before re-syncing.
			if(packetNumber != j) {
				j = -1;
				resets += 1;
				usleep(1000);
				if(resets == 750) {
					SpiClosePort(0);
					usleep(750000);
					SpiOpenPort(0);
				}
			}
		}

		int row, column;
		uint16_t value;

		//Obtain the segment number from the header of line 20. It should never be 0.
		uint8_t segNum = result[20*PACKET_SIZE]>>4;
		if(segNum != checkSegment){
			checkSegment = 1;
			continue;
		}
		checkSegment++;
		if(checkSegment > 4) {
			checkSegment = 1;
		}

		 //Iterate through the current segment one line at a time. In this loop, we find the
		 //minimum and maximum values of the image for AGC.
		for(int i=0;i<FRAME_SIZE_UINT16;i++) {
			//Skip the header of each line
			if(i % PACKET_SIZE_UINT16 < 2) continue; 
			
			//Flip the MSB and LSB for correct coloring.
			frameBuffer[i + (segNum-1)*(FRAME_SIZE_UINT16)] = result[i*2]<<8 | result[i*2+1];
	
			value = frameBuffer[i+ (segNum-1)*FRAME_SIZE_UINT16];
			totalCounts += value;
			if(value > maxValue) {
				maxValue = value;
			}
			if(value < minValue && value > 0) {
				minValue = value;
			}
			column = i % PACKET_SIZE_UINT16 - 2;
			row = i / (PACKET_SIZE_UINT16 * 2);
		}
	
		//We only finalize the image for emission once all four segments are in.
		if(segNum != 4)continue; 	
		
		//If the difference between Max and Min is 0, we need to get a new frame before emitting.
		float diff = maxValue - minValue;
		if(diff != 0){
			float scale = 255/diff;
			QRgb color;

			//Iterates through the entire frame, all four segments, for colorization.
			for(int i=0;i<FRAME_SIZE_UINT16*4;i++) {
				//Skip the header of each line.
				if(i % PACKET_SIZE_UINT16 < 2) continue; 
				
				value = (frameBuffer[i] - minValue) * scale;
				const int *colormap = colormap_ironblack;
				if(value > 255) value = 255;	
				color = qRgb(colormap[3*value], colormap[3*value+1], colormap[3*value+2]);
			
				column = (i % PACKET_SIZE_UINT16 ) - 2;
				row = i / (PACKET_SIZE_UINT16);
				int new_row = (row/2);
				int new_column = (row % 2 == 0) ? column : column + 80 ;
				myImage.setPixel(new_column, new_row, color);
			}

			//Emit the finalized image for update.
			emit updateImage(myImage);
		}
		minValue = 65535;
		maxValue = 0;
		totalCounts = 0;
	}
	
	SpiClosePort(0);
}

void LeptonThread::performFFC() {
	lepton_perform_ffc();
}
