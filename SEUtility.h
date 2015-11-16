#ifndef SEUTILITY_H
#define SEUTILITY_H

#include "sfml\System\Clock.hpp"
#include "sfml\System\Time.hpp"

#include "SESystem.h"

enum componentType;

class SEComponent;

#define SE_Utility			se_system::SEUtility::getObj()
#define SE_Utility_Release	se_system::SEUtility::release()

namespace se_system{

class SEUtility :public SESystem<SEUtility>{
	friend SESystem<SEUtility>;
public:
	void setFPSLimit(int limit);

	void update();
	float getFrameTime() const { return frameTime; }

	void typeToString(char *str, componentType type) const;
	SEComponent *typeToPointer(componentType type,
		std::string name = std::string(),
		std::string tag = std::string()) const;

private:
	SEUtility();

	sf::Clock clock;
	float frameTime;
	int fpsLimit;
};

}

#endif