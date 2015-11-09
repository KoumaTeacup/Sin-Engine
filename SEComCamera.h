#ifndef CAMERA_H
#define CAMERA_H

#include "SEComponent.h"
#include "SEFile.h"
#include "SEMatrix.h"

#define CAMERA_BACK_CULL			0x01
#define CAMERA_WIREFRAME			0x02
#define CAMERA_FLAT_LIGHTING		0x04
#define CAMERA_MOTION_BLUR_ENABLED	0x18

class SEComCamera : public SEComponent {
public:
	SEComCamera(SEGameObject *onwer,
		std::string name = std::string(),
		std::string tag = std::string());
	SEComCamera(const SEComCamera &rhs);
	~SEComCamera();

	SEComCamera& operator =(const SEComCamera &rhs);

	// Local methods.
	void attach(const char* filename) {}
	void preDraw();

	// Public variables.
	SEMatrix4f viewTr, ProjTr;

protected:
	// Inherited pure virtuals.
	void onInit() {}
	void onRelease() {}

	// Inherited virtuals.
	void onUpdate() {}
	void onDraw() {}
	void onPostUpdate() {}
	void onPause() {}
	void onResume() {}


private:
	char flags;
	unsigned viewPortWidth, viewPortHeight;
	float viewAngle;
	float frontClip, backClip;

};

#endif