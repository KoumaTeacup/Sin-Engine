#include <GL\glew.h>

#include "SEComCamera.h"

#include "SESin.h"
#include "SEComTransform.h"

SEComCamera::SEComCamera(std::string name, std::string tag, SEGameObject* owner) :
	SEComponent(COM_CAMERA, name, tag, owner),
	viewPortX(0),
	viewPortY(0),
	viewPortWidth(SIN.getWindowSize()[0]),
	viewPortHeight(SIN.getWindowSize()[1]),
	viewAngle(40),
	frontClip(0.1f),
	backClip(10000.0f),
	up(SEVector3f(0.0f, 1.0f, 0.0f)){
}

SEComCamera::SEComCamera(const SEComCamera& rhs) :
	SEComponent(rhs),
	flags(rhs.flags),
	viewPortX(rhs.viewPortX),
	viewPortY(rhs.viewPortY),
	viewPortWidth(rhs.viewPortWidth),
	viewPortHeight(rhs.viewPortHeight),
	viewAngle(rhs.viewAngle),
	frontClip(rhs.frontClip),
	backClip(rhs.backClip),
	focus(rhs.focus),
	up(rhs.up){}

SEComCamera::~SEComCamera() {
}

SEComCamera& SEComCamera::operator=(const SEComCamera & rhs) {
	SEComponent::operator=(rhs);
	flags = rhs.flags;
	viewPortX = rhs.viewPortX;
	viewPortY = rhs.viewPortY;
	viewPortWidth = rhs.viewPortWidth;
	viewPortHeight = rhs.viewPortHeight;
	viewAngle = rhs.viewAngle;
	frontClip = rhs.frontClip;
	backClip = rhs.backClip;
	focus = rhs.focus;
	up = rhs.up;
	return *this;
}

void SEComCamera::preDraw() {
	// View Matrix Construction
	SEComTransform &trans = SE_TRANSFORM;
	if (flags&CAMERA_FOCUS_MODE) {
		SEVector3f camZ = focus - SEVector3f(trans[tx], trans[ty], trans[tz]);
		SEVector3f camX = up % camZ;
		SEVector3f camY = camZ % camX;
		camX.unify();
		camY.unify();
		camZ.unify();
		SEMatrix4f viewRot(
			camX[0], camX[1], camX[2], 0.0f,
			camY[0], camY[1], camY[2], 0.0f,
			camZ[0], camZ[1], camZ[2], 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
			);
		viewTr = viewRot * SE_MATRIX_TRANSLATE4(0.0f - trans[tx], 0.0f - trans[ty], 0.0f - trans[tz]);
	}
	else {
		SEMatrix4f viewRot =
			SE_MATRIX_ROTATE4(se_data::AXIS_Y, -SIN.toRad(trans[ry])) *
			SE_MATRIX_ROTATE4(se_data::AXIS_X, -SIN.toRad(trans[rx])) *
			SE_MATRIX_ROTATE4(se_data::AXIS_Z, -SIN.toRad(trans[rz]));
		viewTr = viewRot * SE_MATRIX_TRANSLATE4(0.0f - trans[tx], 0.0f - trans[ty], 0.0f - trans[tz]);
	}

	// Projection Matrix Construction
	float ry = tanf(SIN.toRad(viewAngle) / 2.0f);
	float rx = ry * viewPortWidth / viewPortHeight;
	projTr = SE_MATRIX_PROJECT4(frontClip, backClip, rx, ry);

	glViewport(viewPortX, viewPortY, viewPortWidth, viewPortHeight);
}

void SEComCamera::onResize() {
	if ((flags & CAMERA_VIEWPORT_INDEPENDENT) == 0) {
		viewPortX = 0;
		viewPortY = 0;
		viewPortWidth = SIN.getWindowSize()[0];
		viewPortHeight = SIN.getWindowSize()[1];
	}
}