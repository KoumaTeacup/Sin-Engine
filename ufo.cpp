#include "ufo.h"

#include "SEEvent.h"
#include "SESin.h"
#include "SEComTransform.h"
#include "SEComRigidBody.h"
#include "SEGameObject.h"

int UFO::spawnPos = 0;

UFO::UFO() : hitCount(0)
{
	spot[0] = SEVector3f(300.0f, 16.0f, 0.0f);
	spot[1] = SEVector3f(-300.0f, 16.0f, 0.0f);
	spot[2] = SEVector3f(0.0f, 16.0f, 200.0f);
	spot[3] = SEVector3f(0.0f, 16.0f, -200.0f);
}

void UFO::onInit() {
	if (spawnPos > 3) spawnPos = 0;
	SE_TRANSFORM.translation() = spot[spawnPos++];
}

void UFO::handle(SEEvent & e)
{
	if (strcmp(e.infoString, "player_position") == 0) {
		playerPos = e.info.collisionDirction;
		return;
	}

	if (e.type == EVENT_COLLIDE) {
		if (e.pObjs[1] == &getOwner() && e.pObjs[0]->isTag("ufo")) {
			SE_RIGIDBODY.applyForce(e.info.collisionDirction.unify() * 1000.0f);
		}
		else if (e.pObjs[0] == &getOwner() && e.pObjs[1]->isTag("bullet") ||
			e.pObjs[1] == &getOwner() && e.pObjs[0]->isTag("bullet")) {
			e.info.collisionDirction[1] = 0.0f;
			SE_RIGIDBODY.applyForce(e.info.collisionDirction.unify() * 5000.0f);
			hitCount++;
		}
	}
}

void UFO::onUpdate()
{
	SE_TRANSFORM[ty] = 16.0f;
	SEVector3f dir = (playerPos - SE_TRANSFORM.translation()) / 10.0f;
	float power = dir.lengthSqaure() - 150.0f;
	if (power > 300.0f) power = 300.0f;
	SE_RIGIDBODY.resetVelocity(se_data::AXIS_Y);
	SE_RIGIDBODY.applyForce(dir.unify() * power * UFO_SPEED);
}
