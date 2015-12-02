#ifndef CLOTHSPRING_H
#define CLOTHSPRING_H

#include "SEComListener.h"
#include "SEVector.h"

class Spring : public SEComListener {
public:
	Spring(float amp) : amplitute(amp), integrator(Euler_Cromer) {}
	~Spring() {}

	void handle(SEEvent &event);

	void onInit();
	void onUpdate();

	SEComponent *clone() const { return new Spring(*this); }

private:
	enum integratorType {
		Euler,
		Euler_Cromer,
		Verlet
	} integrator;

	float amplitute;
	SEVector3f direction;
	SEVector3f velocity;
	SEVector3f oldPosition;
};

#endif