#ifndef RENDERER_H
#define RENDERER_H

#include <GL\glew.h>
#include <string>

#include "SEComponent.h"
#include "SEFile.h"

class SEComRenderer : public SEComponent {
public:
	SEComRenderer(std::string name = std::string(),
		std::string tag = std::string(),
		SEGameObject* onwer = NULL);
	SEComRenderer(const SEComRenderer& rhs);
	~SEComRenderer();

	SEComRenderer& operator=(const SEComRenderer& rhs);

	// Local methods.
	void attach(const char* filename);
	void detach();

	// Setters & Getters
	GLuint getShaderId();
	std::string getModelFilename() { return vao->getName(); }
	std::string getTextureFilename() { return texture->getName(); }

	SEComponent *clone() const { return new SEComRenderer(*this); }

protected:
	// Inherited pure virtuals.
	void onInit();
	void onRelease();

	// Inherited virtuals
	void onDraw();

private:
	SE_File shader;
	SE_File	vao;
	SE_File texture;
};
#endif