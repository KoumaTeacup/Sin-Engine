#include <fstream>
#include <string>

#include "shader.h"
#include "SELog.h"

SEShader::SEShader() {
	programId = glCreateProgram();
}

bool SEShader::addShader(const char* shaderFile, GLenum shaderType) {
#ifdef SE_DEBUG
	char log[64];
	sprintf(log, "Loading shader file %s...", shaderFile);
	SE_LogManager.append(se_debug::LOGTYPE_GENERAL, log);
#endif
	char *src = readFile(shaderFile);
	const char *psrc[1] = { src };

	GLuint shaderId = glCreateShader(shaderType);
	glAttachShader(programId, shaderId);
	glShaderSource(shaderId, 1, psrc, NULL);
	glCompileShader(shaderId);
	delete src;

#ifdef SE_DEBUG
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
	SE_LogManager.append(se_debug::LOGTYPE_GENERAL, "Success.");
#endif
	shaderObjs.push_back(shaderId);
	return true;
}

bool SEShader::link() {
	glLinkProgram(programId);
#ifdef SE_DEBUG
	SE_LogManager.append(se_debug::LOGTYPE_GENERAL, "Linking shader program...");
	
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
	SE_LogManager.append(se_debug::LOGTYPE_GENERAL, "Success.");
#endif
	return true;
}

bool SEShader::validate() {
	glValidateProgram(programId);
#ifdef SE_DEBUG
	SE_LogManager.append(se_debug::LOGTYPE_GENERAL, "Validating shader program...");

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
	SE_LogManager.append(se_debug::LOGTYPE_GENERAL, "Success.");
#endif
	return true;
}

void SEShader::use() {
	glUseProgram(programId);
}

void SEShader::unuse() {
	glUseProgram(0);
}

void SEShader::load(TYPE_UNIFORM type, const char* varName, void *data) {
	GLint glLocation = glGetUniformLocation(programId, varName);
#ifdef SE_DEBUG
	if (glLocation == -1)
		SE_LogManager.append(se_debug::LOGTYPE_ERROR, "failed to query uniform variable in shader.");
#endif
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

char* SEShader::readFile(const char* filename) {
	std::ifstream ifs(filename, std::ios_base::binary);
#ifdef SE_DEBUG
	if (!ifs.is_open()) {
		char log[64];
		sprintf(log, "Failed to open file \"%s\".", filename);
		SE_LogManager.append(se_debug::LOGTYPE_ERROR, log);
	}
#endif
	ifs.seekg(0, std::ios_base::end);
	int fileLength = static_cast<int>(ifs.tellg());

	char* content = new char[fileLength + 1];
	ifs.seekg(0, std::ios_base::beg);
	ifs.read(content, fileLength);
	ifs.close();

	content[fileLength] = char(0);
	return content;
}