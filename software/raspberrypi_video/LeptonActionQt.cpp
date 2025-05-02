#include <QImage>

#include "LeptonActionQt.h"

LeptonActionQt::LeptonActionQt()
{
}

LeptonActionQt::~LeptonActionQt() {
}

void LeptonActionQt::setQImage(QImage *newMyImage)
{
	myImage = newMyImage;
}

void LeptonActionQt::setPixel(int col, int row, int colorR, int colorG, int colorB)
{
	QRgb color = qRgb(colorR, colorG, colorB);
	myImage->setPixel(col, row, color);
}
