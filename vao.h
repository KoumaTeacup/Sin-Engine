#ifndef SEVAO_H
#define SEVAO_H

#include "GL\glew.h"
#include <vector>

#include "SEFile.h"

namespace se_data {

class SEVAO : public SEFile{
public:
	SEVAO(std::string name);
	~SEVAO();

	// Inherited methods override
	bool load(const char* filename);

	const char* toString() const { return SEFile::toString(); }

protected:
	void onInit();
	void onRelease();
	void onDraw();
	void onPostUpdate();

private:
	GLuint id;
	// True for triangles, false for quads.
	std::vector<unsigned> modeIndex;
	GLsizei vertexCount;
};

}
#endif