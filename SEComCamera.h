#ifndef CAMERA_H
#define CAMERA_H

#include "SEComponent.h"
#include "SEFile.h"
#include "SEMatrix.h"

#define CAMERA_FOCUS_MODE			0x01
#define CAMERA_BACK_CULL			0x02
#define CAMERA_WIREFRAME			0x04
#define CAMERA_FLAT_LIGHTING		0x08
#define CAMERA_MOTION_BLUR_ENABLED	0x10

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
	void setFocus(SEVector3f f) { focus = f; }
	void setUp(SEVector3f u) { up = u; }

	// Public variables.
	SEMatrix4f viewTr, projTr;

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
	SEVector3f focus, up;

};

#endif