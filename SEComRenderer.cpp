#include "SEComRenderer.h"
#include "SEResource.h"
#include "SELog.h"

SEComRenderer::SEComRenderer(SEGameObject* owner) :
	SEComponent(COM_RENDERER, owner),
	shader(NULL),
	vao(NULL) {
}

SEComRenderer::SEComRenderer(const SEComRenderer& rhs) :
	SEComponent::SEComponent(rhs),
	shader(rhs.shader),
	vao(rhs.vao) {
}

SEComRenderer & SEComRenderer::operator=(const SEComRenderer & rhs){
	SEComponent::operator=(rhs);
	SE_Resource.unload(shader);
	SE_Resource.unload(vao);
	shader = rhs.shader;
	vao = rhs.vao;
	return *this;
}

void SEComRenderer::attach(const char* filename) {
	SEFile* file = SE_Resource.load(filename);
	if (!file) return;

	if (file->getType == RESTYPE_SHADER) {
		if (shader) shader->unload();
		shader = static_cast<SEShader*>(file);
	}
	else if (file->getType == RESTYPE_VERTEX_ARRAY) {
		if (vao) vao->unload();
		vao = static_cast<SEVAO*>(file);
	}

}

void SEComRenderer::onInit(){
	if (vao) vao->initVAO;
	if (shader) {
		glBindAttribLocation(shader->id, 0, "vertex");
		glBindAttribLocation(shader->id, 1, "normal");
		glBindAttribLocation(shader->id, 2, "uv");
		glBindAttribLocation(shader->id, 3, "tangent");

#ifdef SE_DEBUG
		if (!shader->link())
			SE_LogManager.append(se_debug::LOGTYPE_ERROR, "Failed to link shader program.");
#else
		shader->link();
#endif
	}
	
}

void SEComRenderer::onDraw()
{
	shader->use();
	if (vao->getMode())
		glDrawElements(GL_TRIANGLES, vao->getNumOfIndex(), GL_UNSIGNED_INT, vao->getLoc);
	else
		glDrawElements(GL_QUADS, vao->getNumOfIndex(), GL_UNSIGNED_INT, vao->getLoc);
	shader->unuse();
}