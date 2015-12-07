#ifndef SESHADER_H
#define SESHADER_H

#include <GL\glew.h>

#include "SEFile.h"

enum TYPE_UNIFORM {
	UNIFORM_INT,
	UNIFORM_FLOAT,
	UNIFORM_VECTOR3,
	UNIFORM_VECTOR4,
	UNIFORM_MATRIX3,
	UNIFORM_MATRIX4
};

namespace se_data{
class SEShader : public SEFile {
public:
	SEShader(std::string name);
	~SEShader();

	// Inherited methods override
	bool load(const char* shaderFile);

	bool link()		const;
	bool validate() const;
	GLuint getProgramId() const { return programId; }
	void setVal(TYPE_UNIFORM type, const char* varName, const void *data, int num = 1) const;

	const char*	toString() const { return SEFile::toString(); }

protected:
	void onInit();
	void onRelease();
	void onDraw();
	void onPostUpdate();

private:
	GLuint programId;
	GLuint vertShaderId, fragShaderId;
	bool linked;

	char* readFile(const char* filename) const;
};

}

#endif