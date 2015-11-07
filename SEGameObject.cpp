#include "SEGameObject.h"
#include "SEComRenderer.h"
#include "SEComTransform.h"

SEGameObject::SEGameObject() {
	for (int i = 0; i < COM_NUM; ++i)
		components[i] = NULL;
}

SEGameObject::~SEGameObject() {
	for (int i = 0; i < COM_NUM; ++i) {
		if (components.find(static_cast<componentType>(i)) != components.end()) {
			delete components[i];
			components[i] = NULL;
		}
	}
}

SEComponent& SEGameObject::operator[](componentType type) {
	if (components[type]) return *components[type];
	else switch (type) {
	case COM_RENDERER:
		components[type] = new SEComRenderer(this);
		break;
	case COM_TRANSFORM:

		components[type] = new SEComTransform(this);
		break;
	}
	return *components[type];
}