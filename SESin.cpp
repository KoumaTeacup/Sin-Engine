#include "SESin.h"

SESin* SESin::pSin = nullptr;

SESin& SESin::getObj() {
	if (!pSin) pSin = new SESin();
	return *pSin;
}

void SESin::release() {
	if (pSin)delete pSin;
	SE_LogManager_Release;
	SE_Utility_Release;
}
