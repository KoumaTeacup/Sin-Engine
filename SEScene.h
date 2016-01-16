#ifndef SESCENE_H
#define SESCENE_H

#include <set>
#include <vector>
#include <list>
#include <map>
#include <queue>

#include "SEGameObject.h"
#include "SEMatrix.h"

class SEComListener;
class SEComCamera;
class SEComCollider;
class SEComLight;
class SESin;

struct SEEvent;

#define SCENE_END 0x01
#define SCENE_PAUSE 0x02

class SEScene {
	friend SESin;
public:
	SEScene() :currentId(0), afterInit(false) {}
	~SEScene() {}

	// construction
	unsigned		load(SEGameObject *pObj);
	bool			validate(SEGameObject &obj) const;
	bool			loadScript(const char* filename) {}
	unsigned		instantiate(unsigned objId);
	unsigned		InstantiateOnce(SEGameObject *pObj) { return instantiate(load(pObj)); }
	void			destroy(unsigned id);

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
	SEGameObject *getInst(unsigned id);

	const std::vector<SEMatrix3f> &getLightsInfo() const { return lightsInfo; }
	const unsigned getLightsNum() const { return lights.size(); }

	// public member variables
	float globalTimeScale;
	float globalSizeScale;
	float globalMassScale;

private:
	unsigned currentId;
	bool afterInit;
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
	std::list<SEGameObject*> gameInsts;
	std::map<unsigned, std::list<SEGameObject*>::iterator> idMaps;
	std::list<int> idBase;

	// Components container
	std::set<SEComCamera*> cameras;
	std::set<SEComListener*> listeners;
	std::set<SEComCollider*> colliders;
	std::set<SEComLight*> lights;

	// |Position.x, Position.y, Position.z	|
	// |Color.r,	Color.g,	Color.b		|
	// |decay.1,	decay.2,	decay.3		|
	std::vector<SEMatrix3f> lightsInfo;

	// State change container
	std::vector<SEGameObject> toInstantiate;
	std::vector<unsigned> toDestroy;
};

#endif