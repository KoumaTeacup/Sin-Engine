#ifndef HULL_H
#define HULL_H

#include "SEComListener.h"

#define INITIAL_HULL_ROTATION_SPEED	30.0f
#define INITIAL_ENGINE_POWER		100.0f
#define INITIAL_BREAK_SPEED			1.0f

class Hull : public SEComListener {
public:
	Hull() {};
	~Hull() {};

	void handle(SEEvent &e);

	SEComponent *clone() const { return new Hull(*this); }

protected:
	void onUpdate();
};

#endif