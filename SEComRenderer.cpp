#include "SEComRenderer.h"

SEComRenderer::SEComRenderer() :SEComponent(COM_RENDERER) {
	
}

bool SEComRenderer::attachShader(const char* shaderFile, GLenum shaderType) {
	return true;
}