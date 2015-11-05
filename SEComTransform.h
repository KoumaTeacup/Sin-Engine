#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "SEComponent.h"
#include "SEMatrix.h"

const static int trans = 0;
const static int rotate = 1;
const static int scale = 2;
const static int x = 0;
const static int y = 1;
const static int z = 2;

class SEComTransform :public SEComponent {
public:
	SEComTransform();
	~SEComTransform() {};
	SEMatrix3f data;
};

#endif