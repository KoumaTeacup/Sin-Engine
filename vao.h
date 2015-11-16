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

	const char* toString() const { return SEFile::toString(); }

protected:
	void onInit();
	void onRelease();
	void onDraw();

private:
	GLuint id, ibo;
	// True for triangles, false for quads.
	GLuint mode;
	GLsizei indicesCount;
};

}
#endif