#ifndef SESCENE_H
#define SESCENE_H

#include <list>
#include <vector>

#include "SEGameObject.h"

#define SCENE_END 0x01
#define SCENE_PAUSE 0x02

class SEScene {
public:
	SEScene() {}
	~SEScene() {}

	// construction
	unsigned load(SEGameObject *pObj);
	bool loadScript(const char* filename);
	bool instantiate(unsigned id, unsigned num);
	bool validate(SEGameObject &obj) const;

	// modification
	void clear();

	// Flag access
	bool isEnd() { return ((sceneFlags & SCENE_END) != 0); }
	bool isPause() { return ((sceneFlags & SCENE_PAUSE) != 0); }
	void endScene() { sceneFlags |= SCENE_END; }

	// funcional methods
	void handle(SEEvent &event);

	// Setters & Getters

	// States call back methods.
	void init();
	void update();
	void draw();
	void postUpdate();
	void pause();
	void resume();
	void release();

	void resize();

private:
	// scenewise flags
	char sceneFlags;

	// Game objects container
	std::vector<std::list<SEGameObject*>> gameObjs;
	std::vector<SEGameObject*> cameraObjs;
	std::vector<SEComListener*> listeners;
};

#endif