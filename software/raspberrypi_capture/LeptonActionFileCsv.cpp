#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <linux/limits.h>

#include "LeptonActionFileCsv.h"

LeptonActionFileCsv::LeptonActionFileCsv()
{
}

LeptonActionFileCsv::LeptonActionFileCsv(int w, int h)
{
	create(w, h);
}

LeptonActionFileCsv::~LeptonActionFileCsv() {
	if (image_csv != NULL) {
		free(image_csv);
	}
}

void LeptonActionFileCsv::create(int w, int h)
{
	if (image_csv != NULL) {
		free(image_csv);
	}
	image_csv = (uint16_t *)malloc(sizeof(uint16_t) * w * h);
	image_width = w;
	image_height = h;
}

char * LeptonActionFileCsv::getDefaultFilename(char *filename)
{
	return LeptonActionFile::getDefaultFilename(filename);
}

char* LeptonActionFileCsv::getFileExt(char *extname)
{
	sprintf(extname, ".csv");
	return extname;
}

void LeptonActionFileCsv::saveBasename(char *filebase)
{
	LeptonActionFile::saveBasename(filebase);
}

void LeptonActionFileCsv::save()
{
	LeptonActionFile::save();
}

void LeptonActionFileCsv::save(char *filename)
{
	if (filename == NULL) {
		save();
		return;
	}

	uint16_t valMin = UINT16_MAX;
	uint16_t valMax = 0;
	for (int row = 0; row < image_height; row++) {
		for (int col = 0; col < image_width; col++) {
			uint16_t val = *(image_csv + row * image_width + col);
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
	for (int row = 0; row < image_height; row++) {
		for (int col = 0; col < image_width; col++) {
			uint16_t val = *(image_csv + row * image_width + col);
			fprintf(fh, "%d ", val);
			if (col != image_width - 1) {
				fprintf(fh, ",");
			}
		}
		fprintf(fh, "\n");
	}
	fclose(fh);
}

void LeptonActionFileCsv::setRawValue(int col, int row, uint16_t val)
{
	*(image_csv + row * image_width + col) = val;
}
