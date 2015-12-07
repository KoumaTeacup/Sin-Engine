#include "SEScene.h"

#include "SEComTransform.h"
#include "SEComCamera.h"
#include "SEComListener.h"
#include "SEComCollider.h"
#include "SEComLight.h"
#include "SESin.h"
#include "SEEvent.h"

unsigned SEScene::load(SEGameObject *pObj) {
	if (!validate(*pObj)) return 0;
	gameObjs.push_back(*pObj);
	SEGameObject &obj = gameObjs.back();
	return gameObjs.size();
}

unsigned SEScene::instantiate(unsigned obj) {
#ifdef SE_DEBUG
	if (obj > gameObjs.size() || obj == 0) {
		SE_LogManager.append(se_debug::LOGTYPE_ERROR,
			"Invalid game object id can not be instantiated.");
		return 0;
	}
	char log[256];
#endif
	SEGameObject *pObj = new SEGameObject(gameObjs[--obj]);
#ifdef SE_DEBUG
	const char* name = pObj->toString();
	if ((*pObj)[COM_CAMERA]) {
		sprintf(log, "%s> Instantiate same camera object may lead to redundant draw", name);
		SE_LogManager.append(se_debug::LOGTYPE_WARNNING, log);
	}
	free((void*)name);
#endif

	if ((*pObj)[COM_CAMERA]) cameras.insert(static_cast<SEComCamera*>((*pObj)[COM_CAMERA]));
	if ((*pObj)[COM_COLLIDER]) colliders.insert(static_cast<SEComCollider*>((*pObj)[COM_COLLIDER]));
	if ((*pObj)[COM_LIGHT]) lights.insert(static_cast<SEComLight*>((*pObj)[COM_LIGHT]));

	if (pObj->getCompNum() > COM_NUM)
		for (int i = COM_NUM; i < pObj->getCompNum(); ++i)
			if ((*pObj)[i]->getType() == COM_LISTENER)
				listeners.insert(static_cast<SEComListener*>((*pObj)[i]));

	unsigned id;
	gameInsts.push_back(pObj);
	if (idBase.size() > 0) {
		id = idBase.front();
		idBase.pop_front();
	}
	else {
		id = currentId++;
	}

	idMaps[id] = --gameInsts.end();

	if (afterInit) pObj->onInit();
	
	return id;
}

void SEScene::destroy(unsigned id) {
	SEGameObject *pInst = getInst(id);
#ifdef SE_DEBUG
	if (!pInst) {
		SE_LogManager.append(se_debug::LOGTYPE_ERROR,
			"Game instance can't be found, nothing was destroyed.");
		return;
	}
#endif
	if ((*pInst)[COM_CAMERA]) cameras.erase(static_cast<SEComCamera*>((*pInst)[COM_CAMERA]));
	if (pInst->getCompNum() > COM_NUM) {
		for (int i = COM_NUM; i < pInst->getCompNum(); ++i) {
			if ((*pInst)[i]->getType() == COM_LISTENER)
				listeners.erase(static_cast<SEComListener*>((*pInst)[i]));
		}
	}
	delete pInst;

	gameInsts.erase(idMaps[id]);
	idBase.push_back(id);
	idMaps.erase(id);
}

bool SEScene::validate(SEGameObject &obj) const {
#ifdef SE_DEBUG
	char log[256];
	const char* name = obj.toString();
	sprintf(log, "Loading %s...", name);
	SE_LogManager.append(se_debug::LOGTYPE_GENERAL, log);
	if (obj[COM_RENDERER]) {
		if (!obj[COM_TRANSFORM]) {
			sprintf(log, "%s> Renderers needs to have a Transform component for object to be rendered.", name);
			SE_LogManager.append(se_debug::LOGTYPE_CONTINUE, "failed.");
			SE_LogManager.append(se_debug::LOGTYPE_ERROR, log);
			
			return false;
		}
	}
	if (obj[COM_CAMERA]) {
		if (!obj[COM_TRANSFORM]) {
			sprintf(log, "%s> Camera needs to have a Transform component.", name);
			SE_LogManager.append(se_debug::LOGTYPE_CONTINUE, "failed.");
			SE_LogManager.append(se_debug::LOGTYPE_ERROR, log);

			return false;
		}
	}
	SE_LogManager.append(se_debug::LOGTYPE_CONTINUE, "success.");
	free((void*)name);
#endif
	return true;
}

void SEScene::unload() {
	gameObjs.clear();
}

void SEScene::handle(SEEvent &event) {
	for (auto i : listeners) {
		if (i->isEnabled() && i->getOwner().isEnabled())
			i->handle(event);
	}
}

void SEScene::collisionDetection() {
	SEVector3f dir;
	for (auto i = colliders.begin(); i != colliders.end(); ++i) {
		if ((*i)->isEnabled())
			for (auto j = i; j != colliders.end(); ++j)
				if (i != j && (*j)->isEnabled())
					if ((dir = (*i)->testWith(*j)).lengthSqaure() > 0.0f) {
						SEGameObject *pObjs[2];
						pObjs[0] = &(*i)->getOwner();
						pObjs[1] = &(*j)->getOwner();
						SEEvent event("", 0.0f, 2, pObjs, EVENT_COLLIDE);
						event.info.collisionDirction = dir;
						SE_EventManager.broadcast(event);
					}
	}
}

void SEScene::init() {
	for (auto i : gameInsts) {
		i->onInit();
	}
	afterInit = true;
}

void SEScene::update() {
	for (auto i : gameInsts) {
		if (i->isEnabled()) i->onUpdate();
	}
}

void SEScene::draw() {
	lightsInfo.clear();
	for (auto i : lights)
		if (i->isEnabled()) {
			SEVector3f decay = i->getDecay();
			SEVector3f color = i->getColor();
			SEVector3f pos = static_cast<SEComTransform*>(i->getOwner()[COM_TRANSFORM])->translation();
			lightsInfo.push_back(SEMatrix3f(pos, color, decay));
		}

	for (auto pCamera : cameras) {
		if (pCamera->isEnabled()) {
			SIN.setActiveCamera(pCamera);
			pCamera->preDraw();
			for (auto i : gameInsts) {
				if (i->isEnabled()) i->onDraw();
			}
		}
	}
}

void SEScene::postUpdate() {
	for (auto i : gameInsts) {
		if (i->isEnabled()) i->onPostUpdate();
	}
}

void SEScene::pause() {
	for (auto i : gameInsts) {
		if (i->isEnabled()) i->onPause();
	}
}

void SEScene::resume() {
	for (auto i : gameInsts) {
		if (i->isEnabled()) i->onResume();
	}
}

void SEScene::release() {
	for (auto i : gameInsts)
		delete i;
	gameInsts.clear();
	cameras.clear();
	listeners.clear();
	colliders.clear();
	idBase.clear();
	idMaps.clear();
	SE_EventManager.clear();
}

void SEScene::resize() {
	for (auto pCamera : cameras)
		if (pCamera->isEnabled())
			pCamera->onResize();
}

SEGameObject * SEScene::getInst(unsigned id)
{
#ifdef SE_DEBUG
	if (idMaps.find(id) == idMaps.end()) {
		SE_LogManager.append(se_debug::LOGTYPE_ERROR,
			"Accessing invalid Game instance id.");
		return NULL;
	}
#endif
	return *idMaps[id];
}
