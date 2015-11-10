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
	SEComTransform &trans = SE_COMP_TRANSFORM;
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
			SE_MATRIX_ROTATE4(se_data::AXIS_Y, -trans[ry])
			SE_MATRIX_ROTATE4(se_data::AXIS_X, -trans[rx])
			SE_MATRIX_ROTATE4(se_data::AXIS_Z, -trans[rz]);
		viewTr = viewRot * SE_MATRIX_TRANSLATE4(0.0f - trans[tx], 0.0f - trans[ty], 0.0f - trans[tz]);
	}

	// Projection Matrix Construction
	float ry = tanf(SIN.toRad(viewAngle) / 2.0f);
	float rx = ry * viewPortWidth / viewPortHeight;
	projTr = SE_MATRIX_PROJECT4(frontClip, backClip, rx, ry);
}