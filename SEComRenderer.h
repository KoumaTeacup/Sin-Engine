#ifndef RENDERER_H
#define RENDERER_H

#include "SEComponent.h"
#include "SEFile.h"

class SEComRenderer : public SEComponent {
public:
	SEComRenderer(SEGameObject* onwer,
		std::string name = std::string(),
		std::string tag = std::string());
	SEComRenderer(const SEComRenderer& rhs);
	~SEComRenderer();

	SEComRenderer& operator=(const SEComRenderer& rhs);

	// Local methods.
	void attach(const char* filename);

protected:
	// Inherited pure virtuals.
	void onInit();
	void onRelease();

	// Inherited virtuals
	void onDraw();

private:
	SE_File shader;
	SE_File	vao;
};
#endif