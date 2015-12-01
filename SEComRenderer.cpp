#include "SEComRenderer.h"

#include "SELog.h"
#include "SEResource.h"
#include "SEComCamera.h"
#include "SEGameObject.h"
#include "SEComTransform.h"
#include "shader.h"
#include "vao.h"
#include "texture.h"
#include "SESin.h"

SEComRenderer::SEComRenderer(std::string name,	std::string tag, SEGameObject* owner) :
	SEComponent(COM_RENDERER, name, tag, owner) {}

SEComRenderer::SEComRenderer(const SEComRenderer& rhs) :
	SEComponent::SEComponent(rhs),
	shader(rhs.shader),
	vao(rhs.vao),
	texture(rhs.texture){}

SEComRenderer::~SEComRenderer() {
}

SEComRenderer& SEComRenderer::operator=(const SEComRenderer & rhs) {
	SEComponent::operator=(rhs);
	if (rhs.getType() == getType()) {
		shader = rhs.shader;
		vao = rhs.vao;
		texture = rhs.texture;
	}
	return *this;
}

void SEComRenderer::onInit() {
	SEComponent::onInit();
	if (!vao->getFile()) {
#ifdef SE_DEBUG
		char log[256];
		const char* name = toString();
		sprintf(log,
			"%s > Mesh file not found, render may cancel.",	name);
		free((void*)name);
		SE_LogManager.append(se_debug::LOGTYPE_WARNNING, log);
#endif
		return;
	}
	// Using default shader if not specified by user.
	if (!shader->getFile()) {
		attach("default.vert");
		attach("default.frag");
	}

	// Bind attribute location.
	shader->onInit();
}

void SEComRenderer::onRelease() {
}

void SEComRenderer::onDraw()
{
	SEComponent::onDraw();
	if (!vao->getFile()) return;
	// Bind shader.
	if (texture->getFile()) texture->onDraw();
	shader->onDraw();

	// Load worldspace stransfromation from transform component.
	if (getOwner()[COM_TRANSFORM])
		SE_Shader(shader)->setVal(UNIFORM_MATRIX, "ModelTr", &SE_TRANSFORM.modelTr);
	SEComCamera* camera = SIN.getActiveCamera();
	if (camera) {
		SE_Shader(shader)->setVal(UNIFORM_MATRIX, "ViewTr", &camera->viewTr);
		SE_Shader(shader)->setVal(UNIFORM_MATRIX, "ProjTr", &camera->projTr);
	}

	// Load Texture Sampler
	unsigned unit = 1;
	SE_Shader(shader)->setVal(UNIFORM_INT, "DiffuseMap", &unit);

	// Bind VAO&IBO -> draw model -> Unbind.
	vao->onDraw();

	vao->onPostUpdate();
	shader->onPostUpdate();
	if (texture->getFile()) texture->onPostUpdate();
}

GLuint SEComRenderer::getShaderId()
{
	if (shader->getFile()) {
		return SE_Shader(shader)->getProgramId();
	}
	else return 0;
}

void SEComRenderer::attach(const char* filename) {
	SE_File file = SE_Resource.load(filename);
	if (!file->getFile()) return;
	if (file->getType() == se_data::RESTYPE_VERTEX_ARRAY) vao = file;
	else if (file->getType() == se_data::RESTYPE_SHADER) shader = file;
	else if (file->getType() == se_data::RESTYPE_TEXTURE) texture = file;
}

void SEComRenderer::detach() {
	shader = SE_File();
	vao = SE_File();
	texture = SE_File();
}