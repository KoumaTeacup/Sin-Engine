#include "SEComController.h"

#include "SEEvent.h"
#include "SEGameObject.h"
#include "SESin.h"

void SEComController::handle(SEEvent &event) {
	switch (event.type) {
	case (EVENT_KEYPRESS) : break;
	case (EVENT_KEYRELEASE) : break;
	default:break;
	}
}

void SEComController::onUpdate() {
	float x = SIN.getJoystickPos(0, SE_JOYSTICK_X);
	float y = SIN.getJoystickPos(0, SE_JOYSTICK_Y);

	SE_TRANSFORM[tx] += x/100;
	SE_TRANSFORM[ty] -= y/100;
}