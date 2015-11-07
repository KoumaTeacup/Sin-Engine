#ifndef SESCENE_H
#define SESCENE_H

#include <list>

#include "SEGameObject.h"

class SEScene {
public:
	SEScene() {}
	~SEScene() {}

private:
	std::list<SEGameObject*> gameInsts;
};

#endif