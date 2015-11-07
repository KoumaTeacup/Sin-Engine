#include "SEComTransform.h"

SEComTransform::SEComTransform(SEGameObject* owner):SEComponent(COM_TRANSFORM, owner) {
	data[scale][x] = data[scale][y] = data[scale][z] = 1.0f;
}

SEComTransform::SEComTransform(const SEComTransform & rhs) :
	SEComponent::SEComponent(rhs),
	data(rhs.data) {
}

SEComTransform & SEComTransform::operator=(const SEComTransform & rhs)
{
	SEComponent::operator=(rhs);
	data = rhs.data;
	return *this;
}
