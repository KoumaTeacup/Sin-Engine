#ifndef BULLET_H
#define BULLET_H

#define BULLET_SPEED 300.0f

#include "SEComListener.h"
#include "SEVector.h"

class Bullet : public SEComListener {
public:
	SEVector3f direction;

	Bullet();
	~Bullet() {};

	void handle(SEEvent &e);

	SEComponent *clone() const { return new Bullet(*this); }

protected:
	void onInit();
	void onUpdate();

};

#endif