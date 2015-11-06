#ifndef SESIN_H
#define SESIN_H

#include "SELog.h"
#include "SEUtility.h"

#define SIN		SESin::getObj()

class SESin {
public:

	static SESin&	 getObj();
	static void		 release();

	// Log methods
	void logConifg() {};
	void assert() {};
	void append() {};
	void print() {};

	// Utility methods
	void	setFPSLimit() {};
	float	getFrameTime() {};

	// Resource methods

private:
	SESin() {};

	static SESin *pSin;
};
#endif