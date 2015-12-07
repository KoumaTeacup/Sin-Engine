#include <GL\glew.h>

#include "SESin.h"

#include "SEGameObject.h"
#include "SELog.h"
#include "SEUtility.h"
#include "SEResource.h"
#include "SEScene.h"
#include "SEEvent.h"
#include "SEInput.h"
#include "SEComCamera.h"
#include "SEComRenderer.h"
#include "SEComTransform.h"
#include "SEComRigidBody.h"
#include "SEComCollider.h"
#include "SEComLight.h"
#include "SEComConstraint.h"
#include "SEComTemp.h"

SESin::SESin():activeScene(NULL), activeCamera(NULL), windowSize(800, 600) {
	glSettings.depthBits = 24;
	glSettings.stencilBits = 8;
	glSettings.antialiasingLevel = 4;
	glSettings.majorVersion = 3;
	glSettings.minorVersion = 0;
}

bool SESin::init(){
	//Initiaize SE_LogManager, which is necessary to redirect sf::err
	//Have to called before sf:: calls.
#ifdef SE_DEBUG
	SE_LogManager.config(SE_LOG_LOCAL | SE_LOG_ENABLED);
#endif

	// create the window
	window.create(sf::VideoMode(windowSize[0], windowSize[1]), "Sin Engine", sf::Style::Default, glSettings);
	window.setVerticalSyncEnabled(true);

	// Initialize glew
	GLenum res = glewInit();
#ifdef SE_DEBUG
	if (res != GLEW_OK) {
		SE_LogManager.append(se_debug::LOGTYPE_OPENGL, (const char*)glewGetErrorString(res));
		return false;
	}
#endif

	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_TEXTURE_2D);
	return true;
}

void SESin::cleanUp() {
	// Release all global managers
#ifdef SE_DEBUG
	SE_LogManager.print();
	SE_LogManager_Release;
#endif
	SE_Utility_Release;
	SE_Resource_Release;
}

void SESin::beginScene(SEScene &scene) {
	activeScene = &scene;
	activeSceneState = true;

	scene.init();
	// run the main loop
	while (!scene.isEnd())
	{
		if (activeSceneState && scene.isPause()) {
			scene.pause();
			activeSceneState = false;
		}
		if (!activeSceneState && !scene.isPause()) {
			scene.resume();
			activeSceneState = true;
		}
		if (activeSceneState) {
			// Process window/input events
			SE_EventManager.updateWindowEvent(window);
			// Process local events
			scene.update();
			// Collision Dectection
			scene.collisionDetection();
			// Distribute & handle events
			SE_EventManager.distribute();
			// Draw scene
			scene.draw();
			// Swap buffer
			window.display();
			// Post draw business
			scene.postUpdate();
			// Leave current frame
			SE_Utility.update();
		}
	}

	scene.release();
}

void SESin::pauseScene() {
	activeScene->sceneFlags |= SCENE_PAUSE;
}

void SESin::resumeScene() {
	activeScene->sceneFlags &= ~SCENE_PAUSE;
}

void SESin::endScene() {
	activeScene->sceneFlags |= SCENE_END;
}

void SESin::logConfig(char bits = SE_LOG_ENABLED) const {
	SE_LogConfig(bits);
}

void SESin::assert(bool exp, const char* msg = "User assertion failed.") const {
	SE_Assert(exp, msg);
}

void SESin::setFPSLimit(int limit) const {
	SE_Utility.setFPSLimit(limit);
}

float SESin::getFrameTime() const{
	return SE_Utility.getFrameTime();
}

void SESin::backFaceCulling() const
{
	SE_Utility.backFaceCulling();
}

void SESin::broadcast(SEEvent e) const {
	SE_Broadcast(e);
}

SEVector2ui SESin::getMousePosition() const { 
	return SE_EventManager.getMousePosition(); 
}

bool SESin::isKeyPressed(SE_KEY key) {
	return SE_InputManager.isKeyPressed(key);
}

bool SESin::isKeyPressed(int index, int button) {
	return SE_InputManager.isKeyPressed(index, button);
}

float SESin::getJoystickPos(int index, SE_Axis axis) {
	return SE_InputManager.getJoystickPos(index, axis);
}

SEComCamera &SESin::getCamera(const SEComponent * comp) const{
	SEGameObject &pObj = comp->getOwner();
#ifdef SE_DEBUG
	if (!pObj[COM_CAMERA]) {
		char log[256];
		const char *name = comp->toString();
		sprintf(log, "%s> Accessing invalid Camera component.", name);
		SE_LogManager.append(se_debug::LOGTYPE_WARNNING, log);
		free((void*)name);
		return SEComCamera(NULL);
	}
	else
#endif
		return (*static_cast<SEComCamera*>(pObj[COM_CAMERA]));
}

