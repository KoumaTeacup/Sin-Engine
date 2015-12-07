#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "SEComListener.h"

// Any class derived from Listener must implement handle()
class SEComController : public SEComListener {
public:
	SEComController():SEComListener(){}

	void handle(SEEvent &event);

	SEComponent *clone() const { return new SEComController(*this); }

protected:
	void onUpdate();

private:
};

#endif