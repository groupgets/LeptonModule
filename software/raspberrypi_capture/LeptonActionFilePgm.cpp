#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <linux/limits.h>

#include "LeptonActionFilePgm.h"

LeptonActionFilePgm::LeptonActionFilePgm()
{
}

LeptonActionFilePgm::LeptonActionFilePgm(int w, int h)
{
	create(w, h);
}

LeptonActionFilePgm::~LeptonActionFilePgm() {
	if (image_pgm != NULL) {
		free(image_pgm);
	}
}

void LeptonActionFilePgm::create(int w, int h)
{
	if (image_pgm != NULL) {
		free(image_pgm);
	}
	image_pgm = (uint16_t *)malloc(sizeof(uint16_t) * w * h);
	image_width = w;
	image_height = h;
}

char * LeptonActionFilePgm::getDefaultFilename(char *filename)
{
	return LeptonActionFile::getDefaultFilename(filename);
}

char* LeptonActionFilePgm::getFileExt(char *extname)
{
	sprintf(extname, ".pgm");
	return extname;
}

void LeptonActionFilePgm::saveBasename(char *filebase)
{
	LeptonActionFile::saveBasename(filebase);
}

void LeptonActionFilePgm::save()
{
	LeptonActionFile::save();
}

void LeptonActionFilePgm::save(char *filename)
{
	if (filename == NULL) {
		save();
		return;
	}

	char *magicNumber = "P2"; // PGM
	uint16_t valMin = UINT16_MAX;
	uint16_t valMax = 0;
	for (int row = 0; row < image_height; row++) {
		for (int col = 0; col < image_width; col++) {
			uint16_t val = *(image_pgm + row * image_width + col);
			if (val < valMin) {
				valMin = val;
			}
			if (valMax < val) {
				valMax = val;
			}
		}
	}

	//
	FILE *fh = fopen(filename, "w");
	fprintf(fh, "%s\n", magicNumber);
	fprintf(fh, "%d %d\n", image_width, image_height);
	fprintf(fh, "%d\n", valMax - valMin);
	for (int row = 0; row < image_height; row++) {
		for (int col = 0; col < image_width; col++) {
			uint16_t val = *(image_pgm + row * image_width + col) - valMin;
			fprintf(fh, "%d ", val);
		}
		fprintf(fh, "\n");
	}
	fclose(fh);
}

void LeptonActionFilePgm::setRawValue(int col, int row, uint16_t val)
{
	*(image_pgm + row * image_width + col) = val;
}
