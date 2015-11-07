#ifndef SESHADER_H
#define SHSHADER_H

#include <GL\glew.h>

#include "SEResource.h"
#include "SEMatrix.h"

enum TYPE_UNIFORM {
	UNIFORM_INT,
	UNIFORM_FLOAT,
	UNIFORM_VECTOR,
	UNIFORM_MATRIX
};

class SEShader : public SEFile {
public:
	SEShader(const char* name, resourceType type);
	~SEShader();

	// Inherited methods override
	bool load(const char* shaderFile);
	int unload();

	bool link();
	bool validate();
	void use();
	void unuse();
	void setVal(TYPE_UNIFORM type, const char* varName, void *data);

	GLuint id() { return programId; }

private:
	char* readFile(const char* filename);

	GLuint programId;

	GLuint vertShaderId, fragShaderId;
};

#endif