#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <linux/limits.h>

#include "LeptonActionFile.h"

LeptonActionFile::LeptonActionFile()
{
}

LeptonActionFile::LeptonActionFile(int w, int h)
{
	create(w, h);
}

LeptonActionFile::~LeptonActionFile() {
}

void LeptonActionFile::create(int w, int h)
{
}

char * LeptonActionFile::getDefaultFilename(char *filename)
{
	//
	const char *dir = getenv("LEPTON_DATA_DIR");
	if (dir == NULL) {
		dir = ".";
	}

	//
	struct timeval tv;
	gettimeofday(&tv, NULL);
	struct tm *now = localtime(&tv.tv_sec);
	sprintf(filename, "%s/lepton-%04d%02d%02d-%02d%02d%02d.%03d", dir, now->tm_year + 1900, now->tm_mon + 1, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec, (int)(tv.tv_usec/1000));

	return filename;
}

char * LeptonActionFile::getFileExt(char *extname)
{
	sprintf(extname, ".ext");
	return extname;
}

void LeptonActionFile::saveBasename(char *filebase)
{
	char filename[PATH_MAX];
	char extname[PATH_MAX];

	if (filebase == NULL) {
		save();
		return;
	}
	sprintf(filename, "%s%s", filebase, getFileExt(extname));
	save(filename);
}

void LeptonActionFile::save()
{
	char filename[PATH_MAX];
	char extname[PATH_MAX];

	sprintf(filename, "%s%s", getDefaultFilename(filename), getFileExt(extname));
	save(filename);
}

void LeptonActionFile::save(char *filename)
{
}

void LeptonActionFile::setPixel(int col, int row, int colorR, int colorG, int colorB)
{
}
