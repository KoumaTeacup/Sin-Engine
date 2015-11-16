#include "SEComController.h"

#include "SEEvent.h"
#include "SEInput.h"
#include "SESin.h"
#include "SEComTransform.h"

void SEComController::handle(SEEvent &event) {
	switch (event.type) {
	case (EVENT_KEYPRESS) : break;
	case (EVENT_KEYRELEASE) : break;
	default:break;
	}
}

void SEComController::onUpdate() {
	//float x = SIN.getJoystickPos(0, SE_JOYSTICK_X);
	//float y = SIN.getJoystickPos(0, SE_JOYSTICK_Y);

	//SE_TRANSFORM[tx] += x/100;
	//SE_TRANSFORM[ty] -= y/100;

	if (SIN.isKeyPressed(SE_KEY_LEFT))
		SE_TRANSFORM[tx] -= 0.5f;
	if (SIN.isKeyPressed(SE_KEY_RIGHT))
		SE_TRANSFORM[tx] += 0.5f;
}