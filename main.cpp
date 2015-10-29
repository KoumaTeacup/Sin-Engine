//main.cpp

#include <GL/glew.h>
#include <SFML/Window.hpp>

#include "SELog.h"
#include "SEVector.h"
#include "SEMatrix.h"
#include "SEUtility.h"

int main()
{
	//Initiaize SE_LogManager, which is necessary to redirect sf::err
	//Have to called before sf:: calls.
	SE_LogManager.init(SE_LOG_LOCAL|SE_LOG_ENABLED);

	// create the window
	sf::ContextSettings glSettings;
	glSettings.depthBits = 24;
	glSettings.stencilBits = 8;
	glSettings.antialiasingLevel = 4;
	glSettings.majorVersion = 3;
	glSettings.minorVersion = 0;
	sf::Window window(sf::VideoMode(800, 600), "Sin Engine", sf::Style::Default, glSettings);
	window.setVerticalSyncEnabled(true);

	GLenum res = glewInit();
	if (res != GLEW_OK) {
		SE_LogManager.append(se_debug::LOGTYPE_OPENGL, (const char*)glewGetErrorString(res));
		return 1;
	}

	const char* shaders[2] = { "shader.vert", "shader.frag" };
	GLenum shaderTypes[2] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };
	GLuint shaderProgram = createShaderProgram(shaders, shaderTypes, 2);
	if (!shaderProgram)
		SE_LogManager.append(se_debug::LOGTYPE_ERROR, "Filed to create shader program.");

	glUseProgram(shaderProgram);

	SEVector3f vertices[8];
	vertices[0].initData(-1.0f, -1.0f, -1.0f);
	vertices[1].initData(-1.0f, -1.0f, 1.0f);
	vertices[2].initData(-1.0f, 1.0f, -1.0f);
	vertices[3].initData(-1.0f, 1.0f, 1.0f);
	vertices[4].initData(1.0f, -1.0f, -1.0f);
	vertices[5].initData(1.0f, -1.0f, 1.0f);
	vertices[6].initData(1.0f, 1.0f, -1.0f);
	vertices[7].initData(1.0f, 1.0f, 1.0f);
	// load resources, initialize the OpenGL states, ...
	GLuint VBO;
	GLuint IBO;

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	int index[36] = {
		0,1,2,
		1,2,3,
		0,2,4,
		2,4,6,
		2,3,7,
		2,6,7,
		1,3,7,
		1,5,7,
		0,1,5,
		0,4,5,
		4,5,7,
		4,6,7
	};
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);

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

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glDrawElements(GL_TRIANGLES, sizeof(index), GL_UNSIGNED_INT, 0);

		glDisableVertexAttribArray(0);

		// draw...
		SE_LogManager.print();
		// end the current frame (internally swaps the front and back buffers)
		window.display();
	}

	// release resources...
	SE_LogManager.print();
	SE_LogManager_Release();

	return 0;
}