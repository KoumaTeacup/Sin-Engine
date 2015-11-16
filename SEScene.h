#ifndef SESCENE_H
#define SESCENE_H

#include <set>
#include <vector>

#include "SEGameObject.h"

class SEComListener;
class SEComCamera;
class SEComCollider;
class SESin;

struct SEEvent;

#define SCENE_END 0x01
#define SCENE_PAUSE 0x02

class SEScene {
	friend SESin;
public:
	SEScene() {}
	~SEScene() {}

	// construction
	unsigned		load(SEGameObject *pObj);
	bool			validate(SEGameObject &obj) const;
	bool			loadScript(const char* filename) {}
	SEGameObject*	instantiate(unsigned objId);
	SEGameObject*	InstantiateOnce(SEGameObject *pObj) { return instantiate(load(pObj)); }
	void			destroy(SEGameObject *pInst);

	// modification
	void unload();

	// Flag access
	bool isEnd() { return ((sceneFlags & SCENE_END) != 0); }
	bool isPause() { return ((sceneFlags & SCENE_PAUSE) != 0); }

	// funcional methods
	void handle(SEEvent &event);
	void collisionDetection();
	void resize();

	// Setters & Getters

	// public member variables
	float globalTimeScale;
	float globalSizeScale;
	float globalMassScale;

private:
	// States call back methods.
	void init();
	void update();
	void draw();
	void postUpdate();
	void pause();
	void resume();
	void release();

	// scenewise flags
	char sceneFlags;

	// Game objects container
	std::vector<SEGameObject> gameObjs;
	std::set<SEGameObject*> gameInsts;
	std::set<SEComCamera*> cameras;
	std::set<SEComListener*> listeners;
	std::set<SEComCollider*> colliders;
};

#endif