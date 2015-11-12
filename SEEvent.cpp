#include <SFML\Window.hpp>
#include <GL\glew.h>

#include "SEEvent.h"

#include "SESin.h"
#include "SEGameObject.h"

using namespace se_system;

void SEEventManager::update(sf::Window &window) {
	// handle sfml events
	sf::Event sfEvent;
	while (window.pollEvent(sfEvent))
	{
		if (sfEvent.type == sf::Event::Closed)
		{
			// end the current scene
			SIN.getActiveScene()->endScene();
		}
		else if (sfEvent.type == sf::Event::Resized)
		{
			broadcast(SEEvent(EVENT_CAMERA, 0.0f));
		}
	}

	auto i = eventQueue.begin();
	while (i != eventQueue.end()){
		i->delay -= SE_Utility.getFrameTime();
		if (i->delay <= 0.0f) {
			SIN.getActiveScene()->handle(*i);
			i = eventQueue.erase(i);
		}
		else ++i;
	}
}

