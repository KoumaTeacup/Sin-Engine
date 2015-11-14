#ifndef SESHADER_H
#define SESHADER_H

#include <GL\glew.h>

#include "SEFile.h"

#define SE_Shader(x) static_cast<se_data::SEShader*>((x)->getFile())


enum TYPE_UNIFORM {
	UNIFORM_INT,
	UNIFORM_FLOAT,
	UNIFORM_VECTOR,
	UNIFORM_MATRIX
};

namespace se_data{
class SEShader : public SEFile {
public:
	SEShader(std::string name, resourceType type);
	~SEShader();

	// Inherited methods override
	bool load(const char* shaderFile);
	void onInit();
	void onRelease();
	void onDraw();

	bool link()		const;
	bool validate() const;
	void setVal(TYPE_UNIFORM type, const char* varName, void *data);

	const char*	toString() const { return SEFile::toString(); }

private:
	GLuint programId;
	GLuint vertShaderId, fragShaderId;

	char* readFile(const char* filename) const;
};

}

#endif