#include <GL\glew.h>


#include "SESin.h"

SESin* SESin::pSin = nullptr;

SESin::SESin() {
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
	
	scene.update();
	// run the main loop
	bool running = true;
	while (running)
	{
		scene.update();
		// handle events
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				// end the program
				running = false;
			}
			else if (event.type == sf::Event::Resized)
			{
				// adjust the viewport when the window is resized
				glViewport(0, 0, event.size.width, event.size.height);
			}
		}

		// clear the buffers
		//testShader.use();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/*glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

		PVM *= SE_MATRIX_ROTATE4(se_data::AXIS_Y, 0.01f)
			*SE_MATRIX_ROTATE4(se_data::AXIS_Z, 0.05f);
		testShader.setVal(UNIFORM_MATRIX, "PVM", &PVM);

		glDrawElements(GL_TRIANGLES, sizeof(index), GL_UNSIGNED_INT, 0);

		glDisableVertexAttribArray(0);

		testShader.unuse();*/

		// draw...
		// end the current frame (internally swaps the front and back buffers)
		scene.draw();

		window.display();

		SE_Utility.update();
	}
}

void SESin::end(SEScene &scene) {
	scene.release();
}