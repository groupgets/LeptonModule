#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <linux/limits.h>
#include <png.h>
#include <png++/png.hpp>

#include "LeptonActionFilePng.h"

LeptonActionFilePng::LeptonActionFilePng()
{
}

LeptonActionFilePng::LeptonActionFilePng(int w, int h)
{
	create(w, h);
}

LeptonActionFilePng::~LeptonActionFilePng() {
}

void LeptonActionFilePng::create(int w, int h)
{
	image_png.resize(w, h);
}

char * LeptonActionFilePng::getDefaultFilename(char *filename)
{
	return LeptonActionFile::getDefaultFilename(filename);
}

char* LeptonActionFilePng::getFileExt(char *extname)
{
	sprintf(extname, ".png");
	return extname;
}

void LeptonActionFilePng::saveBasename(char *filebase)
{
	LeptonActionFile::saveBasename(filebase);
}

void LeptonActionFilePng::save()
{
	LeptonActionFile::save();
}

void LeptonActionFilePng::save(char *filename)
{
	if (filename == NULL) {
		save();
		return;
	}

	image_png.write(filename);
}

void LeptonActionFilePng::setPixel(int col, int row, int colorR, int colorG, int colorB)
{
	image_png[row][col] = png::rgb_pixel(colorR, colorG, colorB);
}
