#include <fstream>
#include <string>

#include "SEShader.h"
#include "SELog.h"

SEShader::SEShader() {
	programId = glCreateProgram();
}

bool SEShader::addShader(const char* shaderFile, GLenum shaderType) {
	char *src = readFile(shaderFile);
	const char *psrc[1] = { src };

	GLuint shaderId = glCreateShader(shaderType);
	glAttachShader(programId, shaderId);
	glShaderSource(shaderId, 1, psrc, NULL);
	glCompileShader(shaderId);
	delete src;

	GLint info;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &info);
	if (info != 1) {
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &info);
		GLchar *logBuffer = new char[info];
		glGetShaderInfoLog(shaderId, info, NULL, logBuffer);
		SE_LogManager.append(se_debug::LOGTYPE_ERROR, "Shader compilation error.");
		SE_LogManager.append(se_debug::LOGTYPE_OPENGL, logBuffer);
		delete logBuffer;
		return false;
	}

	shaderObjs.push_back(shaderId);
	return true;
}

bool SEShader::link() {
	glLinkProgram(programId);
	
	GLint info;
	glGetProgramiv(programId, GL_LINK_STATUS, &info);
	if (info != 1) {
		glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &info);
		GLchar *logBuffer = new char[info];
		glGetProgramInfoLog(programId, info, NULL, logBuffer);
		SE_LogManager.append(se_debug::LOGTYPE_ERROR, "Shader program link error.");
		SE_LogManager.append(se_debug::LOGTYPE_OPENGL, logBuffer);
		delete logBuffer;
		return false;
	}
	return true;
}

bool SEShader::validate() {
	glValidateProgram(programId);

	GLint info;
	glGetProgramiv(programId, GL_VALIDATE_STATUS, &info);
	if (info != 1) {
		glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &info);
		GLchar *logBuffer = new char[info];
		glGetProgramInfoLog(programId, info, NULL, logBuffer);
		SE_LogManager.append(se_debug::LOGTYPE_ERROR, "Shader program validation error.");
		SE_LogManager.append(se_debug::LOGTYPE_OPENGL, logBuffer);
		delete logBuffer;
		return false;
	}
	return true;
}

void SEShader::use() {
	glUseProgram(programId);
}

void SEShader::unuse() {
	glUseProgram(0);
}

char* SEShader::readFile(const char* filename) {
	std::ifstream ifs(filename, std::ios_base::binary);
	if (!ifs.is_open()) {
		std::string errorStr("Failed to open file \"");
		errorStr += filename;
		errorStr += "\".";
		SE_LogManager.append(se_debug::LOGTYPE_ERROR, errorStr.c_str());
	}
	ifs.seekg(0, std::ios_base::end);
	int fileLength = static_cast<int>(ifs.tellg());

	char* content = new char[fileLength + 1];
	ifs.seekg(0, std::ios_base::beg);
	ifs.read(content, fileLength);
	ifs.close();

	content[fileLength] = char(0);
	return content;
}