#include <iostream>
#include <fstream>

#include "SEUtility.h"
#include "SELog.h"

bool readFile(const char* filename, std::string &output) {
	std::ifstream f(filename);

	if (f.is_open()) {
		std::string line;
		while (getline(f, line)) {
			output.append(line);
			output.append("\n");
		}
		f.close();
	}
	else {
		std::string error(filename);
		error = "Failed to open file: " + error + ".";
		SE_LogManager.append(se_debug::LOGTYPE_ERROR, error.c_str());
		return false;
	}
	return true;
}

bool addShader(GLuint shaderProgram, const char* pShaderText, GLenum shaderType) {
	GLuint shaderObj = glCreateShader(shaderType);

	if (shaderObj == 0) {
		SE_LogManager.append(se_debug::LOGTYPE_OPENGL, "Failed to create shader object.");
		return false;
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint lengths[1];
	lengths[0] = strlen(pShaderText);
	glShaderSource(shaderObj, 1, p, lengths);
	glCompileShader(shaderObj);
	GLint success;
	glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &success);

	if (!success) {
		GLchar infoLog[1024];
		glGetShaderInfoLog(shaderObj, 1024, NULL, infoLog);
		SE_LogManager.append(se_debug::LOGTYPE_OPENGL, infoLog);
		return false;
	}

	glAttachShader(shaderProgram, shaderObj);
	return true;
}