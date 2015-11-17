#ifndef SEEVENT_H
#define SEEVENT_H

#include <SFML\Window.hpp>

#include <list>
#include "SESystem.h"
#include "SEVector.h"

class SEGameObject;

#define SE_EventManager	se_system::SEEventManager::getObj()
#define SE_Broadcast	se_system::SEEventManager::getObj().broadcast

enum eventType {
	EVENT_DEFAULT,
	EVENT_COLLIDE,
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

	union infoType{
		sf::Event::KeyEvent				key;
		sf::Event::JoystickButtonEvent	button;
		sf::Event::JoystickMoveEvent	move;
		
		SEVector3f						collisionDirction;
		char							infoString[64];

		infoType() { memset(this, 0, sizeof(infoType)); }
		infoType(const infoType &rhs) { memcpy(this, &rhs, sizeof(infoType)); }
		infoType& operator=(const infoType& rhs) { memcpy(this, &rhs, sizeof(infoType)); }
	} info;

	SEEvent(const char* i = "", float d = 0.0f, int num = 0, SEGameObject **obj = NULL, eventType t = EVENT_DEFAULT) :
		type(t),
		delay(d),
		numOfObjects(num){
		strcpy(info.infoString, i);
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
	void updateWindowEvent(sf::Window &window);
	void broadcast(SEEvent e) { eventQueue.push_back(e); }
	void distribute();

	void clear() { eventQueue.clear(); }

private:
	SEEventManager() {}
	std::list<SEEvent> eventQueue;
};

}

#endif