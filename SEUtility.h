#ifndef SEUTILITY_H
#define SEUTILITY_H

#include "sfml\System\Clock.hpp"
#include "sfml\System\Time.hpp"

#include "SESystem.h"

#define SE_Utility			se_system::SEUtility::getObj()
#define SE_Utility_Release	se_system::SEUtility::release()

namespace se_system{

class SEUtility :public SESystem<SEUtility>{
	friend SESystem<SEUtility>;
public:
	void setFPSLimit(int limit);

	void update();
	float getFrameTime() { return frameTime; }

private:
	SEUtility();

	sf::Clock clock;
	float frameTime;
	int fpsLimit;
};

}

#endif