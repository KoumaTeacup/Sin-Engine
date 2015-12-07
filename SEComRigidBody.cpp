#include "SEComRigidBody.h"

#include "SESin.h"
#include "SEComTransform.h"

SEComRigidBody::SEComRigidBody(std::string name, std::string tag, SEGameObject *owner) :
	SEComponent(COM_RIGIDBODY, name, tag, owner),
	mass(1.0f),
	velocity(),
	velocityDamp(0.02f),
	pendingForce() {}

SEComRigidBody::SEComRigidBody(float damp, std::string name, std::string tag, SEGameObject * owner) :
	SEComponent(COM_RIGIDBODY, name, tag, owner),
	mass(1),
	velocity(),
	velocityDamp(damp),
	pendingForce() {}

SEComRigidBody::SEComRigidBody(const SEComRigidBody &rhs):
	SEComponent(rhs),
	mass(rhs.mass),
	velocity(rhs.velocity),
	velocityDamp(rhs.velocityDamp),
	pendingForce(rhs.pendingForce) {}

SEComRigidBody& SEComRigidBody::operator=(const SEComRigidBody &rhs) {
	SEComponent::operator=(rhs);
	mass = rhs.mass;
	velocity = rhs.velocity;
	velocityDamp = rhs.velocityDamp;
	pendingForce = rhs.pendingForce;
	return *this;
}

void SEComRigidBody::applyForce(SEVector3f force) {
	pendingForce += force / mass;
}

void SEComRigidBody::resetVelocity(se_data::axis axis) {
	switch (axis) {
	case se_data::AXIS_X:
		velocity[0] = 0.0f;
		break;
	case se_data::AXIS_Y:
		velocity[1] = 0.0f;
		break;
	case se_data::AXIS_Z:
		velocity[2] = 0.0f;
		break;
	}
}

void SEComRigidBody::onUpdate() {
	float ft = SIN.getFrameTime();
	if (ft > 0.05) return;
	velocity += (SEVector3f(0.0f, -PHY_G, 0.0f) + pendingForce) * ft;
}

void SEComRigidBody::onPostUpdate() {
	float ft = SIN.getFrameTime();
	if (ft > 0.05) return;
	SE_TRANSFORM.translation() += ft * velocity;

	SEVector3f dir = velocity;
	dir.unify();
	pendingForce = (-velocity ^ velocity.absolute() * velocityDamp) - 50.0f * mass * dir;
}