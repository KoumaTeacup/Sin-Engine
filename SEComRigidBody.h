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
	SEComRigidBody(const SEComRigidBody &rhs);
	~SEComRigidBody() {}

	SEComRigidBody& operator =(const SEComRigidBody &rhs);

	// Local methods.

	SEComponent *clone() const { return new SEComRigidBody(*this); }

protected:
	// Inherited pure virtuals.
	void onInit() {}
	void onRelease() {}

	// Inherited virtuals.
	void onUpdate();
	void onPostUpdate();

private:
	float mass;
	SEVector3f velocity;


};

#endif