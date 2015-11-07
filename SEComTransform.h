#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "SEComponent.h"
#include "SEMatrix.h"

class SEComTransform : public SEComponent {
public:
	SEComTransform(SEGameObject* owner);
	SEComTransform(const SEComTransform& rhs);
	~SEComTransform() {};

	SEComTransform& operator=(const SEComTransform& rhs);
private:
	enum modelTr {
		trans = 0,
		rotate,
		scale
	};

	enum axie {
		x = 0,
		y,
		z
	};
	SEMatrix3f data;
};

#endif