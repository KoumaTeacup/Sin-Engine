#include "SEComConstraint.h"

#include "SEComTransform.h"
#include "SEGameObject.h"
#include "SELog.h"
#include "SESin.h"

SEComConstraint::SEComConstraint(std::string name, std::string tag, SEGameObject * owner):
	SEComponent(COM_CONSTRAINT, name, tag, owner),
	tolerance(0.0f),
	targetScale(1.0f,1.0f,1.0f),
	localScale(1.0f,1.0f,1.0f) {}

SEComConstraint::SEComConstraint(const SEComConstraint & rhs):
	SEComponent(rhs), 
	tolerance(rhs.tolerance),
	targetTrans(rhs.targetTrans),
	targetRot(rhs.targetRot),
	targetScale(rhs.targetScale),
	localTrans(rhs.localTrans),
	localRot(rhs.localRot),
	localScale(rhs.localScale),
	constraints(rhs.constraints) {}

SEComConstraint::~SEComConstraint() {
}

SEComConstraint & SEComConstraint::operator=(const SEComConstraint & rhs)
{
	SEComponent::operator=(rhs);
	tolerance = rhs.tolerance;
	targetTrans = rhs.targetTrans;
	targetRot = rhs.targetRot;
	targetScale = rhs.targetScale;
	localTrans = rhs.localTrans;
	localRot = rhs.localRot;
	localScale = rhs.localScale;
	constraints = rhs.constraints;
	return *this;
}

void SEComConstraint::setLocalTransform(SEVector3f t, SEVector3f r, SEVector3f s) {
	localTrans = t;
	localRot = r;
	localScale = s;
}

void SEComConstraint::onUpdate() {
	SEComTransform &comTran = SE_TRANSFORM;

	// Update local space change.
	localTrans = comTran.translation() - targetTrans;
	localRot = comTran.rotation() - targetRot;
	localScale = comTran.scales() - targetScale;

	// Update target space change.
	SEComTransform *targetTransform;
	targetTrans = targetRot = SEVector3f();
	targetScale = SEVector3f(1.0f, 1.0f, 1.0f);
	for (auto &i : constraints) {
		targetTransform = static_cast<SEComTransform*>((*i.target)[COM_TRANSFORM]);
		if (i.type == CONSTRAINT_POINT || i.type == CONSTRAINT_PARENT)
			targetTrans = targetTrans + targetTransform->translation();
		if (i.type == CONSTRAINT_ORIENT || i.type == CONSTRAINT_PARENT)
			targetRot = targetRot + targetTransform->rotation();
		if (i.type == CONSTRAINT_SCALE || i.type == CONSTRAINT_PARENT) {
			const SEVector3f scale = targetTransform->scales();
			targetScale[0] *= scale[0];
			targetScale[1] *= scale[1];
			targetScale[2] *= scale[2];
		}

		// Attention: Gimbal Lock May Happen!
		if (i.type == CONSTRAINT_AIM) {
			SEVector3f oldTrans = targetTransform->translation() - comTran.translation();
			SEVector3f vec0(i.localDirection[0], 0.0f, i.localDirection[2]);
			SEVector3f vec1(oldTrans[0], 0.0f, oldTrans[2]);
			float dot = vec0 * vec1;
			float angle = SIN.toDegree(acosf(dot * dot / vec0.lengthSqaure() / vec1.lengthSqaure()));
			if ((vec0 % vec1)[1] < 0) angle *= -1;
			targetRot += SEVector3f(0.0f, angle, 0.0f);
			
			vec0[0] = 0.0f;
			vec0[1] = i.localDirection[1];
			vec1[0] = 0.0f;
			vec1[1] = oldTrans[1];
			dot = vec0 * vec1;
			angle = SIN.toDegree(acosf(dot * dot / vec0.lengthSqaure() * vec1.lengthSqaure()));
			if ((vec0 % vec1)[1] < 0) angle *= -1;
			targetRot += SEVector3f(angle, 0.0f, 0.0f);
		}
	}

	// Update Transform component.
	comTran.translation() = targetTrans + localTrans;
	comTran.rotation() = targetRot + localRot;
	comTran.scales() = targetScale + localScale;
}
