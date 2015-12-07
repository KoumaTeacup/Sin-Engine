#ifndef PARTICLE_H
#define PARTICLE_H

#include <set>
#include "SEMatrix.h"

class Cloth;
class ClothParticle;

struct constraint {

	ClothParticle *p1, *p2;
	float restLength;
	float stiffness;
	constraint(ClothParticle *_p1 = NULL, ClothParticle *_p2 = NULL, float rest = 0.0f, float stiff = 1.0f):
		p1(_p1), p2(_p2), restLength(rest), stiffness(stiff) {}
	bool operator==(const constraint &rhs) const;
	
	void relax() const;
};

class ClothParticle {
	friend constraint;

public:
	ClothParticle(Cloth *c, SEVector3f pos, bool f = false) :
		pCloth(c), origin(pos), position(pos), fixed(f) {}
	~ClothParticle() {}

	void VerletSolver();
	void EulerSolver();
	SEMatrix4f getTransMatrix() const;

	SEVector3f getOrigin() const { return origin; }
	SEVector3f getPos() const { return position; }
	void fix() { fixed = true; }

private:
	bool fixed;
	Cloth *pCloth;
	SEVector3f position;
	SEVector3f oldPosition;
	SEVector3f velocity;
	SEVector3f origin;
};

#endif