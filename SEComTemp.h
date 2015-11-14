#ifndef TEMP_H
#define TEMP_H

#include "SEComponent.h"

class SEComTemp :public SEComponent {
public:
	SEComTemp() :SEComponent(COM_UNDEFINED) {}

	SEComponent *clone() const { return NULL; }
};

#endif