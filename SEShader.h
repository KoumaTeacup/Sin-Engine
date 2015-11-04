#ifndef SESHADER_H
#define SHSHADER_H

#include <GL\glew.h>
#include <vector>

class SEShader {
public:
	SEShader() {};
	~SEShader() {};

	bool addShader(const char* shaderFile, GLenum shaderType);
	bool Link();
	bool Validate();
	void Use();
	void Unuse();

	GLuint id() { return programId; }

private:
	char* readFile(const char* filename);

	GLuint programId;
	std::vector<GLuint> shaderObjs;
};

#endif