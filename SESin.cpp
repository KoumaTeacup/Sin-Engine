#include <GL\glew.h>

#include "SESin.h"
#include "SEComTemp.h"

SESin::SESin():activeScene(NULL), activeCamera(NULL) {
	glSettings.depthBits = 24;
	glSettings.stencilBits = 8;
	glSettings.antialiasingLevel = 4;
	glSettings.majorVersion = 3;
	glSettings.minorVersion = 0;
}

void SESin::cleanUp() {
	// Release all global managers
#ifdef SE_DEBUG
	SE_LogManager.print();
	SE_LogManager_Release;
#endif
	SE_Utility_Release;
	SE_Resource_Release;
}

bool SESin::init(){
	//Initiaize SE_LogManager, which is necessary to redirect sf::err
	//Have to called before sf:: calls.
#ifdef SE_DEBUG
	SE_LogManager.config(SE_LOG_LOCAL | SE_LOG_ENABLED);
#endif

	// create the window
	window.create(sf::VideoMode(800, 600), "Sin Engine", sf::Style::Default, glSettings);
	window.setVerticalSyncEnabled(true);

	// Initialize glew
	GLenum res = glewInit();
#ifdef SE_DEBUG
	if (res != GLEW_OK) {
		SE_LogManager.append(se_debug::LOGTYPE_OPENGL, (const char*)glewGetErrorString(res));
		return false;
	}
#endif

	return true;
}

void SESin::begin(SEScene &scene) {
	activeScene = &scene;

	scene.init();
	// run the main loop
	while (!scene.isEnd())
	{
		// Process window/input events
		SE_EventManager.update(window);

		scene.update();

		scene.draw();
		
		window.display();

		scene.postUpdate();

		SE_Utility.update();
	}

	scene.release();
}

SEComCamera &SESin::getCamera(SEComponent * comp) const{
	SEGameObject &pObj = comp->getOwner();
#ifdef SE_DEBUG
	if (!pObj[COM_CAMERA]) {
		char log[256];
		const char *name = comp->toString();
		sprintf(log, "%s> Accessing invalid Camera component.", name);
		SE_LogManager.append(se_debug::LOGTYPE_WARNNING, log);
		free((void*)name);
		return SEComCamera(NULL);
	}
	else
#endif
		return (*static_cast<SEComCamera*>(pObj[COM_CAMERA]));
}

SEComRenderer &SESin::getRenderer(SEComponent * comp) const {
	SEGameObject &pObj = comp->getOwner();
#ifdef SE_DEBUG
	if (!pObj[COM_CAMERA]) {
		char log[256];
		const char *name = comp->toString();
		sprintf(log, "%s> Accessing invalid Renderer component.", name);
		SE_LogManager.append(se_debug::LOGTYPE_WARNNING, log);
		free((void*)name);
		return SEComRenderer(NULL);
	}
	else
#endif
		return (*static_cast<SEComRenderer*>(pObj[COM_RENDERER]));
}

SEComTransform &SESin::getTransform(SEComponent * comp) const {
	SEGameObject &pObj = comp->getOwner();
#ifdef SE_DEBUG
	if (!pObj[COM_TRANSFORM]) {
		char log[256];
		const char *name = comp->toString();
		sprintf(log, "%s> Accessing invalid Transform component.", name);
		SE_LogManager.append(se_debug::LOGTYPE_WARNNING, log);
		free((void*)name);
		return SEComTransform(NULL);
	}
	else
#endif
		return (*static_cast<SEComTransform*>(pObj[COM_TRANSFORM]));
}

SEComponent &SESin::getComponent(SEComponent * comp, int id) const {
	SEGameObject &pObj = comp->getOwner();
#ifdef SE_DEBUG
	if (pObj[id]->getType() == COM_UNDEFINED) {
		char log[256];
		const char *name = comp->toString();
		sprintf(log, "%s> Accessing invalid user defined component.", name);
		SE_LogManager.append(se_debug::LOGTYPE_WARNNING, log);
		free((void*)name);
		return SEComTemp();
	}
	else
#endif
		return (*pObj[id]);
}
