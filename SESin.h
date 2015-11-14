#ifndef SESIN_H
#define SESIN_H

#include <SFML\Window.hpp>

#include "SESystem.h"

#include "SELog.h"
#include "SEUtility.h"
#include "SEScene.h"
#include "SEEvent.h"

#define SIN			SESin::getObj()
#define SIN_Release	SESin::getObj().cleanUp();SESin::release()

// New Components TO DOs
#define SE_CAMERA		SIN.getCamera(this)
#define SE_TRANSFORM	SIN.getTransform(this)
#define SE_RENDERER		SIN.getRenderer(this)
#define SE_COMPONENT(x)	SIN.getComponent(this, x)

#define PI 3.1415926536f

class SESin : public se_system::SESystem<SESin>{
	friend SESystem<SESin>;

public:

	bool init();
	void cleanUp();

	void begin(SEScene &scene);

	// Log methods
	void logConfig(char bits = SE_LOG_ENABLED) const { SE_LogConfig(bits); }
	void assert(bool exp, const char* msg = "User assertion failed.") const { SE_Assert(exp, msg); }

	// Utility methods
	void	setFPSLimit(int limit) const { SE_Utility.setFPSLimit(limit); }
	float	getFrameTime() const { return SE_Utility.getFrameTime(); }

	// Resource methods
	SE_File load(const char* filename) const { return SE_Resource.load(filename); }

	// Event methods
	void broadcast(SEEvent e) const  { SE_Broadcast(e); }

	// Input methods
	bool isKeyPressed(SE_KEY key) { return SE_InputManager.isKeyPressed(key); }
	bool isKeyPressed(int index, int button) { return SE_InputManager.isKeyPressed(index, button); }
	float getJoystickPos(int index, SE_Axis axis) { return SE_InputManager.getJoystickPos(index, axis); }

	// Component Access
	// Debug mode does one more step of check
	SEComCamera		&getCamera(SEComponent *comp) const;
	SEComRenderer	&getRenderer(SEComponent *comp) const;
	SEComTransform	&getTransform(SEComponent *comp) const;
	SEComponent		&getComponent(SEComponent *comp, int id) const;

	// Setters & Getters
	void			setActiveCamera(SEComponent* pCam) { activeCamera = static_cast<SEComCamera*>(pCam); }
	SEComCamera*	getActiveCamera() const { return activeCamera; }
	SEVector2ui		getWindowSize() const { return SEVector2ui(window.getSize().x, window.getSize().y); }
	SEScene*		getActiveScene() const { return activeScene; }

	// Functional Methods
	float toRad(float degree) const { return degree / 180.0f*PI; }

private:
	SESin();

	sf::ContextSettings glSettings;
	sf::Window window;
	SEScene *activeScene;
	SEComCamera *activeCamera;
};
#endif