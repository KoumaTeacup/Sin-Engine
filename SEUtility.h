#ifndef SEUTILITY_H
#define SEUTILITY_H

#include <string>
#include <GL\glew.h>

bool readFile(const char* filename, std::string &output);

bool addShader(GLuint shaderProgram, const char* pShaderText, GLenum shaderType);

#endif