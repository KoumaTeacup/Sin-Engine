#include "particle.h"
#include "SESin.h"
#include "cloth.h"

void ClothParticle::VerletSolver()
{
	if (fixed) return;

	float ft = SIN.getFrameTime();
	// Calculate Acceleration
	// Currently we only consider gravity and wind
	// To do: collision handling

	float noise;
	SEVector3f acceleration;
	SEVector3f windFiled = pCloth->localWind;
	
	if (pCloth->windNoise - 0.0f > 0.0001f) {
		noise = (rand() * 2 / (float)RAND_MAX - 1.0f) * pCloth->windNoise;
		windFiled[0] += windFiled[0] * noise;
		noise = (rand() * 2 / (float)RAND_MAX - 1.0f) * pCloth->windNoise;
		windFiled[1] += windFiled[1] * noise;
		noise = (rand() * 2 / (float)RAND_MAX - 1.0f) * pCloth->windNoise;
		windFiled[2] += windFiled[2] * noise;
	}

	acceleration += pCloth->localGravity + windFiled;

	// Using Verlet method to update the postion of particles.
	SEVector3f temp = oldPosition;
	oldPosition = position;

	position += (position - temp) * (1 - pCloth->damp) + acceleration * ft * ft;
}

void ClothParticle::EulerSolver() {
	if (fixed) return;

	float ft = SIN.getFrameTime();

	// Calculate Acceleration
	// Currently we only consider gravity and wind
	// To do: collision handling

	SEVector3f acceleration;
	acceleration += pCloth->localGravity + pCloth->localWind;

	// Using Euler method to update the postion of particles.
	oldPosition = position;
	velocity += acceleration * ft * (1-pCloth->damp);
	position += velocity * ft;
}

SEMatrix4f ClothParticle::getTransMatrix() const
{
	SEVector3f transform = position - origin;
	return SE_MATRIX_TRANSLATE4(transform[0], transform[1], transform[2]);
}

bool constraint::operator==(const constraint & rhs) const
{
	return p1 == rhs.p1 && p2 == rhs.p2 || p1 == rhs.p2 && p2 == rhs.p1;
}

void constraint::relax() const
{
	SEVector3f delta = p1->position - p2->position;
	delta *= restLength * restLength / (delta*delta + restLength*restLength) - 0.5f;
	if (!p1->fixed) p1->position += delta * stiffness;
	if (!p2->fixed) p2->position -= delta * stiffness;
}
