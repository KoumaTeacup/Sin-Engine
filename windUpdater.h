#ifndef WINDUPDATER_H
#define WINDUPDATER_H

#include "SEComUser.h"
#include "SEVector.h"

class WindUpdater : public SEComUser {
public:
	WindUpdater(){}
	~WindUpdater(){}

	void onUpdate();

private:
	SEVector3f globalWind;
};

#endif