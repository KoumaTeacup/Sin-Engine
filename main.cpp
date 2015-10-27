//main.cpp

#include <GL/glew.h>
#include <SFML/Window.hpp>

#include "SELog.h"

int main()
{
	//Initiaize SE_LogManager, which is necessary to redirect sf::err
	SE_LogManager.init(SE_LOG_LOCAL);

	// create the window
	sf::ContextSettings glSettings;
	glSettings.depthBits = 32;
	glSettings.stencilBits = 8;
	glSettings.antialiasingLevel = 4;
	glSettings.majorVersion = 3;
	glSettings.minorVersion = 0;
	sf::Window window(sf::VideoMode(800, 600), "Sin Engine", sf::Style::Default, glSettings);
	window.setVerticalSyncEnabled(true);

	glewInit();
	// load resources, initialize the OpenGL states, ...
	GLuint VBO;
	glGenBuffers(1, &VBO);

	// run the main loop
	bool running = true;
	while (running)
	{
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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// draw...
		SE_LogManager.print();
		// end the current frame (internally swaps the front and back buffers)
		window.display();
	}

	// release resources...
	SE_LogManager_Release();

	return 0;
}