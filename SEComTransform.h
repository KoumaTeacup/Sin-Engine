#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "SEComponent.h"
#include "SEMatrix.h"

enum index {
	tx, ty, tz,
	rx, ry, rz,
	sx, sy, sz
};

class SEComTransform : public SEComponent {
public:
	SEMatrix4f modelTr;

	SEComTransform(std::string name = std::string(),
		std::string tag = std::string(),
		SEGameObject* owner = NULL);
	SEComTransform(SEVector3f t, SEVector3f r, SEVector3f s,
		std::string name = std::string(),
		std::string tag = std::string(),
		SEGameObject* owner = NULL);
	SEComTransform(const SEComTransform& rhs);
	~SEComTransform();

	SEComTransform& operator=(const SEComTransform& rhs);

	// Local methods.

	// Setters & Getters
	float  operator[](index i) const;
	float& operator[](index i);

	SEVector3f&			translation() { return trans; }
	const SEVector3f	translation() const { return trans; }
	SEVector3f&			rotation() { return rot; }
	const SEVector3f	rotation() const { return rot; }
	SEVector3f&			scales() { return scale; }
	const SEVector3f	scales() const {return scale; }

	SEMatrix4f translationMatrix() const;
	SEMatrix4f rotationMatrix() const;
	SEMatrix4f scaleMatrix() const;

	SEComponent *clone() const { return new SEComTransform(*this); }

protected:
	// Inherited pure virtuals.
	void onInit();
	void onRelease() {}

	// Inherited virtuals.
	void onDraw();

private:
	
	SEVector3f trans, rot, scale;

};

#endif