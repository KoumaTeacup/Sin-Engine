#include "turret.h"

#include "bullet.h"
#include "SESin.h"
#include "SEEvent.h"
#include "SEVector.h"
#include "SEComTransform.h"
#include "SEInput.h"
#include "SEScene.h"

void Turret::handle(SEEvent & e) {
	if (e.type == EVENT_KEYPRESS &&
		e.info.key.code == SE_KEYBOARD::Space) {
		if (fireDalay < 1.0f) return;
		unsigned inst = SIN.getActiveScene()->instantiate(bulletId);
		SEGameObject *pObj = SIN.getActiveScene()->getInst(inst);
		Bullet *ptr = static_cast<Bullet*>((*pObj)[componentId]);
		SEComTransform *comTran = static_cast<SEComTransform*>((*pObj)[COM_TRANSFORM]);
		comTran->translation() = SE_TRANSFORM.translation();
		comTran->rotation() = SE_TRANSFORM.rotation();
		float rotY = SIN.toRad(SE_TRANSFORM.rotation()[1]);
		SEVector3f dir(-sinf(rotY), 0.0f, -cosf(rotY));
		ptr->direction = dir;

		SEEvent e("bulletDie");
		e.delay = 1.0f;
		e.info.userInt = inst;
		SIN.broadcast(e);

		fireDalay = 0.0f;

		return;
	}

	if (strcmp(e.infoString, "bulletDie") == 0) {
		SIN.getActiveScene()->destroy(e.info.userInt);
		return;
	}
}

void Turret::onUpdate() {
	if (fireDalay <1.0f) fireDalay += SIN.getFrameTime();
	SEVector2ui uPos = SIN.getMousePosition();
	SEVector2ui uSize = SIN.getWindowSize();
	if (uPos[0] > uSize[0] || uPos[1] > uSize[1]) return;

	SEVector2f pos((float)uPos[0], (float)uPos[1]);
	SEVector2f size(uSize[0]/2.0f, uSize[1] * 0.6f);
	SEVector2f dir = (pos - size) ^ SEVector2f(1.0f, 2.4f);
	SEVector2f origin = SEVector2f(0.0f, -1.0f);
	float dot = dir * origin;
	float angle = SIN.toDegree(acosf(dot * dot / origin.lengthSqaure() / dir.lengthSqaure()));
	if (dir[1] > 0) angle = 180.0f - angle;
	if (dir[0] > 0) angle *= -1;

	float &rotY = SE_TRANSFORM[ry];
	if (rotY - angle > 180.0f ||  angle - rotY < 180.0f && angle-rotY > 0.0f) {
		rotY += INITIAL_TURRET_ROTATION_SPEED;
	}
	else {
		rotY -= INITIAL_TURRET_ROTATION_SPEED;
	}
	if (rotY > 180.0f) rotY -= 360.0f;
	if (rotY < -180.0f) rotY += 360.0f;
}
