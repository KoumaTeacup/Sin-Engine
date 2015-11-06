#ifndef SESHADER_H
#define SHSHADER_H

#include <GL\glew.h>
#include <vector>

#include "SEMatrix.h"

enum TYPE_UNIFORM {
	UNIFORM_INT,
	UNIFORM_FLOAT,
	UNIFORM_VECTOR,
	UNIFORM_MATRIX
};

class SEShader {
public:
	SEShader();
	~SEShader() {};

	bool addShader(const char* shaderFile, GLenum shaderType);
	bool link();
	bool validate();
	void use();
	void unuse();
	void load(TYPE_UNIFORM type, const char* varName, void *data);

	GLuint id() { return programId; }

private:
	char* readFile(const char* filename);

	GLuint programId;
	std::vector<GLuint> shaderObjs;
};

#endif