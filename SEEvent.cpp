#include <SFML\Window.hpp>
#include <GL\glew.h>

#include "SEEvent.h"

#include "SESin.h"
#include "SEGameObject.h"

using namespace se_system;

void SEEventManager::update(sf::Window &window) {
	// handle sfml events
	sf::Event sfEvent;
	SEEvent event;
	while (window.pollEvent(sfEvent))
	{
		switch (sfEvent.type) {
		case sf::Event::Closed:
			// end the current scene
			SIN.getActiveScene()->endScene();
			break;
		case sf::Event::Resized:
			SIN.getActiveScene()->resize();
			break;
		case sf::Event::KeyPressed:
			event.type = EVENT_KEYPRESS;
			event.key = sfEvent.key.code;
			broadcast(event);
			break;
		case sf::Event::KeyReleased:
			event.type = EVENT_KEYRELEASE;
			event.key = sfEvent.key.code;
			broadcast(event);
			break;
		case sf::Event::JoystickButtonPressed:
			break;
		case sf::Event::JoystickButtonReleased:
			break;
		default:
			break;
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

