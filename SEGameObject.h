#ifndef SEGAMEOBJECT_H
#define SEGAMEOBJECT_H

#include <unordered_map>

#include "SEComponent.h"

class SEGameObject {
public:
	SEGameObject();
	~SEGameObject();

	SEComponent& operator[](componentType type);
private:
	std::unordered_map<componentType, SEComponent*> components;
};

#endif 