#include <GL\glew.h>


#include "SESin.h"

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