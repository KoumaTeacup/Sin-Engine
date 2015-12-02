#include "SEEvent.h"

#include "SESin.h"
#include "SEScene.h"
#include "SEUtility.h"

using namespace se_system;

void SEEventManager::updateWindowEvent(sf::Window &window) {
	// handle sfml events
	sf::Event sfEvent;
	SEEvent event;
	while (window.pollEvent(sfEvent))
	{
		switch (sfEvent.type) {
		case sf::Event::Closed:
			// end the current scene
			SIN.endScene();
			break;
		case sf::Event::Resized:
			SIN.getActiveScene()->resize();
			break;
		case sf::Event::MouseMoved:
			event.type = EVENT_MOUSEMOVE;
			event.info.mouseMove.pos.x = sfEvent.mouseMove.x;
			event.info.mouseMove.pos.y = sfEvent.mouseMove.y;
			if (oldMouseX < 0 || oldMouseY < 0) {
				oldMouseX = event.info.mouseMove.pos.x;
				oldMouseY = event.info.mouseMove.pos.y;
			}
			event.info.mouseMove.delta.x = event.info.mouseMove.pos.x - oldMouseX;
			event.info.mouseMove.delta.y = event.info.mouseMove.pos.y - oldMouseY;
			oldMouseX = event.info.mouseMove.pos.x;
			oldMouseY = event.info.mouseMove.pos.y;

			broadcast(event);
			break;
		case sf::Event::KeyPressed:
			event.type = EVENT_KEYPRESS;
			event.info.key = sfEvent.key;
			broadcast(event);
			break;
		case sf::Event::KeyReleased:
			event.type = EVENT_KEYRELEASE;
			event.info.key = sfEvent.key;
			broadcast(event);
			break;
		case sf::Event::JoystickButtonPressed:
			event.type = EVENT_BUTTONPRESS;
			event.info.button = sfEvent.joystickButton;
			break;
		case sf::Event::JoystickButtonReleased:
			event.type = EVENT_BUTTONRELEASE;
			event.info.button = sfEvent.joystickButton;
			break;
		case sf::Event::JoystickMoved:
			event.type = EVENT_MOVE;
			event.info.move = sfEvent.joystickMove;
		default:
			break;
		}
	}
}

void SEEventManager::distribute() {
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

