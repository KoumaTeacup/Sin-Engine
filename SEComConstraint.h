#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include <vector>

#include "SEComponent.h"
#include "SEMatrix.h"

enum constraintType {
	CONSTRAINT_AIM,
	CONSTRAINT_POINT,
	CONSTRAINT_ORIENT,
	CONSTRAINT_SCALE,
	CONSTRAINT_PARENT
};

struct constraint {
	constraintType type;
	SEGameObject *target;
	SEVector3f localDirection;

	constraint(constraintType _type, SEGameObject *_target, SEVector3f dir = SEVector3f(0.0f, 0.0f, 1.0f)):
		type(_type), target(_target), localDirection(dir) {}
	constraint(const constraint &rhs) : type(rhs.type), target(rhs.target), localDirection(rhs.localDirection) {}
	constraint& operator = (const constraint &rhs) { 
		type = rhs.type; target = rhs.target; localDirection = rhs.localDirection; return *this;
	}
};

class SEComConstraint : public SEComponent {
public:
	SEComConstraint(std::string name = std::string(),
		std::string tag = std::string(),
		SEGameObject *owner = NULL);
	SEComConstraint(const SEComConstraint &rhs);
	~SEComConstraint();

	SEComConstraint& operator =(const SEComConstraint &rhs);

	// Local methods.

	SEComponent *clone() const { return new SEComConstraint(*this); }

	// Getter & Setter
	void set(const constraint& con) { constraints.push_back(con); }
	void setLocalTransform(SEVector3f t, SEVector3f r, SEVector3f s);

	SEVector3f &localTranslation() { return localTrans; }
	SEVector3f &localRotation() { return localRot; }
	SEVector3f &localScales() { return localScale; }
	SEVector3f worldTranslation() const { return localTrans + targetTrans; }
	SEVector3f worldRotation() const { return localRot + targetRot; }
	SEVector3f worldScales() const { return SEVector3f(
		localScale[0] * targetScale[0],
		localScale[1] * targetScale[1],
		localScale[2] * targetScale[2]);
	}

protected:
	// Inherited pure virtuals.
	void onInit() {}
	void onRelease() {}

	// Inherited virtuals.
	void onUpdate();

private:

	float tolerance;
	SEVector3f targetTrans, targetRot, targetScale;
	SEVector3f localTrans, localRot, localScale;

	std::vector<constraint> constraints;
};

#endif