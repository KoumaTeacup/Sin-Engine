#include "SEComTransform.h"

SEComTransform::SEComTransform():SEComponent(COM_TRANSFORM) {
	data[scale][x] = data[scale][y] = data[scale][z] = 1.0f;
}