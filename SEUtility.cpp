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

GLuint createShaderProgram(const char** ppFilename, GLenum* pShaderType, int n) {
	GLuint shaderProgram = glCreateProgram();
	if (shaderProgram == 0) {
		SE_LogManager.append(se_debug::LOGTYPE_OPENGL, "Failed creating shader program.");
		return 0;
	}

	std::string shaderRaw;
	int i = 0;
	while (i<n) {
		shaderRaw = std::string();
		if (!readFile(ppFilename[i], shaderRaw)) return 0;
		if (!addShader(shaderProgram, shaderRaw.c_str(), pShaderType[i])) return 0;
		++i;
	}

	GLint success = 0;
	GLchar infoLog[1024] = { 0 };

	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
		SE_LogManager.append(se_debug::LOGTYPE_OPENGL, infoLog);
		return 0;
	}
	glValidateProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
		SE_LogManager.append(se_debug::LOGTYPE_OPENGL, infoLog);
		return 0;
	}
	return shaderProgram;
}