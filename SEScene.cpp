#include "SEScene.h"

#include "SELog.h"
#include "SEComCamera.h"
#include "SESin.h"

unsigned SEScene::load(SEGameObject *obj) {
	if (!validate(*obj)) return 0;
	SEGameObject *pObj = new SEGameObject(*obj);
	gameObjs.push_back(std::list<SEGameObject*>(1, pObj));
	if ((*obj)[COM_CAMERA]) {
		CameraObjs.push_back(obj);
	}
	return gameObjs.size();
}

bool SEScene::instantiate(unsigned id, unsigned num) {
	SEGameObject* pObj = gameObjs[id].front();
#ifdef SE_DEBUG
	char log[256];
	const char* name = pObj->toString();
	if (--id >= gameObjs.size()) {
		SE_LogManager.append(se_debug::LOGTYPE_ERROR,
			"Invalid game object id can not be instantiated.");
		return false;
	}
	if ((*pObj)[COM_CAMERA]) {
		sprintf(log, "%s> Same camera object instantiation won't draw more than once", name);
		SE_LogManager.append(se_debug::LOGTYPE_WARNNING, log);
	}
#endif
	for (; num > 0; --num) {
		pObj = new SEGameObject(*gameObjs[id].front());
#ifdef SE_DEBUG
		if (!pObj) {
			sprintf(log, "Not enougn memory for %s instantiation.", name);
			SE_LogManager.append(se_debug::LOGTYPE_ERROR, log);
			return false;
		}
#endif
		gameObjs[id].push_back(pObj);
	}
#ifdef SE_DEBUG
	free((void*)name);
#endif

	return true;
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

void SEScene::clear() {
	for (auto i : gameObjs) {
		for (auto j : i)
			delete j;
		i.clear();
	}
	gameObjs.clear();
}

void SEScene::handle(SEEvent &event) {
	for (auto i : listeners) {
		i->handle(event);
	}
}

void SEScene::init() {
	for (auto i : gameObjs) {
		for (auto j : i)
			j->onInit();
	}
}

void SEScene::update() {
	for (auto i : gameObjs) {
		for (auto j : i)
			j->onUpdate();
	}
}

void SEScene::draw() {
	for (auto c : CameraObjs) {
		SEComponent *pCamera = (*c)[COM_CAMERA];
		if (pCamera->isEnabled()) {
			SIN.setActiveCamera(pCamera);
			SIN.getActiveCamera()->preDraw();
			for (auto i : gameObjs) {
				for (auto j : i)
					j->onDraw();
			}
		}
	}
}

void SEScene::postUpdate() {
	for (auto i : gameObjs) {
		for (auto j : i)
			j->onPostUpdate();
	}
}

void SEScene::pause() {
	sceneFlags |= SCENE_PAUSE;
	for (auto i : gameObjs) {
		for (auto j : i)
			j->onPause();
	}
}

void SEScene::resume() {
	for (auto i : gameObjs) {
		for (auto j : i)
			j->onResume();
	}
}

void SEScene::release() {
	for (auto i : gameObjs) {
		for (auto j : i)
			j->onRelease();
	}
}