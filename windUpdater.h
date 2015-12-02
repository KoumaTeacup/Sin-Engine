#ifndef WINDUPDATER_H
#define WINDUPDATER_H

#include "SEComUser.h"
#include "SEVector.h"

class WindUpdater : public SEComUser {
public:
	WindUpdater() : globalWind(0.0f, 0.0f, 0.0f){}
	~WindUpdater(){}

	void onUpdate();

	SEComponent *clone() const { return new WindUpdater(*this); }

private:
	SEVector3f globalWind;
};

#endif