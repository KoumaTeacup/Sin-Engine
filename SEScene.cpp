#include "SEScene.h"

#include "SEComCamera.h"
#include "SEComListener.h"
#include "SEComCollider.h"
#include "SESin.h"
#include "SEEvent.h"

unsigned SEScene::load(SEGameObject *pObj) {
	if (!validate(*pObj)) return 0;
	gameObjs.push_back(*pObj);
	SEGameObject &obj = gameObjs.back();
	return gameObjs.size();
}

SEGameObject* SEScene::instantiate(unsigned obj) {
#ifdef SE_DEBUG
	if (obj > gameObjs.size()) {
		SE_LogManager.append(se_debug::LOGTYPE_ERROR,
			"Invalid game object id can not be instantiated.");
		return NULL;
	}
	char log[256];
#endif
	SEGameObject *pObj = new SEGameObject(gameObjs[--obj]);
#ifdef SE_DEBUG
	const char* name = pObj->toString();
	if ((*pObj)[COM_CAMERA]) {
		sprintf(log, "%s> Same camera object instantiation won't draw more than once", name);
		SE_LogManager.append(se_debug::LOGTYPE_WARNNING, log);
	}
#endif
	gameInsts.insert(pObj);
#ifdef SE_DEBUG
	free((void*)name);
#endif
	if ((*pObj)[COM_CAMERA]) cameras.insert(static_cast<SEComCamera*>((*pObj)[COM_CAMERA]));
	if (pObj->getCompNum() > COM_NUM) {
		for (int i = COM_NUM; i < pObj->getCompNum(); ++i) {
			if ((*pObj)[i]->getType() == COM_LISTENER)
				listeners.insert(static_cast<SEComListener*>((*pObj)[i]));
			else if ((*pObj)[i]->getType() == COM_COLLIDER)
				colliders.insert(static_cast<SEComCollider*>((*pObj)[i]));
		}
	}
	return pObj;
}

void SEScene::destroy(SEGameObject *pInst) {
	if ((*pInst)[COM_CAMERA]) cameras.erase(static_cast<SEComCamera*>((*pInst)[COM_CAMERA]));
	if (pInst->getCompNum() > COM_NUM) {
		for (int i = COM_NUM; i < pInst->getCompNum(); ++i) {
			if ((*pInst)[i]->getType() == COM_LISTENER)
				listeners.erase(static_cast<SEComListener*>((*pInst)[i]));
		}
	}
	delete pInst;
	gameInsts.erase(pInst);
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
					if ((dir = (*i)->testWith(*j)).lengthSqaure() < FLOAT_OFFSET) {
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
}

void SEScene::update() {
	for (auto i : gameInsts) {
		if (i->isEnabled()) i->onUpdate();
	}
}

void SEScene::draw() {
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
	SE_EventManager.clear();
}

void SEScene::resize() {
	for (auto pCamera : cameras)
		if (pCamera->isEnabled())
			pCamera->onResize();
}