#include <string>
#include <unistd.h>

#include "Lepton.h"
#include "LeptonAction.h"
#include "LeptonActionFile.h"
#include "LeptonActionFileCsv.h"
#include "LeptonActionFilePng.h"
#include "LeptonActionFilePgm.h"

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
               " -ss x   SPI bus speed [MHz] (10 - 30)\n"
               "           20 : 20MHz [default]\n"
               " -min x  override minimum value for scaling (0 - 65535)\n"
               "           [default] automatic scaling range adjustment\n"
               "           e.g. -min 30000\n"
               " -max x  override maximum value for scaling (0 - 65535)\n"
               "           [default] automatic scaling range adjustment\n"
               "           e.g. -max 32000\n"
               " -d x    log level (0-255)\n"
               " -dms  x delay time [milliseconds] (0 - 65535)\n"
               "           [default] 0\n"
               " -ims  x interval [milliseconds] (1 - 65535)\n"
               " -c    x count (0 - 65535)\n"
               "           0 : endless\n"
               "           [default] take one shot\n"
               " -tff x  select type of file format\n"
               "           0 : CSV\n"
               "           1 : PNG [default]\n"
               "           2 : PGM\n"
               " -o x    use 'x' as basename for file\n"
               "           [default] lepton-YYYYMMDD-hhmmss.sss\n"
               "         extension depends on file format\n"
               "           CSV : .cvs\n"
               "           PNG : .png\n"
               "           PGM : .pmg\n"
               "Environment variable(s)\n"
               " LEPTON_DATA_DIR\n"
               "           make png file(s) in this directory\n"
               "", cmdname, cmdname);
	return;
}

int main( int argc, char **argv )
{
	int typeColormap = 3; // colormap_ironblack
	int typeLepton = 2; // Lepton 2.x
	int spiSpeed = 20; // SPI bus speed 20MHz
	int rangeMin = -1; //
	int rangeMax = -1; //
	int delayMs = 0; //
	int intervalMs = 0; //
	int limitTakePic = 1; //
	int count_take_pic = 0;
	int typeFileFormat = 1; // 1:PNG  2:PGM
	int segment_number0 = 0;
	int loglevel = 0;
	char *filename = NULL;
	for(int i=1; i < argc; i++) {
		if (strcmp(argv[i], "-h") == 0) {
			printUsage(argv[0]);
			exit(0);
		}
		else if (strcmp(argv[i], "-d") == 0) {
			int val = 3;
			if ((i + 1 != argc) && (strncmp(argv[i + 1], "-", 1) != 0)) {
				val = std::atoi(argv[i + 1]);
				i++;
			}
			if (0 <= val) {
				loglevel = val & 0xFF;
			}
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
		else if ((strcmp(argv[i], "-ss") == 0) && (i + 1 != argc)) {
			int val = std::atoi(argv[i + 1]);
			if ((10 <= val) && (val <= 30)) {
				spiSpeed = val;
				i++;
			}
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
		else if ((strcmp(argv[i], "-dms") == 0) && (i + 1 != argc)) {
			int val = std::atoi(argv[i + 1]);
			if ((0 <= val) && (val <= 65535)) {
				delayMs = val;
				i++;
			}
		}
		else if ((strcmp(argv[i], "-ims") == 0) && (i + 1 != argc)) {
			int val = std::atoi(argv[i + 1]);
			if ((1 <= val) && (val <= 65535)) {
				intervalMs = val;
				i++;
			}
		}
		else if ((strcmp(argv[i], "-c") == 0) && (i + 1 != argc)) {
			int val = std::atoi(argv[i + 1]);
			if ((0 <= val) && (val <= 65535)) {
				limitTakePic = val;
				i++;
			}
		}
		else if ((strcmp(argv[i], "-tff") == 0) && (i + 1 != argc)) {
			int val = std::atoi(argv[i + 1]);
			if ((0 <= val) || (val <= 2)) {
				typeFileFormat = val;
				i++;
			}
		}
		else if ((strcmp(argv[i], "-o") == 0) && (i + 1 != argc)) {
		char *val = argv[i + 1];
			if (strcmp(val, "") != 0) {
				filename = val;
				i++;
			}
		}
	}

	//create a thread to gather SPI data
	//when the thread emits updateImage, the label should update its image accordingly
	Lepton *myLepton = new Lepton();
	myLepton->setLogLevel(loglevel);
	myLepton->useColormap(typeColormap);
	myLepton->useLepton(typeLepton);
	myLepton->useSpiSpeedMhz(spiSpeed);
	myLepton->setAutomaticScalingRange();
	if (0 <= rangeMin) myLepton->useRangeMinValue(rangeMin);
	if (0 <= rangeMax) myLepton->useRangeMaxValue(rangeMax);

	//
	LeptonActionFile *leptonActionFile;
	switch (typeFileFormat) {
	case 0:
		leptonActionFile = new LeptonActionFileCsv(myLepton->getWidth(), myLepton->getHeight());
		break;
	case 2:
		leptonActionFile = new LeptonActionFilePgm(myLepton->getWidth(), myLepton->getHeight());
		break;
	default:
		leptonActionFile = new LeptonActionFilePng(myLepton->getWidth(), myLepton->getHeight());
		break;
	}
	LeptonAction *leptonAction = leptonActionFile;

	//open Lepton port
	myLepton->open();

	//
	usleep(delayMs * 1000);
	while(true) {
		//read data packets from lepton over SPI
		int segment_number = myLepton->readFrameData(leptonAction);

		if (segment_number0 + 1 == segment_number) {
			if ((typeLepton == 2) || (segment_number == 4)) {
				leptonActionFile->saveBasename(filename);
				count_take_pic++;
				if ((limitTakePic != 0) && (limitTakePic <= count_take_pic)) {
					break;
				}
				usleep(intervalMs * 1000);
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

	//finally, close Lepton just bcuz
	myLepton->close();

	return 0;
}

