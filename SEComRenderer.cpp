#include "SEComRenderer.h"
#include "SEResource.h"
#include "SEGameObject.h"
#include "SEComTransform.h"
#include "SELog.h"
#include "shader.h"
#include "SEMatrix.h"
#include "SESin.h"

SEComRenderer::SEComRenderer(SEGameObject* owner, std::string name,	std::string tag) :
	SEComponent(COM_RENDERER, owner, name, tag),
	shader(NULL),
	vao(NULL) {}

SEComRenderer::SEComRenderer(const SEComRenderer& rhs) :
	SEComponent::SEComponent(rhs),
	shader(rhs.shader),
	vao(rhs.vao) {}

SEComRenderer::~SEComRenderer() {
}

SEComRenderer& SEComRenderer::operator=(const SEComRenderer & rhs) {
	SEComponent::operator=(rhs);
	if (rhs.getType() == getType()) {
		shader = rhs.shader;
		vao = rhs.vao;
	}
	return *this;
}

void SEComRenderer::onInit() {
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
	// Bind vertex array.
	vao->onInit();

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

	// Load worldspace stransfromation from transform component.
	if (getOwner()[COM_TRANSFORM])
		SE_Shader(shader)->setVal(UNIFORM_MATRIX, "ModelTr", &SE_COMP_TRANSFORM.modelTr);
	SEComCamera* camera = SIN.getActiveCamera();
	if (camera) {
		SE_Shader(shader)->setVal(UNIFORM_MATRIX, "ViewTr", &camera->viewTr);
		SE_Shader(shader)->setVal(UNIFORM_MATRIX, "ProjTr", &camera->projTr);
	}

	// Bind VAO&IBO -> draw model -> Unbind.
	vao->onDraw();
}

void SEComRenderer::attach(const char* filename) {
	SE_File file = SE_Resource.load(filename);
	if (file->getType() == se_data::RESTYPE_VERTEX_ARRAY) vao = file;
	else if (file->getType() == se_data::RESTYPE_SHADER) shader = file;
}