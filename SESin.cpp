#include <GL\glew.h>


#include "SESin.h"

SESin* SESin::pSin = nullptr;

SESin::SESin():activeScene(NULL), activeCamera(NULL) {
	glSettings.depthBits = 24;
	glSettings.stencilBits = 8;
	glSettings.antialiasingLevel = 4;
	glSettings.majorVersion = 3;
	glSettings.minorVersion = 0;
}

SESin& SESin::getObj() {
	if (!pSin) pSin = new SESin();
	return *pSin;
}

void SESin::release() {
	// Release all global managers
#ifdef SE_DEBUG
	SE_LogManager.print();
	SE_LogManager_Release;
#endif
	SE_Utility_Release;
	SE_Resource_Release;

	if (pSin)delete pSin;
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
		scene.update();
		// handle events
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				// end the program
				scene.endScene();
			}
			else if (event.type == sf::Event::Resized)
			{
				// adjust the viewport when the window is resized
				glViewport(0, 0, event.size.width, event.size.height);
			}
		}

		// draw...
		scene.draw();

		// end the current frame (internally swaps the front and back buffers)
		window.display();

		scene.postUpdate();

		SE_Utility.update();
	}

	scene.release();
}