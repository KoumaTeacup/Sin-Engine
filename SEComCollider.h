#ifndef COLLIDER_H
#define COLLIDER_H

#include <set>

#include "SEComponent.h"

#include "SEMatrix.h"

struct SECollider {
	enum shapeType {
		none,
		point,
		line,
		plane,
		sphere,
		AABB,
		box
	} type;

	union colliderShape {
		SEVector3f normal;
		SEVector3f lhw;
		float radius;

		colliderShape() { memset(this, 0, sizeof(colliderShape)); }
		colliderShape(const colliderShape &rhs) { memcpy(this, &rhs, sizeof(colliderShape)); }
		colliderShape& operator=(const colliderShape& rhs) {
			memcpy(this, &rhs, sizeof(colliderShape));
		}
	} shape;

	SEVector3f offset;
	float sqRadius;

	// methods
	SECollider() : type(none), sqRadius(0.0f), shape() {}
	SECollider(const SECollider &rhs);
	bool operator<(const SECollider &rhs) const { return this < &rhs; }
	SECollider& calculateSqRadius();
	bool isPointInAABB(SEVector3f point) const;
	bool isAABBInAABB(SEVector3f dis, SEVector3f lhw) const;
	bool isAABBInSphere(SEVector3f dis, SEVector3f lhw) const;
	bool isPointInBox(SEVector3f point, SEMatrix4f tranM) const;
	bool isAABBIntersectWithPlane(SEVector3f normal, SEVector3f dis) const;
	bool isBoxIntersectWithPlane(SEVector4f normal, SEVector4f dis, SEMatrix4f rotM) const;
};

class SEComCollider : public SEComponent {
public:
	SEComCollider(std::string name = std::string(),
		std::string tag = std::string(),
		SEGameObject *owner = NULL);
	SEComCollider(const SEComCollider &rhs);
	~SEComCollider() {}

	SEComCollider& operator =(const SEComCollider &rhs);

	// Local methods.

	void set(SECollider col) { colliders.insert(col.calculateSqRadius()); }
	bool testWith(const SEComCollider *col) const;

	SEComponent *clone() const { return new SEComCollider(*this); }

protected:
	// Inherited pure virtuals.
	void onInit() {}
	void onRelease() {}

	// Inherited virtuals.
	void onUpdate();
	void onDraw(){}
	void onPostUpdate(){}
	void onPause(){}
	void onResume(){}

private:

	std::set<SECollider> colliders;
};

#endif