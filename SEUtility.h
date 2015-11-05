#ifndef SEUTILITY_H
#define SEUTILITY_H

#include "sfml\System\Clock.hpp"
#include "sfml\System\Time.hpp"

#define SE_Utility			SEUtility::getObj()
#define SE_Utility_Release	SEUtility::release

class SEUtility {
public:

	static SEUtility& getObj();
	static void release() { delete utilityManager; }

	void setFPSLimit(int limit);

	void update();
	float getFrameTime() { return frameTime; }
private:
	SEUtility();

	static SEUtility* utilityManager;

	sf::Clock clock;
	float frameTime;
	int fpsLimit;
};

#endif