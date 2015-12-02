#ifndef FREECAMERA_H
#define FREECAMERA_H

#include "SEComListener.h"

class SEComponent;

class FreeCameraController : public SEComListener {
public:
	FreeCameraController() :distantceToCenter(100.0f) {}

	void handle(SEEvent &event);

	SEComponent *clone() const { return new FreeCameraController(*this); }

private:
	float distantceToCenter;
	void onUpdate();
};

#endif