SEComRenderer &SESin::getRenderer(const SEComponent * comp) const {
	SEGameObject &pObj = comp->getOwner();
#ifdef SE_DEBUG
	if (!pObj[COM_RENDERER]) {
		char log[256];
		const char *name = comp->toString();
		sprintf(log, "%s> Accessing invalid Renderer component.", name);
		SE_LogManager.append(se_debug::LOGTYPE_WARNNING, log);
		free((void*)name);
		return SEComRenderer(NULL);
	}
	else
#endif
		return (*static_cast<SEComRenderer*>(pObj[COM_RENDERER]));
}

SEComTransform &SESin::getTransform(const SEComponent * comp) const {
	SEGameObject &pObj = comp->getOwner();
#ifdef SE_DEBUG
	if (!pObj[COM_TRANSFORM]) {
		char log[256];
		const char *name = comp->toString();
		sprintf(log, "%s> Accessing invalid Transform component.", name);
		SE_LogManager.append(se_debug::LOGTYPE_WARNNING, log);
		free((void*)name);
		return SEComTransform(NULL);
	}
	else
#endif
		return (*static_cast<SEComTransform*>(pObj[COM_TRANSFORM]));
}

SEComRigidBody & SESin::getRigidBody(const SEComponent * comp) const {
	SEGameObject &pObj = comp->getOwner();
#ifdef SE_DEBUG
	if (!pObj[COM_RIGIDBODY]) {
		char log[256];
		const char *name = comp->toString();
		sprintf(log, "%s> Accessing invalid RigidBody component.", name);
		SE_LogManager.append(se_debug::LOGTYPE_WARNNING, log);
		free((void*)name);
		return SEComRigidBody(NULL);
	}
	else
#endif
		return (*static_cast<SEComRigidBody*>(pObj[COM_RIGIDBODY]));
}

SEComCollider & SESin::getCollider(const SEComponent * comp) const {
	SEGameObject &pObj = comp->getOwner();
#ifdef SE_DEBUG
	if (!pObj[COM_COLLIDER]) {
		char log[256];
		const char *name = comp->toString();
		sprintf(log, "%s> Accessing invalid Collider component.", name);
		SE_LogManager.append(se_debug::LOGTYPE_WARNNING, log);
		free((void*)name);
		return SEComCollider(NULL);
	}
	else
#endif
		return (*static_cast<SEComCollider*>(pObj[COM_COLLIDER]));
}

SEComLight & SESin::getLight(const SEComponent * comp) const {
	SEGameObject &pObj = comp->getOwner();
#ifdef SE_DEBUG
	if (!pObj[COM_LIGHT]) {
		char log[256];
		const char *name = comp->toString();
		sprintf(log, "%s> Accessing invalid Light component.", name);
		SE_LogManager.append(se_debug::LOGTYPE_WARNNING, log);
		free((void*)name);
		return SEComLight(NULL);
	}
	else
#endif
		return (*static_cast<SEComLight*>(pObj[COM_LIGHT]));
}

SEComConstraint & SESin::getConstraint(const SEComponent * comp) const {
	SEGameObject &pObj = comp->getOwner();
#ifdef SE_DEBUG
	if (!pObj[COM_CONSTRAINT]) {
		char log[256];
		const char *name = comp->toString();
		sprintf(log, "%s> Accessing invalid Constraint component.", name);
		SE_LogManager.append(se_debug::LOGTYPE_WARNNING, log);
		free((void*)name);
		return SEComConstraint(NULL);
	}
	else
#endif
		return (*static_cast<SEComConstraint*>(pObj[COM_CONSTRAINT]));
}

SEComponent &SESin::getComponent(const SEComponent * comp, int id) const {
	SEGameObject &pObj = comp->getOwner();
#ifdef SE_DEBUG
	if (pObj[id]->getType() == COM_UNDEFINED) {
		char log[256];
		const char *name = comp->toString();
		sprintf(log, "%s> Accessing invalid user defined component.", name);
		SE_LogManager.append(se_debug::LOGTYPE_WARNNING, log);
		free((void*)name);
		return SEComTemp();
	}
	else
#endif
		return (*pObj[id]);
}

void SESin::setActiveCamera(SEComponent * pCam) {
	activeCamera = static_cast<SEComCamera*>(pCam);
}

SEVector2ui SESin::getWindowSize() const {
	return SEVector2ui(window.getSize().x, window.getSize().y);
}

unsigned SESin::getActiveLightsNum() const
{
	return activeScene->getLightsNum();
}

const std::vector<SEMatrix3f>& SESin::getActiveLightsInfo() const
{
	return activeScene->getLightsInfo();
}
