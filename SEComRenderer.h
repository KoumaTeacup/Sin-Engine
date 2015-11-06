#ifndef RENDERER_H
#define RENDERER_H

#include "SEComponent.h"
#include "vao.h"
#include "shader.h"

class SEComRenderer : public SEComponent {
public:
	SEComRenderer();
	~SEComRenderer() {};

	bool attachShader(const char* shaderFile, GLenum shaderType);
private:
	SEShader	shader;
	SEVAO		vao;
};
#endif