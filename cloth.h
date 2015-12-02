#ifndef CLOTH_H
#define CLOTH_H

#include <vector>

#include "SEMatrix.h"
#include "SEComListener.h"

#include "particle.h"

class Cloth : public SEComListener {
	friend ClothParticle;
public:
	Cloth(int fix = 1);
	~Cloth() {}

	void handle(SEEvent &e);

	SEComponent *clone() const { return new Cloth(*this); }

protected:
	// Inherited pure virtuals.
	void onInit();
	void onRelease() {}

	void onUpdate();

private:
	// Internal methods
	bool checkExist(std::vector<constraint> &container, ClothParticle *first, ClothParticle *second) const;
	void addConstraint(std::vector<constraint> &container, ClothParticle *first, ClothParticle *second, float stiff);

	// Cloth parameters
	float damp;
	float stiffness;
	float fold_resist;

	// Solver parameters
	enum solverType {
		SOLVER_EULER,
		SOLVER_VERLET,
		SOLVER_NUM
	};
	void (ClothParticle::*solvers[SOLVER_NUM])();
	bool selfStarted;
	solverType currentSolver;
	int iteration;

	// Force parameters
	int fixedAxis;
	SEVector3f localGravity;
	SEVector3f localWind;
	float windNoise;

	// Data
	std::vector<SEMatrix4f> particleTr;
	std::vector<ClothParticle> particles;
	std::vector<constraint> structure;
	std::vector<constraint> shear;
	std::vector<constraint> bend;
};

#endif