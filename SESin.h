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

#define PI 3.1415926536f

class SESin : public se_system::SESystem<SESin>{
	friend SESystem<SESin>;

public:

	bool init();
	void cleanUp();

	void begin(SEScene &scene);

	// Log methods
	void logConfig(char bits = SE_LOG_ENABLED) { SE_LogConfig(bits); }
	void assert(bool exp, const char* msg = "User assertion failed.") { SE_Assert(exp, msg); }

	// Utility methods
	void	setFPSLimit(int limit) { SE_Utility.setFPSLimit(limit); }
	float	getFrameTime() { return SE_Utility.getFrameTime(); }

	// Resource methods
	SE_File load(const char* filename) { return SE_Resource.load(filename); }

	// Event methods
	void broadcast(SEEvent e) { SE_Broadcast(e); }

	// Setters & Getters
	void			setActiveCamera(SEComponent* pCam) { activeCamera = static_cast<SEComCamera*>(pCam); }
	SEComCamera*	getActiveCamera() { return activeCamera; }
	SEVector2ui		getWindowSize() { return SEVector2ui(window.getSize().x, window.getSize().y); }
	SEScene*		getActiveScene() { return activeScene; }

	// Functional Methods
	float toRad(float degree) { return degree / 180.0f*PI; }

private:
	SESin();

	sf::ContextSettings glSettings;
	sf::Window window;
	SEScene *activeScene;
	SEComCamera *activeCamera;
};
#endif