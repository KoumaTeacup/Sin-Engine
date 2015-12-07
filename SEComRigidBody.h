#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "SEComponent.h"
#include "SEVector.h"

#define PHY_G	9.80665f

class SEComRigidBody : public SEComponent {
public:
	SEComRigidBody(std::string name = std::string(),
		std::string tag = std::string(),
		SEGameObject *owner = NULL);
	SEComRigidBody(float damp, std::string name = std::string(),
		std::string tag = std::string(),
		SEGameObject *owner = NULL);
	SEComRigidBody(const SEComRigidBody &rhs);
	~SEComRigidBody() {}

	SEComRigidBody& operator =(const SEComRigidBody &rhs);

	// Local methods.

	void applyForce(SEVector3f force);
	void resetVelocity() { velocity = SEVector3f(); }
	void resetVelocity(se_data::axis axis);

	SEComponent *clone() const { return new SEComRigidBody(*this); }


protected:
	// Inherited pure virtuals.
	void onInit() {}
	void onRelease() {}

	// Inherited virtuals.
	void onUpdate();
	void onPostUpdate();

private:
	float velocityDamp;
	SEVector3f pendingForce;
	float mass;
	SEVector3f velocity;


};

#endif