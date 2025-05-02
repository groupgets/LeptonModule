#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>                /* low-level i/o */
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <stdint.h>

#include "SPI.h"
#include "Lepton.h"
#include "LeptonAction.h"
#include "LeptonActionV4l2.h"

#define DEFAULT_VIDEO_DEVICE "/dev/video1"

void printUsage(char *cmd) {
	char *cmdname = basename(cmd);
	printf("Usage: %s [OPTION]...\n"
			" -h      display this help and exit\n"
			" -cm x   select colormap\n"
			"           1 : rainbow\n"
			"           2 : grayscale\n"
			"           3 : ironblack [default]\n"
			" -tl x   select type of Lepton\n"
			"           2 : Lepton 2.x [default]\n"
			"           3 : Lepton 3.x\n"
			"               [for your reference] Please use nice command\n"
			"                 e.g. sudo nice -n 0 ./%s -tl 3\n"
			" -sd x   select SPI device\n"
			"           %s [default]\n"
			" -ss x   SPI bus speed [MHz] (10 - 30)\n"
			"           20 : 20MHz [default]\n"
			" -vd x   v4l2 loopback device "
			"           %s [default]\n"
			" -min x  override minimum value for scaling (0 - 65535)\n"
			"           [default] automatic scaling range adjustment\n"
			"           e.g. -min 30000\n"
			" -max x  override maximum value for scaling (0 - 65535)\n"
			"           [default] automatic scaling range adjustment\n"
			"           e.g. -max 32000\n"
			"", cmd, cmd, DEFAULT_SPI_DEVICE, DEFAULT_VIDEO_DEVICE);
}

int main(int argc, char **argv)
{

	int typeColormap = 3; // colormap_ironblack
	int typeLepton = 2; // Lepton 2.x
	char *spiDevice = DEFAULT_SPI_DEVICE; // SPI Device
	int spiSpeed = 20; // SPI bus speed 20MHz
	char *v4l2Device = DEFAULT_VIDEO_DEVICE;
	int rangeMin = -1; //
	int rangeMax = -1; //

	for(int i=1; i < argc; i++) {
		if (strcmp(argv[i], "-h") == 0) {
			printUsage(argv[0]);
			exit(0);
		}
		else if ((strcmp(argv[i], "-cm") == 0) && (i + 1 != argc)) {
			int val = std::atoi(argv[i + 1]);
			if ((val == 1) || (val == 2)) {
				typeColormap = val;
				i++;
			}
		}
		else if ((strcmp(argv[i], "-tl") == 0) && (i + 1 != argc)) {
			int val = std::atoi(argv[i + 1]);
			if (val == 3) {
				typeLepton = val;
				i++;
			}
		}
		else if ((strcmp(argv[i], "-sd") == 0) && (i + 1 != argc)) {
			spiDevice = argv[i + 1];
			i++;
		}
		else if ((strcmp(argv[i], "-ss") == 0) && (i + 1 != argc)) {
			int val = std::atoi(argv[i + 1]);
			if ((10 <= val) && (val <= 30)) {
				spiSpeed = val;
				i++;
			}
		}
		else if ((strcmp(argv[i], "-vd") == 0) && (i + 1 != argc)) {
			v4l2Device = argv[i + 1];
			i++;
		}
		else if ((strcmp(argv[i], "-min") == 0) && (i + 1 != argc)) {
			int val = std::atoi(argv[i + 1]);
			if ((0 <= val) && (val <= 65535)) {
				rangeMin = val;
				i++;
			}
		}
		else if ((strcmp(argv[i], "-max") == 0) && (i + 1 != argc)) {
			int val = std::atoi(argv[i + 1]);
			if ((0 <= val) && (val <= 65535)) {
				rangeMax = val;
				i++;
			}
		}
	}

	//
	Lepton *myLepton = new Lepton();
	myLepton->useColormap(typeColormap);
	myLepton->useLepton(typeLepton);
	myLepton->useSpiDevice(spiDevice);
	myLepton->useSpiSpeedMhz(spiSpeed);
	myLepton->setAutomaticScalingRange();
	if (0 <= rangeMin) myLepton->useRangeMinValue(rangeMin);
	if (0 <= rangeMax) myLepton->useRangeMaxValue(rangeMax);

	//
	LeptonActionV4l2 *leptonActionV4l2 = new LeptonActionV4l2();
	leptonActionV4l2->init(v4l2Device, myLepton->getWidth(), myLepton->getHeight());

	//
	int segment_number0 = 0;
	for (;;) {
		myLepton->open();
		for (;;) {
			int segment_number = myLepton->readFrameData((LeptonAction*)leptonActionV4l2);
			if (segment_number0 + 1 == segment_number) {
				if ((typeLepton == 2) || (segment_number == 4)) {
					leptonActionV4l2->send_video();
					segment_number0 = 0;
				}
				else {
					segment_number0 = segment_number;
				}
			}
			else {
				segment_number0 = 0;
			}
		}
		myLepton->close();
	}

	return 0;
}
