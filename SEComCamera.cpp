#include <GL\glew.h>
#include <SFML\Window\Window.hpp>

#include "SEComCamera.h"

#include "SESin.h"

SEComCamera::SEComCamera(SEGameObject* owner, std::string name, std::string tag) :
	SEComponent(COM_CAMERA, owner, name, tag),
	viewPortWidth(SIN.getWindowSize()[0]),
	viewPortHeight(SIN.getWindowSize()[1]),
	viewAngle(40),
	frontClip(0.1f),
	backClip(1000.0f){
}

SEComCamera::SEComCamera(const SEComCamera& rhs) :
	SEComponent(rhs) {}

SEComCamera::~SEComCamera() {
}

SEComCamera& SEComCamera::operator=(const SEComCamera & rhs) {
	return static_cast<SEComCamera&>(SEComponent::operator=(rhs));
}

void SEComCamera::preDraw() {
	// View Matrix Construction
	viewTr.identify();

	// Projection Matrix Construction
	float ry = sinf(viewAngle / 2.0f);
	float rx = ry * viewPortWidth / viewPortHeight;
	ProjTr = SE_MATRIX_PROJECT4(frontClip, backClip, rx, ry);
}