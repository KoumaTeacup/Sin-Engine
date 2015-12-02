#ifndef SEINPUT_H
#define SEINPUT_H

#include <SFML\Window.hpp>

#include "SESystem.h"

typedef sf::Keyboard::Key	SE_KEY;
typedef sf::Joystick::Axis	SE_Axis;

//-------------------------------------------
// Keyboard enum mapping
#define SE_KEYBOARD	sf::Keyboard
//-------------------------------------------
// JoyStick enum mappings
#define SE_JOYSTICK sf::Joystick

#define SE_InputManager	se_system::SEInput::getObj()

namespace se_system {

class SEInput : public SESystem<SEInput> {
	friend SESystem<SEInput>;

public:

	bool isKeyPressed(SE_KEY key) { return sf::Keyboard::isKeyPressed(key); }
	bool isKeyPressed(int index, int button) { return sf::Joystick::isButtonPressed(index, button); }
	float getJoystickPos(int index, SE_Axis axis) { return sf::Joystick::getAxisPosition(index, axis); }

private:
	SEInput() : joysticks(0) {}

	int joysticks;
};

}

#endif