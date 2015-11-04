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
	void load(TYPE_UNIFORM type, const char* varName, void *data) {
		GLint glLocation = glGetUniformLocation(programId, varName);
		if (glLocation == -1)
			SE_LogManager.append(se_debug::LOGTYPE_ERROR, "failed to query uniform variable in shader.");
		switch (type) {
		case UNIFORM_INT:
			glUniform1i(glLocation, *(int*)data);
			break;
		case UNIFORM_FLOAT:
			glUniform1f(glLocation, *(float*)data);
			break;
		case UNIFORM_VECTOR:
			glUniform4fv(glLocation, 1, &(*(SEVector4f*)data)[0]);
			break;
		case UNIFORM_MATRIX:
			glUniformMatrix4fv(glLocation, 1, GL_TRUE, &(*(SEMatrix4f*)data)[0][0]);
			break;
		}
	}

	GLuint id() { return programId; }

private:
	char* readFile(const char* filename);

	GLuint programId;
	std::vector<GLuint> shaderObjs;
};

#endif