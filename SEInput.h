#ifndef SEINPUT_H
#define SEINPUT_H

#include <SFML\Window.hpp>

#include "SESystem.h"

typedef sf::Keyboard::Key	SE_KEY;
typedef sf::Joystick::Axis	SE_Axis;

//-------------------------------------------
// Keyboard enum mapping
#define SE_KEY_UP		sf::Keyboard::Up
#define SE_KEY_DOWN		sf::Keyboard::Down
#define SE_KEY_LEFT		sf::Keyboard::Left
#define SE_KEY_RIGHT	sf::Keyboard::Right
//-------------------------------------------
// JoyStick enum mapping
// left stick
#define SE_JOYSTICK_X	sf::Joystick::X
#define SE_JOYSTICK_Y	sf::Joystick::Y
// right stick
#define SE_JOYSTICK_Z	sf::Joystick::Z
#define SE_JOYSTICK_R	sf::Joystick::R
// third stick if exist
#define SE_JOYSTICK_U	sf::Joystick::U
#define SE_JOYSTICK_V	sf::Joystick::V

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