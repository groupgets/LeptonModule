#include <stdint.h>
#include "LeptonAction.h"

LeptonAction::LeptonAction()
{
}

LeptonAction::~LeptonAction() {
}

void LeptonAction::setRawValue(int col, int row, uint16_t val)
{
	// override this method
}

void LeptonAction::setPixel(int col, int row, int colorR, int colorG, int colorB)
{
	// override this method
}

