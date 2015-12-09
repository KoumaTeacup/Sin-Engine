#ifndef UFO_H
#define UFO_H

#define UFO_SPEED 1.0f

#include "SEComListener.h"
#include "SEVector.h"

class UFO : public SEComListener {
public:
	unsigned hitCount;

	UFO();
	~UFO() {};

	void handle(SEEvent &e);

	SEComponent *clone() const { return new UFO(*this); }

protected:
	void onInit();
	void onUpdate();

private:
	SEVector3f playerPos;
	static int spawnPos;
	SEVector3f spot[4];
};

#endif