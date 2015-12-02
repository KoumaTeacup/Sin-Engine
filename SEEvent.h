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
	EVENT_MOUSEMOVE,
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
	char infoString[64];

	union infoType{
		sf::Event::KeyEvent				key;
		sf::Event::JoystickButtonEvent	button;
		sf::Event::JoystickMoveEvent	move;
		struct {
			struct { int x, y; } pos;
			struct { int x, y; } delta;
		} mouseMove;
		
		SEVector3f						collisionDirction;

		infoType() { memset(this, 0, sizeof(infoType)); }
		infoType(const infoType &rhs) { memcpy(this, &rhs, sizeof(infoType)); }
		infoType& operator=(const infoType& rhs) { memcpy(this, &rhs, sizeof(infoType)); }
	} info;

	SEEvent(const char* i = "", float d = 0.0f, int num = 0, SEGameObject **obj = NULL, eventType t = EVENT_DEFAULT) :
		type(t),
		delay(d),
		numOfObjects(num){
		strcpy(infoString, i);
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
	int oldMouseX, oldMouseY;
	SEEventManager(): oldMouseX(-1), oldMouseY(-1) {}
	std::list<SEEvent> eventQueue;
};

}

#endif