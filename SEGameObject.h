#ifndef SEGAMEOBJECT_H
#define SEGAMEOBJECT_H

#include <unordered_map>

#include "SEComponent.h"

class SEGameObject {
public:
	~SEGameObject() {};
private:
	std::unordered_map<componentType, SEComponent*> components;
};

#endif 