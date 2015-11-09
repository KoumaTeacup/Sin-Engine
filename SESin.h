#ifndef SESIN_H
#define SESIN_H

#include <SFML\Window.hpp>

#include "SELog.h"
#include "SEUtility.h"
#include "SEScene.h"

#define SIN			SESin::getObj()
#define SIN_Release	SESin::release()

class SESin {

public:

	static SESin&	 getObj();
	static void		 release();

	bool init();
	void begin(SEScene &scene);

	// Log methods
	void logConfig(char bits = SE_LOG_ENABLED) { SE_LogConfig(bits); }
	void assert(bool exp, const char* msg = "User assertion failed.") { SE_Assert(exp, msg); }

	// Utility methods
	void	setFPSLimit(int limit) { SE_Utility.setFPSLimit(limit); }
	float	getFrameTime() { return SE_Utility.getFrameTime(); }

	// Resource methods
	SE_File load(const char* filename) { return SE_Resource.load(filename); }

	// Setters & Getters
	void			setActiveCamera(SEComponent* pCam) { activeCamera = static_cast<SEComCamera*>(pCam); }
	SEComCamera*	getActiveCamera() { return activeCamera; }
	SEVector2ui		getWindowSize() { return SEVector2ui(window.getSize().x, window.getSize().y); }

private:
	SESin();

	sf::ContextSettings glSettings;
	sf::Window window;
	SEScene *activeScene;
	SEComCamera *activeCamera;
	static SESin *pSin;
};
#endif