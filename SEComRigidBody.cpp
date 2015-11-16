#include "SEComRigidBody.h"

#include "SESin.h"
#include "SEComTransform.h"

SEComRigidBody::SEComRigidBody(std::string name, std::string tag, SEGameObject *owner) :
	SEComponent(COM_RIGIDBODY, name, tag, owner),
	mass(1),
	velocity() {}

SEComRigidBody::SEComRigidBody(const SEComRigidBody &rhs):
	SEComponent(rhs),
	mass(rhs.mass),
	velocity() {}

SEComRigidBody& SEComRigidBody::operator=(const SEComRigidBody &rhs) {
	SEComponent::operator=(rhs);
	mass = rhs.mass;
	return *this;
}

void SEComRigidBody::onUpdate() {
	velocity += SEVector3f(0.0f, -PHY_G * SIN.getFrameTime(), 0.0f);
}

void SEComRigidBody::onPostUpdate() {
	SE_TRANSFORM[tx] += SIN.getFrameTime() * velocity[0];
	SE_TRANSFORM[ty] += SIN.getFrameTime() * velocity[1];
	SE_TRANSFORM[tz] += SIN.getFrameTime() * velocity[2];
}