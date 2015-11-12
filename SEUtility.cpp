#include <GL\glew.h>

#include "SEUtility.h"
#include "SELog.h"

using namespace se_system;

SEUtility::SEUtility() :fpsLimit(0), frameTime(1.0f) {
#ifdef SE_DEBUG
	SE_LogManager.append(se_debug::LOGTYPE_GENERAL, "utilityManager Initialized.");
#endif
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

	// Clear framebuffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void SEUtility::setFPSLimit(int limit) {
#ifdef SE_DEBUG
	char log[32];
	sprintf(log, "FPS limit set to %d", limit);
	SE_LogManager.append(se_debug::LOGTYPE_GENERAL, log);
#endif
	fpsLimit = limit; 
}
