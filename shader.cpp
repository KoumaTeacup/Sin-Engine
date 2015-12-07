#include <fstream>
#include <string>

#include "shader.h"

#include "SELog.h"
#include "SEMatrix.h"

using namespace se_data;

SEShader::SEShader(std::string name) :
	SEFile(name, RESTYPE_SHADER),
	programId(glCreateProgram()),
	vertShaderId(0),
	fragShaderId(0),
	linked(false) {
}

SEShader::~SEShader() {
	glDeleteProgram(programId);
}

bool SEShader::load(const char* shaderFile) {

	// Determine the shader type.
	GLuint shaderId;
	const char* ext = strrchr(shaderFile, '.') + 1;
	if (strcmp(ext,"vert") == 0) {
		if (vertShaderId) return true;
		shaderId = vertShaderId = glCreateShader(GL_VERTEX_SHADER);
	}
	else if (strcmp(ext,"frag") == 0) {
		if (fragShaderId) return true;
		shaderId = fragShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	}

#ifdef SE_DEBUG
	char log[64];
	sprintf(log, "Loading shader file %s...", shaderFile);
	SE_LogManager.append(se_debug::LOGTYPE_GENERAL, log);
#endif
	// Read file.
	char *src = readFile(shaderFile);
	if (!src) return false;
	// Create shader and attach to the program.
	glAttachShader(programId, shaderId);
	glShaderSource(shaderId, 1, &src, NULL);
	glCompileShader(shaderId);
	delete src;

	// Check for error.
#ifdef SE_DEBUG
	GLint info;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &info);
	if (info != 1) {
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &info);
		GLchar *logBuffer = new char[info];
		glGetShaderInfoLog(shaderId, info, NULL, logBuffer);
		SE_LogManager.append(se_debug::LOGTYPE_CONTINUE, "Failed.");
		SE_LogManager.append(se_debug::LOGTYPE_ERROR, "Shader compilation error.");
		SE_LogManager.append(se_debug::LOGTYPE_OPENGL, logBuffer);
		delete logBuffer;
		return false;
	}
	SE_LogManager.append(se_debug::LOGTYPE_CONTINUE, "Success.");
#endif
	return true;
}

void SEShader::onInit() {
	if (linked) return;
	glBindAttribLocation(programId, 0, "id");
	glBindAttribLocation(programId, 1, "vertex");
	glBindAttribLocation(programId, 2, "normal");
	glBindAttribLocation(programId, 3, "uv");
	glBindAttribLocation(programId, 4, "tangent");

#ifdef SE_DEBUG
	if (!link())
		SE_LogManager.append(se_debug::LOGTYPE_ERROR, "Failed to link shader program.");
	if (!validate())
		SE_LogManager.append(se_debug::LOGTYPE_ERROR, "Shader validation failed.");
#else
	link();
#endif
	linked = true;
}

void SEShader::onRelease() {
}

void SEShader::onDraw() {
	glUseProgram(programId);
}

void SEShader::onPostUpdate() {
	// Unbind shader.
	glUseProgram(0);
}

bool SEShader::link() const {
	glLinkProgram(programId);
#ifdef SE_DEBUG
	SE_LogManager.append(se_debug::LOGTYPE_GENERAL, "Linking shader program...");
	
	GLint info;
	glGetProgramiv(programId, GL_LINK_STATUS, &info);
	if (info != 1) {
		glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &info);
		GLchar *logBuffer = new char[info];
		glGetProgramInfoLog(programId, info, NULL, logBuffer);
		SE_LogManager.append(se_debug::LOGTYPE_CONTINUE, "Failed.");
		SE_LogManager.append(se_debug::LOGTYPE_ERROR, "Shader program link error.");
		SE_LogManager.append(se_debug::LOGTYPE_OPENGL, logBuffer);
		delete logBuffer;
		return false;
	}
	SE_LogManager.append(se_debug::LOGTYPE_CONTINUE, "Success.");
#endif
	return true;
}

bool SEShader::validate() const {
	glValidateProgram(programId);
#ifdef SE_DEBUG
	SE_LogManager.append(se_debug::LOGTYPE_GENERAL, "Validating shader program...");

	GLint info;
	glGetProgramiv(programId, GL_VALIDATE_STATUS, &info);
	if (info != 1) {
		glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &info);
		GLchar *logBuffer = new char[info];
		glGetProgramInfoLog(programId, info, NULL, logBuffer);
		SE_LogManager.append(se_debug::LOGTYPE_CONTINUE, "Failed.");
		SE_LogManager.append(se_debug::LOGTYPE_ERROR, "Shader program validation error.");
		SE_LogManager.append(se_debug::LOGTYPE_OPENGL, logBuffer);
		delete logBuffer;
		return false;
	}
	SE_LogManager.append(se_debug::LOGTYPE_CONTINUE, "Success.");
#endif
	return true;
}

void SEShader::setVal(TYPE_UNIFORM type, const char* varName, const void *data, int num) const {
	GLint glLocation = glGetUniformLocation(programId, varName);

	switch (type) {
	case UNIFORM_INT:
		glUniform1i(glLocation, *(int*)data);
		break;
	case UNIFORM_FLOAT:
		glUniform1f(glLocation, *(float*)data);
		break;
	case UNIFORM_VECTOR3:
		glUniform3fv(glLocation, num, &(*(SEVector3f*)data)[0]);
		break;
	case UNIFORM_VECTOR4:
		glUniform4fv(glLocation, num, &(*(SEVector4f*)data)[0]);
		break;
	case UNIFORM_MATRIX3:
		glUniformMatrix3fv(glLocation, num, GL_FALSE, &(*(SEMatrix3f*)data)[0][0]);
		break;
	case UNIFORM_MATRIX4:
		glUniformMatrix4fv(glLocation, num, GL_TRUE, &(*(SEMatrix4f*)data)[0][0]);
		break;
	}
}

char* SEShader::readFile(const char* filename) const {
	std::ifstream ifs(filename, std::ios_base::binary);
#ifdef SE_DEBUG
	if (!ifs.is_open()) {
		char log[64];
		sprintf(log, "Failed to open file \"%s\".", filename);
		SE_LogManager.append(se_debug::LOGTYPE_ERROR, log);
		return NULL;
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