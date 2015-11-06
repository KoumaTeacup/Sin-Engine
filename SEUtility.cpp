#include "SEUtility.h"
#include "SELog.h"

SEUtility* SEUtility::utilityManager = nullptr;

SEUtility::SEUtility() :fpsLimit(0), frameTime(1.0f) {
#ifdef SE_DEBUG
	SE_LogManager.append(se_debug::LOGTYPE_GENERAL, "utilityManager Initialized.");
#endif
}

SEUtility& SEUtility::getObj() {
	if (!utilityManager) utilityManager = new SEUtility();
	return *utilityManager;
}

void SEUtility::update() {
#ifdef SE_DEBUG
	SE_LogManager.print();
#endif
	frameTime = clock.restart().asSeconds();
	if (fpsLimit > 0) {
		float spf = 1.0f / fpsLimit;
		while (frameTime < spf)
			frameTime += clock.restart().asSeconds();
	}
}

void SEUtility::setFPSLimit(int limit) {
#ifdef SE_DEBUG
	char log[32];
	sprintf(log, "FPS limit set to %d", limit);
	SE_LogManager.append(se_debug::LOGTYPE_GENERAL, log);
#endif
	fpsLimit = limit; 
}
