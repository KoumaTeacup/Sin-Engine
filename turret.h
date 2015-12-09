#ifndef TURRET_H
#define TURRET_H

#include "SEComListener.h"

#define INITIAL_TURRET_ROTATION_SPEED	0.8f

class Turret : public SEComListener {
public:
	Turret(unsigned id, unsigned c) :bulletId(id), componentId(c), fireDalay(1.0f){};
	~Turret() {};

	void handle(SEEvent &e);

	SEComponent *clone() const { return new Turret(*this); }

protected:
	float fireDalay;
	unsigned bulletId;
	unsigned componentId;
	void onUpdate();
};

#endif