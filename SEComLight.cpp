#include "SEComLight.h"

SEComLight::SEComLight(std::string name, std::string tag, SEGameObject * owner):
	SEComponent(COM_LIGHT, name, tag, owner),
	color(1.0f,1.0f,1.0f),
	intensity(2000.0f),
	decay(LIGHT_QUADRATIC),
	type(LIGHT_POINT) {}

SEComLight::SEComLight(const SEComLight & rhs):
	SEComponent(rhs),
	color(rhs.color),
	intensity(rhs.intensity),
	decay(rhs.decay),
	type(rhs.type) {}

SEComLight::~SEComLight() {
}

SEComLight & SEComLight::operator=(const SEComLight & rhs) {
	SEComponent::operator=(rhs);
	color = rhs.color;
	intensity = rhs.intensity;
	decay = rhs.decay;
	type = rhs.type;
	return *this;
}

SEVector3f SEComLight::getDecay() const
{
	switch (decay) {
	case LIGHT_NONE:
		return SEVector3f(0.0f, 0.0f, 0.0f);
	case LIGHT_LINER:
		return SEVector3f(1.0f, 0.0f, 0.0f);
	case LIGHT_QUADRATIC:
		return SEVector3f(1.0f, 1.0f, 0.0f);
	case LIGHT_CUBIC:
		return SEVector3f(1.0f, 1.0f, 1.0f);
	default:
		return SEVector3f(0.0f, 0.0f, 0.0f);
	}
}
