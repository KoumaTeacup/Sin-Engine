#ifndef BOT_H
#define BOT_H

#define BOT_SPEED 120.0f

#include "SEComListener.h"
#include "SEVector.h"

class Bot : public SEComListener {
public:
	unsigned hitCount;

	Bot();
	~Bot() {};

	void handle(SEEvent &e);

	SEComponent *clone() const { return new Bot(*this); }

protected:
	void onInit();
	void onUpdate();

private:
	float stateTime;
	bool state;
	SEVector3f playerPos;
	static int spawnPos;
	SEVector3f spot[4];
};

#endif