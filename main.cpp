//main.cpp

#include <GL/glew.h>
#include <SFML/Window.hpp>

int main()
{
	// create the window
	sf::ContextSettings glSettings;
	glSettings.depthBits = 32;
	glSettings.stencilBits = 8;
	glSettings.antialiasingLevel = 4;
	glSettings.majorVersion = 3;
	glSettings.minorVersion = 0;
	sf::Window window(sf::VideoMode(800, 600), "Sin Engine", sf::Style::Default, glSettings);
	window.setVerticalSyncEnabled(true);

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

		// end the current frame (internally swaps the front and back buffers)
		window.display();
	}

	// release resources...

	return 0;
}