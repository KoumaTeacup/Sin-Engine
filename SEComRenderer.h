#ifndef RENDERER_H
#define RENDERER_H

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