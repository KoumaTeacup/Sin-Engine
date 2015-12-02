#include "SEComController.h"

#include "SEEvent.h"
#include "SEInput.h"
#include "SESin.h"
#include "SEComTransform.h"

void SEComController::handle(SEEvent &event) {
	switch (event.type) {
	case EVENT_KEYPRESS : break;
	case EVENT_KEYRELEASE : break;
	default:break;
	}
}

void SEComController::onUpdate() {
	SE_TRANSFORM[tx] += SIN.getJoystickPos(0, SE_JOYSTICK_X)/100;
	SE_TRANSFORM[ty] -= SIN.getJoystickPos(0, SE_JOYSTICK_Y)/100;
	SE_TRANSFORM[ry] += SIN.getJoystickPos(0, SE_JOYSTICK_Z)/300;

	//[tx] += x/100;
	//SE_TRANSFORM[ty] -= y/100;

	if (SIN.isKeyPressed(SE_KEY_LEFT))
		SE_TRANSFORM[tx] -= 0.5f;
	if (SIN.isKeyPressed(SE_KEY_RIGHT))
		SE_TRANSFORM[tx] += 0.5f;
	if (SIN.isKeyPressed(SE_KEY_UP))
		SE_TRANSFORM[ry] -= 1.0f;
	if (SIN.isKeyPressed(SE_KEY_DOWN))
		SE_TRANSFORM[ry] += 1.0f;
}