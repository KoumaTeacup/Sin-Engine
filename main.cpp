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

	GLuint shaderProgram = glCreateProgram();
	if (shaderProgram == 0) {
		SE_LogManager.append(se_debug::LOGTYPE_OPENGL, "Failed creating shader program.");
		return 1;
	}

	std::string vs, fs;
	if (!readFile("shader.vert", vs)) return 1;
	if (!readFile("shader.frag", fs)) return 1;

	if (!addShader(shaderProgram, vs.c_str(), GL_VERTEX_SHADER)) return 1;
	if (!addShader(shaderProgram, fs.c_str(), GL_FRAGMENT_SHADER)) return 1;

	GLint success = 0;
	GLchar infoLog[1024] = { 0 };

	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
		SE_LogManager.append(se_debug::LOGTYPE_OPENGL, infoLog);
		return 1;
	}
	glValidateProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
		SE_LogManager.append(se_debug::LOGTYPE_OPENGL, infoLog);
		return 1;
	}

	glUseProgram(shaderProgram);

	SEVector3f vertices[3];
	vertices[0].initData(-1.0f, -1.0f);
	vertices[1].initData(1.0f, -1.0f);
	vertices[2].initData(0.0f, 1.0f);
	// load resources, initialize the OpenGL states, ...
	GLuint VBO;

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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

		glDrawArrays(GL_TRIANGLES, 0, 3);

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