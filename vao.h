#ifndef SEVAO_H
#define SEVAO_H

#include <vector>

#include "GL\glew.h"

#include "SEVector.h"

class SEVAO {
public:
	SEVAO();

	bool load(const char* filename);
	bool initVAO();

private:

	GLuint id;

	std::vector<SEVector3f> pnt;
	std::vector<SEVector3f> nrm;
	std::vector<SEVector2f> tex;
	std::vector<SEVector3f> tan;
	std::vector<SEVector3i> tri;
	std::vector<SEVector4i> quad;
};
#endif