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
	void load();
	void begin(SEScene &scene);
	void end(SEScene &scene);

	// Log methods
	void logConifg(char bits = SE_LOG_ENABLED) { SE_LogManager.config(bits); }
	void assert(bool exp, const char* msg = "User assertion failed.") { SE_LogManager.assert(exp, msg); }

	// Utility methods
	void	setFPSLimit(int limit) { SE_Utility.setFPSLimit(limit); }
	float	getFrameTime() { return SE_Utility.getFrameTime(); }

	// Resource methods

private:
	SESin();

	sf::ContextSettings glSettings;
	sf::Window window;
	static SESin *pSin;
};
#endif