//main.cpp

#include <GL/glew.h>
#include <SFML/Window.hpp>

#include "SELog.h"
#include "SEVector.h"
#include "SEMatrix.h"
#include "SEShader.h"
#include "SEUtility.h"


int main()
{
	//Initiaize SE_LogManager, which is necessary to redirect sf::err
	//Have to called before sf:: calls.
#ifdef SE_DEBUG
	SE_LogManager.init(SE_LOG_LOCAL|SE_LOG_ENABLED);
#endif

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
#ifdef SE_DEBUG
	if (res != GLEW_OK) {
		SE_LogManager.append(se_debug::LOGTYPE_OPENGL, (const char*)glewGetErrorString(res));
		return 1;
	}
#endif

	SEShader testShader;
	testShader.addShader("shader.vert", GL_VERTEX_SHADER);
	testShader.addShader("shader.frag", GL_FRAGMENT_SHADER);
	testShader.link();

	SEMatrix4f PVM =
		SE_MATRIX_PROJECT4(1000.0f, 0.1f, 0.4f, 0.3f)*
		SE_MATRIX_TRANSLATE4(1.0f, 1.0f, -10.0f)*
		SE_MATRIX_SCALE4(0.5f, 0.5f, 1.0f)*
		SE_MATRIX_ROTATE4(se_data::AXIS_X, 50)
		//SE_MATRIX_ROTATE4(se_data::AXIS_Y, 30)
		;
	float m[4][4] = {
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	testShader.load(UNIFORM_MATRIX, "PVM", &PVM);

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

	SE_Utility.setFPSLimit(60);

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
		testShader.use();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

		PVM *= SE_MATRIX_ROTATE4(se_data::AXIS_Y, 0.01f)
			*SE_MATRIX_ROTATE4(se_data::AXIS_Z, 0.05f);
		testShader.load(UNIFORM_MATRIX, "PVM", &PVM);

		glDrawElements(GL_TRIANGLES, sizeof(index), GL_UNSIGNED_INT, 0);

		glDisableVertexAttribArray(0);

		testShader.unuse();

		// draw...
		// end the current frame (internally swaps the front and back buffers)
		SE_Utility.update();

		window.display();
	}

	// release resources...
#ifdef SE_DEBUG
	SE_LogManager.print();
	SE_LogManager_Release();
#endif
	SE_Utility_Release();

	return 0;
}