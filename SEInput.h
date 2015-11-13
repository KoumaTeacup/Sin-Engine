#ifndef SEINPUT_H
#define SEINPUT_H

#include <SFML\Window.hpp>

#include "SESystem.h"

#define SE_KEY			sf::Keyboard::Key
#define SE_KEY_UP		sf::Keyboard::Up
#define SE_KEY_DOWN		sf::Keyboard::Down
#define SE_KEY_LEFT		sf::Keyboard::Left
#define SE_KEY_RIGHT	sf::Keyboard::Right

namespace se_system {

class SEInput : public SESystem<SEInput> {
	friend SESystem<SEInput>;

public:

	bool isKeyPressed(SE_KEY key) { return sf::Keyboard::isKeyPressed(key); }
	bool isKeyPressed(int index, int button) { return sf::Joystick::isButtonPressed(index, button); }

private:
	SEInput() : joysticks(0) {}

	int joysticks;
};

}

#endif