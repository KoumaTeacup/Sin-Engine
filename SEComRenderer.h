#ifndef RENDERER_H
#define RENDERER_H

#include "SEComponent.h"
#include "vao.h"
#include "shader.h"

class SEComRenderer : public SEComponent {
public:
	SEComRenderer(SEGameObject* onwer);
	SEComRenderer(const SEComRenderer& rhs);
	~SEComRenderer() {};

	SEComRenderer& operator=(const SEComRenderer& rhs);

	void attach(const char* filename);

	void onInit();
	void onDraw();
private:
	SEShader* shader;
	SEVAO*	  vao;
};
#endif