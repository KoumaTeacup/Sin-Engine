#ifndef SEVAO_H
#define SEVAO_H

#include "GL\glew.h"

#include "SEFile.h"

namespace se_data {

class SEVAO : public SEFile{
public:
	SEVAO(std::string name, resourceType type);
	~SEVAO();

	// Inherited methods override
	bool load(const char* filename);
	void onInit();
	void onRelease();
	void onDraw();

	const char* toString() const { return SEFile::toString(); }

private:
	GLuint id, ibo;
	// True for triangles, false for quads.
	GLuint mode;
	GLsizei indicesCount;
	int* pIndices;
};

}
#endif