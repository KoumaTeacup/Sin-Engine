#ifndef LIGHT_H
#define LIGHT_H

#include "SEComponent.h"
#include "SEVector.h"

class SEComLight : public SEComponent {
public:
	SEComLight(std::string name = std::string(),
		std::string tag = std::string(),
		SEGameObject *owner = NULL);
	SEComLight(const SEComLight &rhs);
	~SEComLight();

	SEComLight& operator =(const SEComLight &rhs);

	// Local methods.

	SEComponent *clone() const { return new SEComLight(*this); }

	// Getter & Setter
	SEVector3f getColor() const { return color * intensity; }
	SEVector3f getDecay() const;

protected:
	// Inherited pure virtuals.
	void onInit() {}
	void onRelease() {}

	// Inherited virtuals.

private:
	SEVector3f color;
	float intensity;
	enum decayRate {
		LIGHT_NONE, LIGHT_LINER, LIGHT_QUADRATIC, LIGHT_CUBIC
	} decay;
	enum lightType {
		LIGHT_AMBIENT, LIGHT_POINT, LIGHT_SPOT, LIGHT_AREA, LIGHT_DIRECTIONAL
	} type;

};

#endif