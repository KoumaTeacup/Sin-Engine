#ifndef SEEVENT_H
#define SEEVENT_H

#include <SFML\Window.hpp>

#include <list>
#include <vector>

#include "SESystem.h"
#include "SEInput.h"

#define SE_EventManager	se_system::SEEventManager::getObj()
#define SE_Broadcast	se_system::SEEventManager::getObj().broadcast

class SEGameObject;

enum eventType {
	EVENT_DEFAULT,
	EVENT_KEYPRESS,
	EVENT_KEYRELEASE,
	EVENT_BUTTONPRESS,
	EVENT_BUTTONRELEASE,
	EVENT_MOVE,
	EVENT_NUM
};

struct SEEvent {
	eventType type;
	float delay;
	int numOfObjects;
	SEGameObject **pObjs;

	union {
		sf::Event::KeyEvent				key;
		sf::Event::JoystickButtonEvent	button;
		sf::Event::JoystickMoveEvent	move;
	};

	SEEvent(eventType t = EVENT_DEFAULT, float d = 0.0f, int num = 0, SEGameObject **obj = NULL) :
		type(t),
		delay(d),
		numOfObjects(num){
		if (num > 0) {
			pObjs = new SEGameObject*[num];
			for (int i = 0; i < num; ++i) pObjs[i] = obj[i];
		}
	}

	~SEEvent() { if (numOfObjects > 0) delete[] pObjs; }
};

namespace se_system {

class SEEventManager : public SESystem<SEEventManager> {
	friend SESystem<SEEventManager>;
public:
	void update(sf::Window &window);
	void broadcast(SEEvent e) { eventQueue.push_back(e); }

private:
	SEEventManager() {}
	std::list<SEEvent> eventQueue;
};

}

#endif