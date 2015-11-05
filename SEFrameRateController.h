#ifndef SEFRAMERATECONTROLLER_H
#define SEFRAMERATECONTROLLER_H

#include "sfml\System\Clock.hpp"
#include "sfml\System\Time.hpp"

class SEFrameRateController {
public:
	SEFrameRateController() {};
	~SEFrameRateController() {};

	update() { frameTime = clock.restart(); }

	getFrameTime() { return frameTime; }
private:
	sf::Clock clock;
	float frameTime;
};

#endif