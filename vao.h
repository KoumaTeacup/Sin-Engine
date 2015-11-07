#ifndef SEVAO_H
#define SEVAO_H

#include <vector>

#include "GL\glew.h"

#include "SEResource.h"
#include "SEVector.h"

class SEVAO : public SEFile{
public:
	SEVAO(const char* name, resourceType type);
	~SEVAO();

	// Inherited methods override
	bool load(const char* filename);
	int unload();

	bool initVAO();

	bool getMode() { return mode; }
	int	getNumOfIndex();
	int* getLoc();

private:

	GLuint id;
	bool mode;

	std::vector<SEVector3f> pnt;
	std::vector<SEVector3f> nrm;
	std::vector<SEVector2f> tex;
	std::vector<SEVector3f> tan;
	std::vector<SEVector3i> tri;
	std::vector<SEVector4i> quad;
};
#endif