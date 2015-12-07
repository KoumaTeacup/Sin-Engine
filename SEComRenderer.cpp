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
	SEComponent(COM_RENDERER, name, tag, owner), alpha(0.01f) {}

SEComRenderer::SEComRenderer(float a, std::string name, std::string tag, SEGameObject* owner) :
	SEComponent(COM_RENDERER, name, tag, owner), alpha(a) {}

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
		sprintf(log, "%s > Mesh file not found, render may cancel.", name);
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
	shader->onDraw();

	se_data::SEShader &_shader = SE_Shader(shader);

	if (texture->getFile()) {
		texture->onDraw();
		// Load Texture Sampler
		unsigned unit = 1;
		_shader.setVal(UNIFORM_INT, "DiffuseMap", &unit);

		float repeat = SE_Texture(texture).getRepeat();
		_shader.setVal(UNIFORM_FLOAT, "TextureRepeat", &repeat);
		
	}

	// Load worldspace stransfromation from transform component.
	if (getOwner()[COM_TRANSFORM])
		_shader.setVal(UNIFORM_MATRIX4, "ModelTr", &SE_TRANSFORM.modelTr);
	SEComCamera* camera = SIN.getActiveCamera();
	if (camera) {
		_shader.setVal(UNIFORM_MATRIX4, "ViewTr", &camera->viewTr);
		_shader.setVal(UNIFORM_MATRIX4, "ProjTr", &camera->projTr);

		SEVector3f camPos = static_cast<SEComTransform*>(camera->getOwner()[COM_TRANSFORM])->translation();
		_shader.setVal(UNIFORM_VECTOR3, "EyePos", &camPos);
	}

	_shader.setVal(UNIFORM_FLOAT, "Alpha", &alpha);

	int numOfLight = SIN.getActiveLightsNum();
	_shader.setVal(UNIFORM_INT, "NumOfLights", &numOfLight);

	_shader.setVal(
		UNIFORM_MATRIX3,
		"LightInfo",
		&SIN.getActiveLightsInfo().front(),
		numOfLight);

	// Bind VAO&IBO -> draw model -> Unbind.
	vao->onDraw();

	vao->onPostUpdate();

	if (texture->getFile()) texture->onPostUpdate();

	shader->onPostUpdate();
}

GLuint SEComRenderer::getShaderId()
{
	if (shader->getFile()) {
		return SE_Shader(shader).getProgramId();
	}
	else return 0;
}

void SEComRenderer::TextureRepeat(float time) {
	SE_Texture(texture).setRepeat(time);
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