#include "ufo.h"

#include "SEEvent.h"
#include "SESin.h"
#include "SEComTransform.h"
#include "SEComRigidBody.h"
#include "SEGameObject.h"

int UFO::spawnPos = 0;

UFO::UFO()
{
	spot[0] = SEVector3f(300.0f, 18.0f, 0.0f);
	spot[1] = SEVector3f(-300.0f, 18.0f, 0.0f);
	spot[2] = SEVector3f(0.0f, 18.0f, 200.0f);
	spot[3] = SEVector3f(0.0f, 18.0f, -200.0f);
}

void UFO::onInit() {
	if (spawnPos > 3) spawnPos = 0;
	SE_TRANSFORM.translation() = spot[spawnPos++];
}

void UFO::handle(SEEvent & e)
{
	if (strcmp(e.infoString, "player_position") == 0)
		playerPos = e.info.collisionDirction;
}

void UFO::onUpdate()
{
	SEVector3f dir = (playerPos - SE_TRANSFORM.translation()) / 10.0f;
	float power = dir.lengthSqaure() - 150.0f;
	if (power > 300.0f) power = 300.0f;
	SE_RIGIDBODY.resetVelocity(se_data::AXIS_Y);
	SE_RIGIDBODY.applyForce(dir.unify() * power * UFO_SPEED);
}
