#include <GL\glew.h>
#include <fstream>

#include "cloth.h"

#include "SESin.h"
#include "SEEvent.h"
#include "SEComRenderer.h"
#include "SEComTransform.h"

Cloth::Cloth(int fix): 
	SEComListener(),
	damp(0.0f),
	stiffness(0.2f),
	fold_resist(0.4f),
	fixedAxis(fix),
	localGravity(0.0f, -9.8f, 0.0f),
	localWind(2.0f, -10.0f, 18.0f),
	windNoise(0.3f),
	currentSolver(SOLVER_VERLET),
	iteration(1),
	selfStarted(false){
	solvers[SOLVER_EULER] = &ClothParticle::EulerSolver;
	solvers[SOLVER_VERLET] = &ClothParticle::VerletSolver;
}

void Cloth::handle(SEEvent &e)
{
	if (strcmp(e.infoString, "windDir") != 0) {
		return;
	}
	SEVector3f globalWind = e.info.collisionDirction;
	SEComTransform &comTrans = SE_TRANSFORM;

	SEVector4f globalWind4(globalWind[0], globalWind[1], globalWind[2], 0.0f);
	globalWind4 *= SE_MATRIX_ROTATE4(se_data::AXIS_Y, -comTrans[ry]);

	localWind = SEVector3f(globalWind4[0], globalWind4[1], globalWind4[2]);
}

void Cloth::onInit() {
	std::ifstream ifs(SE_RENDERER.getModelFilename().c_str());

	char buf[1024];
	SEVector3f pos;
	SEVector4i input4i;
	int placeHolder;
	std::vector<std::set<int>> neighbours;

	while (ifs.peek()!=EOF) {
		ifs >> buf;
		if (buf[0] == '#') ifs.getline(buf, 1024);
		else if (strcmp(buf, "g") == 0) ifs.getline(buf, 1024);
		else if (strcmp(buf, "s") == 0) ifs.getline(buf, 1024);
		else if (strcmp(buf, "usemtl") == 0) ifs.getline(buf, 1024);
		else if (strcmp(buf, "v") == 0) {
			ifs >> pos[0] >> pos[1] >> pos[2];
			bool fixed = false;
			if (fixedAxis == 0 && pos[0] == 0 ||
				fixedAxis == 1 && pos[1] == 0 ||
				fixedAxis == 2 && pos[2] == 0)
				fixed = true;
			particles.push_back(ClothParticle(this, pos, fixed));
			neighbours.push_back(std::set<int>());
		}
		else if (strcmp(buf, "f") == 0) {
			ifs.getline(buf, 1024);
			sscanf(buf, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",
				&input4i[0], &placeHolder, &placeHolder,
				&input4i[1], &placeHolder, &placeHolder,
				&input4i[2], &placeHolder, &placeHolder,
				&input4i[3], &placeHolder, &placeHolder);
			input4i -= SEVector4i(1, 1, 1, 1);

			addConstraint(structure, &particles[input4i[0]], &particles[input4i[1]], stiffness);
			addConstraint(structure, &particles[input4i[1]], &particles[input4i[2]], stiffness);
			addConstraint(structure, &particles[input4i[2]], &particles[input4i[3]], stiffness);
			addConstraint(structure, &particles[input4i[3]], &particles[input4i[0]], stiffness);

			addConstraint(shear, &particles[input4i[0]], &particles[input4i[2]], stiffness);
			addConstraint(shear, &particles[input4i[1]], &particles[input4i[3]], stiffness);

			neighbours[input4i[0]].insert(input4i[1]);
			neighbours[input4i[0]].insert(input4i[3]);
			neighbours[input4i[1]].insert(input4i[0]);
			neighbours[input4i[1]].insert(input4i[2]);
			neighbours[input4i[2]].insert(input4i[1]);
			neighbours[input4i[2]].insert(input4i[3]);
			neighbours[input4i[3]].insert(input4i[0]);
			neighbours[input4i[3]].insert(input4i[2]);
		}
		else ifs.getline(buf, 1024);
	}

	particleTr = std::vector<SEMatrix4f>(particles.size(), SEMatrix4f());

	for (unsigned i = 0; i < particles.size(); ++i)
		for (auto j : neighbours[i])
			for (auto k : neighbours[j]) 
				if (i != k && !checkExist(shear, &particles[i], &particles[k])) {
					addConstraint(bend, &particles[i], &particles[k], fold_resist);
					break;
				}
	
}

void Cloth::onUpdate() {
	if (windNoise - 0.0f > 0.0001f)
		srand(time(NULL));

	// If frame rate drop below 20 we skip this frame.
	if (SIN.getFrameTime() > 0.05) return;

	if (!selfStarted && currentSolver == SOLVER_VERLET) {
		for (auto &i : particles)
			i.EulerSolver();
		selfStarted = true;
	}
	else
		for (auto &i : particles)
			(i.*(solvers[currentSolver]))();

	for (int it = 0; it < iteration; ++it) {
		for (auto &i : structure) {
			i.relax();
		}

		for (auto &i : shear) {
			i.relax();
		}

		for (auto &i : bend) {
			i.relax();
		}
	}
	int count = 0;
	for (auto &i : particles) {
		particleTr[count++] = i.getTransMatrix();
	}

}

void Cloth::onDraw()
{
	int id = SE_RENDERER.getShaderId();

	glUseProgram(id);

	unsigned loc = glGetUniformLocation(id, "ClothTr");
	glUniformMatrix4fv(loc, particles.size(), GL_TRUE, &particleTr[0][0][0]);
}

bool Cloth::checkExist(std::vector<constraint>& container, ClothParticle * first, ClothParticle * second) const
{
	constraint compare_constraint(first, second);
	for (auto &i : container)
		if (i == compare_constraint)
			return true;

	return false;
}

void Cloth::addConstraint(std::vector<constraint>& container, ClothParticle * first, ClothParticle * second, float stiff)
{
	if (!checkExist(container, first, second)) {
		container.push_back(
			constraint(first, second, (first->getOrigin() - second->getOrigin()).length(), stiff)
			);
	}
}
