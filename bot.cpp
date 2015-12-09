#include "bot.h"

#include "SEEvent.h"
#include "SESin.h"
#include "SEComTransform.h"
#include "SEComRigidBody.h"
#include "SEGameObject.h"

int Bot::spawnPos = 0;

Bot::Bot() :state(true), stateTime(0.0f), hitCount(0)
{
	spot[0] = SEVector3f(100.0f, 0.0f, 100.0f);
	spot[1] = SEVector3f(100.0f, 0.0f, -100.0f);
	spot[2] = SEVector3f(-100.0f, 0.0f, 100.0f);
	spot[3] = SEVector3f(-100.0f, 0.0f, -100.0f);
}

void Bot::onInit() {
	if (spawnPos > 3) spawnPos = 0;
	SE_TRANSFORM.translation() = spot[spawnPos++];
}

void Bot::handle(SEEvent & e)
{
	if (strcmp(e.infoString, "player_position") == 0) {
		playerPos = e.info.collisionDirction;
		return;
	}

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
		else if (e.pObjs[1] == &getOwner() && e.pObjs[0]->isTag("bot")) {
				SE_RIGIDBODY.applyForce(e.info.collisionDirction.unify() * 1000.0f);
			}
		else if (e.pObjs[0] == &getOwner() && e.pObjs[1]->isTag("bullet") ||
			e.pObjs[1] == &getOwner() && e.pObjs[0]->isTag("bullet")) {
			e.info.collisionDirction[1] = 0.0f;
			SE_RIGIDBODY.applyForce(e.info.collisionDirction.unify() * 2500.0f);
			hitCount++;
		}
	}
}

void Bot::onUpdate()
{
	SEVector3f dir = (playerPos - SE_TRANSFORM.translation()) / 10.0f;

	float power = (dir.lengthSqaure() - 150.0f) / 2.0f;
	if (power > 90) power = 90.0f;
	dir[1] = 0.0f;
	SEVector4f rot(dir[0], dir[1], dir[2], 0.0f);
	rot *= SE_MATRIX_ROTATE4(se_data::AXIS_Y, 90.0f - power);
	dir[0] = rot[0]; dir[2] = rot[2];
	SE_RIGIDBODY.applyForce(dir.unify() * BOT_SPEED);
}
