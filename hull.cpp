#include "hull.h"

#include "SEEvent.h"
#include "SESin.h"
#include "SEComTransform.h"
#include "SEComRigidBody.h"
#include "SEInput.h"
#include "SEGameObject.h"

void Hull::handle(SEEvent & e) {
	if (e.type == EVENT_COLLIDE) {
		if (e.pObjs[0] == &getOwner() && e.pObjs[1]->isTag("room") ||
			e.pObjs[1] == &getOwner() && e.pObjs[0]->isTag("room")) {
			SE_TRANSFORM.translation() += e.info.collisionDirction;
			if (e.info.collisionDirction[0] > 0.01f)
				SE_RIGIDBODY.resetVelocity(se_data::AXIS_X);
			if (e.info.collisionDirction[1] > 0.01f)
				SE_RIGIDBODY.resetVelocity(se_data::AXIS_Y);
			if (e.info.collisionDirction[2] > 0.01f)
				SE_RIGIDBODY.resetVelocity(se_data::AXIS_Z);
		}
	}
}

void Hull::onUpdate() {
	float ft = SIN.getFrameTime();
	if (ft > 0.05) return;

	if (SIN.isKeyPressed(SE_KEYBOARD::W)) {
		float radY = SIN.toRad(SE_TRANSFORM[ry]);
		SE_RIGIDBODY.applyForce(INITIAL_ENGINE_POWER * SEVector3f(-sinf(radY),  0.0f,-cosf(radY)));
	if (SIN.isKeyPressed(SE_KEYBOARD::A))
		SE_TRANSFORM[ry] += INITIAL_HULL_ROTATION_SPEED * ft;
	if (SIN.isKeyPressed(SE_KEYBOARD::D))
		SE_TRANSFORM[ry] -= INITIAL_HULL_ROTATION_SPEED * ft;
	}
	if (SIN.isKeyPressed(SE_KEYBOARD::S)) {
		float radY = SIN.toRad(SE_TRANSFORM[ry]);
		SE_RIGIDBODY.applyForce(INITIAL_ENGINE_POWER * SEVector3f(sinf(radY), 0.0f, cosf(radY)));
		if (SIN.isKeyPressed(SE_KEYBOARD::A))
			SE_TRANSFORM[ry] -= INITIAL_HULL_ROTATION_SPEED * ft;
		if (SIN.isKeyPressed(SE_KEYBOARD::D))
			SE_TRANSFORM[ry] += INITIAL_HULL_ROTATION_SPEED * ft;
	}

	SEEvent event("player_position");
	event.info.collisionDirction = SE_TRANSFORM.translation();
	SIN.broadcast(event);
}